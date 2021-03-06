#include <iostream>
#include <cmath>
#include <thread>
#include "conditioning.h"

/**
 * @brief Initializes all the needed variables
 * @param type Type of the experiment.
 * @param distanceOrStressForce Indicates if experiment is distance or force/stress based.
 * @param forceOrStress Indicates if experiment is force or stress based.
 * @param stageframe Pointer to the stage frame object.
 * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param graph Pointer to the graph.
 * @param wait Wait condition.
 * @param mutex Mutex for wait condition.
 * @param stressForceLimit Stress or force limit value.
 * @param speedInMM Speed in mm/s.
 * @param area Value of the area.
 * @param preloaddistance Preload distance of the stage frame.
 */
Conditioning::Conditioning(std::shared_ptr<StageFrame> stageframe,
                           std::vector<LinearStageMessageHandler *> *linearstagemessagehandlers,
                           std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                           mpFXYVector *vector,
                           std::mutex *vectoraccessmutex,
                           MyFrame *myframe,
                           std::string path,
                           long maxforcelimit,
                           long minforcelimit,
                           long maxdistancelimit,
                           long mindistancelimit,

                           std::condition_variable *wait,
                           std::mutex *mutex,

                           ExperimentType type,
                           DistanceOrStressOrForce distanceOrStressForce,
                           long gagelength,
                           long zerodistance,
                           long currentdistance,
                           double area,

                           double stressForceLimit,
                           int cycles,
                           DistanceOrPercentage dp,
                           int calculateLimit,
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
               distanceOrStressForce,
               Direction::Stop,
               gagelength,
               zerodistance,
               currentdistance,
               area,
               300/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_DistanceOrStressOrForceLimit(distanceOrStressForce),
    m_LinearStageMessageHanders(linearstagemessagehandlers),
    m_DistanceOrPercentage(dp),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_Cycles(cycles),
    m_StressForceLimit(stressForceLimit * 1000),
    m_CalculateLimit(calculateLimit),
    m_DistanceLimit(0),
    m_SpeedInMm(speedInMM),
    m_Area(area * 0.000000000001/*um^2*/),
    //m_PreloadDistance(0),
    m_CurrentCycle(0),
    m_DecreaseSpeedFlag(false)
{
  // Calculate distance limit.
  if(Experiment::DistanceOrPercentage::Distance == m_DistanceOrPercentage){
    m_DistanceLimit = static_cast<long>(m_GageLength + m_CalculateLimit / 0.00009921875/*mm per micro step*/);
  } else if(Experiment::DistanceOrPercentage::Percentage == m_DistanceOrPercentage){
    m_DistanceLimit = ((m_CalculateLimit / 100.0) + 1.0) * m_GageLength;
  }

  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Sets the preload distance and updates the distance limit.
 * @param preloaddistance Preload distance
 */
void Conditioning::setPreloadDistance(long preloaddistance){
  m_GageLength = preloaddistance;

  // Calculate distance limit.
  if(Experiment::DistanceOrPercentage::Distance == m_DistanceOrPercentage){
    m_DistanceLimit = static_cast<long>(m_GageLength + m_CalculateLimit / 0.00009921875/*mm per micro step*/);
  } else if(Experiment::DistanceOrPercentage::Percentage == m_DistanceOrPercentage){
    m_DistanceLimit = ((m_CalculateLimit / 100.0) + 1.0) * m_GageLength;
  }
}

Conditioning::~Conditioning(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);

  std::cout << "Conditioning destructor finished." << std::endl;
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void Conditioning::getPreview(std::vector<PreviewValue> &previewvalue){

}

/**
 * @brief FSM of the conditioning experiment
 * @param event Occuring event
 */
void Conditioning::process(Experiment::Event event){
  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == event){
        m_StageFrame->setSpeed(m_SpeedInMm);
        m_CurrentState = runState;
        //m_ExperimentValues->setStartPoint();

        // If force/stress based
        if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForceLimit) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForceLimit)){
          // If force based
          if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
            if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMm);
            }else if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_SpeedInMm);
            }
          }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
            if((m_CurrentForce/m_Area - m_StressForceLimit) > m_ForceStressThreshold){
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_SpeedInMm);
            }else if((m_StressForceLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_SpeedInMm);
            }

          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForceLimit){ // If distance based
          if((m_CurrentDistance - m_DistanceLimit) > m_DistanceThreshold){
            std::cout << "m_CurrentDistance - m_DistanceLimit: " << m_CurrentDistance - m_DistanceLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_SpeedInMm);
            std::cout << "Conditioning moveForward" << std::endl;
          }else if((m_DistanceLimit - m_CurrentDistance) > m_DistanceThreshold){
            std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_DistanceLimit - m_CurrentDistance << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_SpeedInMm);
            std::cout << "Conditioning moveBackward" << std::endl;
          }
        }
      }

      break;

    case runState:

      if(Event::evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        m_StageFrame->stop();
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If stress/force based
        if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForceLimit) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForceLimit)){
          // If force based
          if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
            if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                m_StageFrame->moveBackward(m_SpeedInMm);
              }
            }else if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){ // Only reverse motor, if state changed
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
                m_CurrentDirection = Direction::Forwards;
                m_StageFrame->moveForward(m_SpeedInMm);
              }
            }else{
              m_CurrentState = goBackState;
              //m_StageFrame->stop();
              //std::cout << "Go to preload distance" << std::endl;
              m_StageFrame->gotoStepsDistance(m_GageLength);
            }
          }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
            if((m_CurrentForce/m_Area - m_StressForceLimit) > m_ForceStressThreshold){
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                m_StageFrame->moveBackward(m_SpeedInMm);
              }
            }else if((m_StressForceLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // Only reverse motor, if state changed
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
                m_CurrentDirection = Direction::Forwards;
                m_StageFrame->moveForward(m_SpeedInMm);
              }
            }else{
              m_CurrentState = goBackState;
              //m_StageFrame->stop();
              m_StageFrame->gotoStepsDistance(m_GageLength);
            }
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForceLimit){ // If distance based

          if((m_CurrentDistance - m_DistanceLimit) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              m_StageFrame->setSpeed(m_SpeedInMm/10);
            }
          }
          // Reduce speed to a tenth if stages are close to the turn point.
          else if((m_DistanceLimit - m_CurrentDistance) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              m_StageFrame->setSpeed(m_SpeedInMm/10);
            }
          }
          if((m_CurrentDistance - m_DistanceLimit) > m_DistanceThreshold){
            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward();
            }
          }else if((m_DistanceLimit - m_CurrentDistance) > m_DistanceThreshold){
            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward();
            }
          }else{
            m_CurrentState = goBackState;
            m_StageFrame->setSpeed(m_SpeedInMm);
            //m_StageFrame->stop();
            m_StageFrame->gotoStepsDistance(m_GageLength);
          }
        }
      }
      break;

    case goBackState:
      if(Event::evUpdate == event){
        //std::cout << "m_CurrentDistance - m_PreloadDistance > m_DistanceThreshold: " << m_CurrentDistance - m_PreloadDistance << "   " << m_DistanceThreshold << std::endl;
        //std::cout << "m_CurrentDistance: " << m_CurrentDistance << " m_PreloadDistance: " << m_PreloadDistance << std::endl;
        if(std::abs(m_GageLength - m_CurrentDistance) < m_DistanceThreshold){
          //std::cout << "diff < m_DistanceThreshold m_Cycles: " << m_Cycles - 1 << " m_CurrentCycle: " << m_CurrentCycle << std::endl;
          if((m_Cycles - 1) <= m_CurrentCycle){
            std::cout << "Stop conditioning." << std::endl;
            m_CurrentState = stopState;
            m_CurrentCycle = 0;
            m_CurrentDirection = Direction::Stop;
            //m_StageFrame->stop();
            std::lock_guard<std::mutex> lck(*m_WaitMutex);
            m_Wait->notify_all();
          }else{
            //std::cout << "Another cycle." << std::endl;
            m_CurrentCycle++;
            m_CurrentState = runState;

            // If force/stress based
            if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForceLimit) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForceLimit)){
              // If force based
              if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
                if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
                  //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
                  m_CurrentDirection = Direction::Forwards;
                  m_StageFrame->moveForward(m_SpeedInMm);
                }else if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){
                  //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
                  m_CurrentDirection = Direction::Backwards;
                  m_StageFrame->moveBackward(m_SpeedInMm);
                }
              }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
                if((m_CurrentForce/m_Area - m_StressForceLimit) > m_ForceStressThreshold){
                  //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
                  m_CurrentDirection = Direction::Forwards;
                  m_StageFrame->moveForward(m_SpeedInMm);
                }else if((m_StressForceLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
                  //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
                  m_CurrentDirection = Direction::Backwards;
                  m_StageFrame->moveBackward(m_SpeedInMm);
                }
              }
            }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForceLimit){ // If distance based
              if((m_CurrentDistance - m_DistanceLimit) > m_DistanceThreshold){
                m_DecreaseSpeedFlag = false;
                m_CurrentDirection = Direction::Forwards;
                m_StageFrame->moveForward(m_SpeedInMm);
              }else if((m_DistanceLimit - m_CurrentDistance) > m_DistanceThreshold){
                m_DecreaseSpeedFlag = false;
                m_CurrentDirection = Direction::Backwards;
                m_StageFrame->moveBackward(m_SpeedInMm);
              }
            }
          }
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
void Conditioning::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  /*
  std::thread t1(&Experiment::process, this, Experiment::Event::evUpdate);
  t1.detach();
  */
  process(Event::evUpdate);
}

/**
 * @brief Do all the required thing to stop the experiment during process.
 */
void Conditioning::resetExperiment(void){
  m_CurrentCycle = 0;
  m_CurrentState = stopState;
  m_CurrentDirection = Direction::Stop;
  m_StageFrame->stop();
}
