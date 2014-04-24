#include <iostream>
#include <mathplot.h>
#include "creep.h"

/**
 * @brief Initializes all the needed variables.
 * @param type Type of the experiment.
 * @param stressOrForce Indicates if the experiment is stress or force based.
 * @param stageframe Pointer to the stage frame object.
 * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param holdstressforce The hold stress/force value.
 * @param holdtime The hold time.
 * @param sensitivity The sensitivity of the hold stress/force.
 * @param speedinmm Speed in mm/s.
 * @param area Value of the area.
 */
Creep::Creep(Experiment::ExperimentType type,
             StressOrForce stressOrForce,
             StageFrame *stageframe,
             std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
             ForceSensorMessageHandler *forcesensormessagehandler,
             mpFXYVector *vector,
             std::mutex *vectoraccessmutex,
             MyFrame *myframe,
             std::condition_variable *wait,
             std::mutex *mutex,
             long holdstressforce, double holdtime, double sensitivity, double speedinmm, double area)
  : Experiment(type,
               stressOrForce,
               stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               Direction::Stop,
               sensitivity*1000/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/,
               area),
    m_HoldStressOrForce(stressOrForce),
    m_StageFrame(stageframe),
    m_LinearStageMessageHanders(linearstagemessagehandlers),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_HoldStressForce(holdstressforce * 1000),
    m_HoldTime(holdtime),
    m_Sensitivity(sensitivity),
    m_SpeedInMm(speedinmm),
    m_Area(area),
    m_HoldValueReachedFlag(false),
    m_EndFlag(false),
    m_WaitingThread(NULL)
{
  m_CurrentDirection = Direction::Stop;
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
Creep::~Creep(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief FSM of the Creep experiment
 * @param event Occuring event
 */
void Creep::process(Event e){
  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == e){
        m_StageFrame->setSpeed(m_SpeedInMm);
        m_CurrentState = runState;

        // If force based
        if(Experiment::StressOrForce::Force == m_HoldStressOrForce){
          if((m_CurrentForce - m_HoldStressForce) > m_ForceStressThreshold){ // If current force is too high.
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMm);
          }else if((m_HoldStressForce - m_CurrentForce) > m_ForceStressThreshold){ // If current force is too low.
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMm);
          }
        }else if(Experiment::StressOrForce::Stress == m_HoldStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_HoldStressForce) > m_ForceStressThreshold){ // If current stress is too high.
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMm);
          }else if((m_HoldStressForce - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // If current stress is too low.
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMm);
          }
        }
        std::cout << "Creep experiment started." << std::endl;
      }
      break;

    case runState:

      if(evStop == e){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_StageFrame->stop();
        std::cout << "Creep stopped." << std::endl;
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_one();
      }
      if(evUpdate == e){

        std::lock_guard<std::mutex> lck{m_EndMutex};
        // Check if the experiment was stopped by the hold time timer.
        if(false == m_EndFlag){

          // If force based
          if(Experiment::StressOrForce::Force == m_HoldStressOrForce){
            //std::cout << "(m_CurrentForce - m_HoldForce) > m_ForceStressThreshold " << (m_CurrentForce - m_HoldStressForce) << " " << m_ForceStressThreshold << std::endl;
            if((m_CurrentForce - m_HoldStressForce) > m_ForceStressThreshold){ // If current force is too high.
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Forwards;
                m_StageFrame->moveForward(m_SpeedInMm);
                std::cout << "Creep update." << std::endl;
              }
            }else if((m_HoldStressForce - m_CurrentForce) > m_ForceStressThreshold){ // If current force is too low.
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                m_StageFrame->moveBackward(m_SpeedInMm);
                std::cout << "Creep update." << std::endl;
              }
            }else{ // If current force is equal to the hold value with sensitivity.
              if(false == m_HoldValueReachedFlag){
                m_HoldValueReachedFlag = true;

                m_WaitingThread = new std::thread(&Creep::waitHoldTime, this, m_HoldTime);
                m_WaitingThread->detach();
              }
              m_CurrentDirection = Direction::Stop;
              m_StageFrame->stop();
                std::cout << "Creep update." << std::endl;
            }
          }else if(Experiment::StressOrForce::Stress == m_HoldStressOrForce){ // If stress based
            //std::cout << "(m_CurrentForce - m_HoldForce) > m_ForceStressThreshold " << (m_CurrentForce - m_HoldStressForce) << " " << m_ForceStressThreshold << std::endl;
            if((m_CurrentForce/m_Area - m_HoldStressForce) > m_ForceStressThreshold){ // If current stress is to high.
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Forwards;
                m_StageFrame->moveForward(m_SpeedInMm);
                std::cout << "Creep update." << std::endl;
              }
            }else if((m_HoldStressForce - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // If current stress is too low.
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                m_StageFrame->moveBackward(m_SpeedInMm);
                std::cout << "Creep update." << std::endl;
              }
            }else{ // If current stress is equal to the hold value with sensitivity.
              // Check if the hold value is reached the first time
              if(false == m_HoldValueReachedFlag){
                m_HoldValueReachedFlag = true;

                // Start the hold time timer.
                m_WaitingThread = new std::thread(&Creep::waitHoldTime, this, m_HoldTime);
                m_WaitingThread->detach();
              }
              if((Direction::Backwards == m_CurrentDirection) || (Direction::Forwards == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Stop;
                m_StageFrame->stop();
                std::cout << "Creep update." << std::endl;
              }
            }
          }
        }else{ // Experiment was stopped by the hold time timer.
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_StageFrame->stop();
          std::cout << "Creep stopped." << std::endl;
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_one();
        }
      }
      break;
  }
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Creep::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  process(Event::evUpdate);
}

/**
 * @brief Waits the hold time and after stops experiment.
 * @param time Witing time in sec.
 */
void Creep::waitHoldTime(double time){
  std::cout << "Creep start waiting for " << time * 1000 << " ms" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time * 1000)));
  std::cout << "Creep finished waiting." << std::endl;
  //process(Event::evStop);
  {
    std::lock_guard<std::mutex> lck{m_EndMutex};
    m_EndFlag = true;
  }
  process(Event::evUpdate);
}
