// Includes
#include <iostream>
#include <mutex>
#include "preload.h"

/**
 * @brief Initializes all the needed variables
 * @param type Type of the experiment.
 * @param forceOrStress Indicates if experiment is force or stress based.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param stressForceLimit Stress or force limit value.
 * @param speedInMM Speed in mm/s.
 * @param area Value of the area.
 */
Preload::Preload(ExperimentType type,
                 StressOrForce forceOrStress,
                 StageFrame *stageframe,
                 ForceSensorMessageHandler *forcesensormessagehandler,
                 mpFXYVector *vector,
                 std::mutex *vectoraccessmutex,
                 MyFrame *myframe,
                 std::string path,
                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,
                 double stressForceLimit,
                 double speedInMM,
                 double area)
  : Experiment(type,
               forceOrStress,
               stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,
               Direction::Stop, 0.3/*stress force threshold*/,
               0.01/*distance threshold*/,
               area),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_StagesStoppedFlag(stagesstopped),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_ForceStressLimit(stressForceLimit),
    m_SpeedInMM(speedInMM),
    m_Area(area * 0.000000000001/*um^2*/)
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

Preload::~Preload(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  // Delete the experiment values because we don't need them for the preloading.
  delete m_ExperimentValues;
  std::cout << "Preload destructor finished." << std::endl;
}

/**
 * @brief Sets the area.
 * @param x Length in x direction.
 * @param y Length in y direction.
 */
void Preload::setArea(double x, double y){
  m_Area = x * y;
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Preload::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  if(UpdatedValuesReceiver::ValueType::Force == type){
    m_CurrentForce = measurementValue.value / 10000.0;

    process(Event::evUpdate);
  }
}

/**
 * @brief FSM of the preload process
 * @param e Occuring event
 */
void Preload::process(Event e){
  switch(m_CurrentState){
    case stopState:
      if(evStart == e){
        //std::cout << "Preload FSM switched to state: runState." << std::endl;
        m_StageFrame->setSpeed(m_SpeedInMM);
        m_CurrentState = runState;
        m_ExperimentValues->setStartPoint();

        // If force based
        if(StressOrForce::Force == m_StressOrForce){
          if((m_CurrentForce - m_ForceStressLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMM);
          }else if((m_ForceStressLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMM);
          }
        }else if(StressOrForce::Stress == m_StressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_ForceStressLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMM);
          }else if((m_ForceStressLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMM);
          }

        }
      }
      break;
    case runState:
      if(evStop == e){
        std::cout << "Preload FSM switched to state: stopState." << std::endl;

        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        {
          std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
          *m_StagesStoppedFlag = false;
        }
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_one();
      }
      if(evUpdate == e){
        // If force based
        if(StressOrForce::Force == m_StressOrForce){
          if((m_CurrentForce - m_ForceStressLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMM);
            }
          }else if((m_ForceStressLimit - m_CurrentForce) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_SpeedInMM);
            }
          }else{

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Backwards == m_CurrentDirection)){
              m_CurrentState = stopState;
              m_CurrentDirection = Direction::Stop;
              {
                std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
                *m_StagesStoppedFlag = false;
              }
              m_StageFrame->stop();
              std::lock_guard<std::mutex> lck(*m_WaitMutex);
              m_Wait->notify_one();
            }
          }
        }else if(StressOrForce::Stress == m_StressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_ForceStressLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMM);
            }
          }else if((m_ForceStressLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_SpeedInMM);
            }
          }else{

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Backwards == m_CurrentDirection)){
              m_CurrentState = stopState;
              m_CurrentDirection = Direction::Stop;
              {
                std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
                *m_StagesStoppedFlag = false;
              }
              m_StageFrame->stop();
              std::lock_guard<std::mutex> lck(*m_WaitMutex);
              m_Wait->notify_one();
            }
          }

        }
      }
      break;
  }
}
