// Includes
#include <iostream>
#include <thread>
#include "onestepevent.h"

OneStepEvent::OneStepEvent(std::shared_ptr<StageFrame> stageframe,
                           std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                           mpFXYVector *vector,
                           std::mutex *vectoraccessmutex,
                           MyFrame *myframe,
                           std::string path,

                           std::condition_variable *wait,
                           std::mutex *mutex,
                           bool *stagesstopped,
                           std::mutex *stagesstoppedmutex,

                           ExperimentType type,
                           DistanceOrStressOrForce distanceOrStressForce,
                           long gagelength,
                           long zerodistance,
                           long currentdistance,
                           double area,

                           DistanceOrPercentage velocityDistanceOrPercentage,
                           double velocity,
                           double holdtime1,
                           long upperlimit,
                           double holdtime2,
                           long lowerlimit,
                           DistanceOrPercentage holdDistanceOrPercentage,
                           long holddistance,
                           int cycles,
                           BehaviorAfterStop behaviorAfterStop)
  : Experiment(stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,

               type,
               distanceOrStressForce,
               Direction::Stop,
               gagelength,
               zerodistance,
               currentdistance,
               area,
               0.3/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_Wait(wait),
    m_WaitMutex(mutex),

    m_VelocityDistanceOrPercentage(velocityDistanceOrPercentage),
    m_Velocity(velocity),
    m_HoldTime1(holdtime1),
    m_UpperLimit(upperlimit),
    m_HoldTime2(holdtime2),
    m_LowerLimit(lowerlimit),
    m_Cycles(cycles),
    m_HoldDistanceOrPercentage(holdDistanceOrPercentage),
    m_HoldDistance(holddistance),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_CurrentState(State::stopState),
    m_CurrentLimit(0),
    m_CurrentCycle(0),
    m_DecreaseSpeedFlag(false),
    m_ExperimentValues(new OneStepEventValues(stageframe,
                                              forcesensormessagehandler,
                                              vector,
                                              vectoraccessmutex,
                                              myframe,
                                              path,

                                              type,
                                              distanceOrStressForce,
                                              area,

                                              velocity,
                                              holdtime1,
                                              upperlimit,
                                              holdtime2,
                                              lowerlimit,
                                              holddistance,
                                              cycles,
                                              behaviorAfterStop))
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

OneStepEvent::~OneStepEvent(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void OneStepEvent::setPreloadDistance(long preloaddistance){
  m_GageLength = preloaddistance;

  if(DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_Velocity / 100.0) * m_GageLength;
    m_ExperimentValues->setVelocity(m_Velocity);
  }
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void OneStepEvent::getPreview(std::vector<Experiment::PreviewValue>& previewvalue){
  // Calculate time point.
  int timepoint;
  if(0 == previewvalue.size()){
   timepoint = 1;
  } else{
   timepoint = previewvalue.back().getTimepoint() + 1;
  }

  for(int i = 0; i < m_Cycles; ++i){
    // Make point if there is a hold time 1.
    if(0 < m_HoldTime1){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, 0));
      timepoint++;
    }
    // Make upper limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
    timepoint++;
    // Make point if there is a hold time 2.
    if(0 < m_HoldTime2){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
      timepoint++;
    }
    // Make lower limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_LowerLimit));
  }
  timepoint++;

  // Make last point depending on the stop behavior.
  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::GoToL0:
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
        break;
    case BehaviorAfterStop::HoldADistance:
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_HoldDistance));
        break;
  }
}

