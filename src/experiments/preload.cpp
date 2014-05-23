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
Preload::Preload(std::shared_ptr<StageFrame> stageframe,
                 std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                 mpFXYVector *vector,
                 std::mutex *vectoraccessmutex,
                 mpFXYVector *maxlimitvector,
                 mpFXYVector *minlimitvector,
                 MyFrame *myframe,
                 std::string path,
                 long maxforcelimit,
                 long minforcelimit,
                 long maxdistancelimit,
                 long mindistancelimit,

                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,

                 ExperimentType type,
                 DistanceOrStressOrForce distanceOrStressOrForce,
                 long gagelength,
                 long zerodistance,
                 long currentdistance,
                 double area,

                 double stressForceLimit,
                 double speedInMM)
  : Experiment(stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,
               maxforcelimit,
               minforcelimit,
               maxdistancelimit,
               mindistancelimit,

               type,
               distanceOrStressOrForce,
               Direction::Stop,
               gagelength,
               zerodistance,
               currentdistance,
               area,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_StagesStoppedFlag(stagesstopped),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_StressForceLimit(stressForceLimit),
    m_SpeedInMM(speedInMM),
    m_ExperimentValues(std::make_shared<PreloadValues>(stageframe,
                                                       forcesensormessagehandler,
                                                       vector,
                                                       vectoraccessmutex,
                                                       maxlimitvector,
                                                       minlimitvector,

                                                       myframe,

                                                       type,
                                                       distanceOrStressOrForce,
                                                       area,

                                                       stressForceLimit,
                                                       speedInMM))
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void Preload::setPreloadDistance(long preloaddistance){

}

Preload::~Preload(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  // Delete the experiment values because we don't need them for the preloading.
  //delete m_ExperimentValues;
  std::cout << "Preload destructor finished." << std::endl;
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void Preload::getPreview(std::vector<PreviewValue> &previewvalue){
  int timepoint;
  if(0 == previewvalue.size()){
   timepoint = 1;
  } else{
   timepoint =  previewvalue.back().getTimepoint() + 1;
  }
  previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StressForceLimit));
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
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      if((true == m_CheckLimitsFlag) && ((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce))){
        std::cout << "OneStepEvent: Force limit exceeded." << std::endl;
        process(Event::evStop);
      } else{
        process(Event::evUpdate);
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      if((true == m_CheckLimitsFlag) && ((m_MaxDistanceLimit < m_CurrentDistance) || (m_MinDistanceLimit > m_CurrentDistance))){
        std::cout << "OneStepEvent: Distance limit exceeded." << std::endl;
        process(Event::evStop);
      }
      break;
  }
}

/**
 * @brief Returns a pointer to the experiment values.
 * @return A pointer to the experiment values.
 * @todo throw exception if pointer is NULL.
 */
std::shared_ptr<ExperimentValues> Preload::getExperimentValues(void){
  return(m_ExperimentValues);
}

/**
 * @brief FSM of the preload process
 * @param e Occuring event
 */
void Preload::process(Event e){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == e){
        //std::cout << "Preload FSM switched to state: runState." << std::endl;
        m_StageFrame->setSpeed(m_SpeedInMM);
        m_CurrentState = runState;
        m_CheckLimitsFlag = true;

        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMM);
          }else if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMM);
          }
        }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_StressForceLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMM);
          }else if((m_StressForceLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMM);
          }

        }
      }
      break;
    case runState:
      if(Event::evStop == e){
        std::cout << "Preload FSM switched to state: stopState." << std::endl;

        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        {
          std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
          *m_StagesStoppedFlag = false;
        }
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == e){
        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_StressForceLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMM);
            }
          }else if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_StressForceLimit - m_CurrentForce << std::endl;

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
              m_Wait->notify_all();
            }
          }
        }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_StressForceLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMM);
            }
          }else if((m_StressForceLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // Only reverse motor, if state changed
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
              std::cout << "Stop preloading." << std::endl;
              m_StageFrame->stop();
              std::lock_guard<std::mutex> lck(*m_WaitMutex);
              m_Wait->notify_all();
            }
          }

        }
      }
      break;
  }
}

/**
 * @brief Do all the required thing to stop the experiment during process.
 */
void Preload::resetExperiment(void){
  m_CurrentState = stopState;
  m_CheckLimitsFlag = false;
  m_StageFrame->stop();
}