/**
* @brief FSM of the experiment
* @param e Occuring event
*/
void OneStepEvent::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == event){

        // Perform hold if there is a hold time 1
        if(0 < m_HoldTime1){
          std::cout << "OneStepEvent holds for hold time 1: " << m_HoldTime1 * 1000 << " ms" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
          std::cout << "OneStepEvent holding over." << std::endl;
        }

        m_CurrentLimit = m_UpperLimit;
        m_StageFrame->setSpeed(m_Velocity);
        m_CurrentState = runState;
        //m_ExperimentValues->setStartPoint();

        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_Velocity);
          }else if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_Velocity);
          }
        }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_Velocity);
          }else if((m_CurrentLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_Velocity);
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based
          if((m_CurrentDistance) - m_CurrentLimit > m_DistanceThreshold){
            std::cout << "m_CurrentDistance - m_DistanceLimit: " << (m_CurrentDistance) - m_CurrentLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_Velocity);
            std::cout << "OneStepEvent moveForward" << std::endl;
          }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
            std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_CurrentLimit - (m_CurrentDistance) << std::endl;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_Velocity);
            std::cout << "OneStepEvent moveBackward" << std::endl;
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
        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          std::cout << "m_CurrentForce: " << m_CurrentForce << " m_CurrentLimit: " <<  m_CurrentLimit << std::endl;
          if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
            std::cout << "m_CurrentForce - m_CurrentLimit: " << m_CurrentForce - m_CurrentLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            std::cout << "m_CurrentLimit - m_CurrentForce: " << m_CurrentLimit - m_CurrentForce << std::endl;

            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_Velocity);
            }
          }else{
            //m_StageFrame->stop();
            //std::cout << "Go to preload distance" << std::endl;
            if(m_CurrentLimit == m_UpperLimit){
              m_CurrentLimit = m_LowerLimit;

              // Perform hold if there is a hold time 2
              if(0 < m_HoldTime2){
                std::cout << "OneStepEvent holds for hold time 2" << m_HoldTime2 * 1000 << " ms" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime2 * 1000)));
                std::cout << "OneStepEvent holding over." << std::endl;
              }
              process(Event::evUpdate);
            }else if(m_CurrentLimit == m_LowerLimit){
              if((m_Cycles - 1) <= m_CurrentCycle){

                switch(m_BehaviorAfterStop){
                  case BehaviorAfterStop::GoToL0:
                    m_CurrentLimit = m_GageLength;
                    break;
                  case BehaviorAfterStop::HoldADistance:
                    m_CurrentLimit = m_HoldDistance;
                    break;
                }

                m_CurrentState = goBackState;
                m_CurrentCycle = 0;
                process(Event::evUpdate);
              } else{
                m_CurrentCycle++;
                m_CurrentState = goStartState;

                // Perform hold if there is a hold time 1
                if(0 < m_HoldTime1){
                  std::cout << "OneStepEvent holds for hold time 1: " << m_HoldTime1 * 1000 << " ms" << std::endl;
                  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "OneStepEvent holding over." << std::endl;
                }

                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
            }
          }
        }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

          if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_Velocity);
            }
          }else{
            //m_CurrentState = goBackState;
            //m_StageFrame->stop();
            if(m_CurrentLimit == m_UpperLimit){
              m_CurrentLimit = m_LowerLimit;

              // Perform hold if there is a hold time 2
              if(0 < m_HoldTime2){
                std::cout << "OneStepEvent holds for hold time 2 " << m_HoldTime2 * 1000 << " ms" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime2 * 1000)));
                std::cout << "OneStepEvent holding over." << std::endl;
              }
              process(Event::evUpdate);
            }else if(m_CurrentLimit == m_LowerLimit){
              if((m_Cycles - 1) <= m_CurrentCycle){

                switch(m_BehaviorAfterStop){
                  case BehaviorAfterStop::GoToL0:
                    m_CurrentLimit = m_GageLength;
                    break;
                  case BehaviorAfterStop::HoldADistance:
                    m_CurrentLimit = m_HoldDistance;
                    break;
                }

                m_CurrentState = goBackState;
                m_CurrentCycle = 0;
                process(Event::evUpdate);
              } else {
                m_CurrentCycle++;
                m_CurrentState = goStartState;

                // Perform hold if there is a hold time 1
                if(0 < m_HoldTime1){
                  std::cout << "OneStepEvent holds for hold time 1: " << m_HoldTime1 * 1000 << " ms" << std::endl;
                  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "OneStepEvent holding over." << std::endl;
                }
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
            }
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based

          if(((m_CurrentDistance) - m_CurrentLimit) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              m_StageFrame->setSpeed(m_Velocity/10);
            }
          }
          // Reduce speed to a tenth if stages are close to the turn point.
          else if((m_CurrentLimit - (m_CurrentDistance)) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              m_StageFrame->setSpeed(m_Velocity/10);
            }
          }
          //std::cout << "m_CurrentDistance : " << m_CurrentDistance << " m_CurrentLimit: " << (m_CurrentLimit) << std::endl;
          if(((m_CurrentDistance) - m_CurrentLimit) > m_DistanceThreshold){
            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward();
            }
          }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward();
            }
          }else{
            //m_CurrentState = goBackState;
            m_StageFrame->setSpeed(m_Velocity);
            //m_StageFrame->stop();
            if(m_CurrentLimit == m_UpperLimit){
              m_CurrentLimit = m_LowerLimit;

              // Perform hold if there is a hold time 2
              if(0 < m_HoldTime2){
                std::cout << "OneStepEvent holds for hold time 2 " << m_HoldTime2 * 1000 << " ms" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime2 * 1000)));
                std::cout << "OneStepEvent holding over." << std::endl;
              }
              process(Event::evUpdate);
            }else if(m_CurrentLimit == m_LowerLimit){
              if((m_Cycles - 1) <= m_CurrentCycle){

                switch(m_BehaviorAfterStop){
                  case BehaviorAfterStop::GoToL0:
                    m_CurrentLimit = m_GageLength;
                    break;
                  case BehaviorAfterStop::HoldADistance:
                    m_CurrentLimit = m_HoldDistance;
                    break;
                }

                m_CurrentState = goBackState;
                m_CurrentCycle = 0;
                process(Event::evUpdate);
              } else {
                m_CurrentCycle++;
                m_CurrentState = goStartState;

                // Perform hold if there is a hold time 1
                if(0 < m_HoldTime1){
                  std::cout << "OneStepEvent holds for hold time 1: " << m_HoldTime1 * 1000 << " ms" << std::endl;
                  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "OneStepEvent holding over." << std::endl;
                }
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
            }
          }
        }
      }
      break;

    case goStartState:
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
        //std::cout << "m_CurrentDistance - m_PreloadDistance > m_DistanceThreshold: " << m_CurrentDistance - m_PreloadDistance << "   " << m_DistanceThreshold << std::endl;
        //std::cout << "m_CurrentDistance: " << m_CurrentDistance << " m_PreloadDistance: " << m_PreloadDistance << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold){
          //std::cout << "diff < m_DistanceThreshold m_Cycles: " << m_Cycles - 1 << " m_CurrentCycle: " << m_CurrentCycle << std::endl;
          m_CurrentLimit = m_UpperLimit;
          m_CurrentState = runState;
            //m_CurrentDirection = Direction::Stop;
            //m_StageFrame->stop();
        }else{
          if((m_CurrentDistance - m_StartLength) > m_DistanceThreshold){
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_Velocity);
          }else if((m_StartLength - m_CurrentDistance) > m_DistanceThreshold){
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_Velocity);
          }
        }
      }
    break;

    case goBackState:
      if(Event::evUpdate == event){
        //std::cout << "m_CurrentDistance - m_PreloadDistance > m_DistanceThreshold: " << m_CurrentDistance - m_PreloadDistance << "   " << m_DistanceThreshold << std::endl;
        //std::cout << "m_CurrentDistance: " << m_CurrentDistance << " m_PreloadDistance: " << m_PreloadDistance << std::endl;
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < m_DistanceThreshold){
          //std::cout << "diff < m_DistanceThreshold m_Cycles: " << m_Cycles - 1 << " m_CurrentCycle: " << m_CurrentCycle << std::endl;
          std::cout << "Stop OneStepEvent." << std::endl;
          m_CurrentState = stopState;
          m_CurrentCycle = 0;
          m_CurrentDirection = Direction::Stop;
          //m_StageFrame->stop();
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }else{
          if((m_CurrentDistance - m_CurrentLimit) > m_DistanceThreshold){
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Forwards;
            m_StageFrame->moveForward(m_Velocity);
          }else if((m_CurrentLimit - m_CurrentDistance) > m_DistanceThreshold){
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Backwards;
            m_StageFrame->moveBackward(m_Velocity);
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
void OneStepEvent::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      break;
  }

  process(Event::evUpdate);
}

/**
* @brief Do all the required thing to stop the experiment during process.
*/
void OneStepEvent::resetExperiment(void){
  m_CurrentCycle = 0;
  m_CurrentState = stopState;
  m_CurrentDirection = Direction::Stop;
  m_StageFrame->stop();
}
