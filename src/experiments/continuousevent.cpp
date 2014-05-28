// Includes
#include <iostream>
#include <thread>
#include <chrono>
#include "continuousevent.h"

ContinuousEvent::ContinuousEvent(std::shared_ptr<StageFrame> stageframe,
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
                                 DistanceOrStressOrForce distanceOrStressForce,
                                 long gagelength,
                                 long zerodistance,
                                 long currentdistance,
                                 double area,

                                 DistanceOrPercentage velocityDistanceOrPercentage,
                                 double velocitypercent,
                                 double velocity,
                                 double holdtime,
                                 DistanceOrPercentage incrementDistanceOrPercentage,
                                 double incrementpercent,
                                 long increment,
                                 StepsOrMaxValue stepsOrMaxValue,
                                 DistanceOrPercentage maxvalueDistanceOrPercentage,
                                 double maxvaluepercent,
                                 long maxvaluelimit,
                                 int steps,
                                 int cycles,
                                 BehaviorAfterStop behaviorAfterStop)
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
                   0.01 * 10000.0/*stress force threshold*/,
                   0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
        m_Wait(wait),
        m_WaitMutex(mutex),

        m_VelocityDistanceOrPercentage(velocityDistanceOrPercentage),
        m_VelocityPercent(velocitypercent),
        m_Velocity(velocity),
        m_HoldTime(holdtime),
        m_IncrementDistanceOrPercentage(incrementDistanceOrPercentage),
        m_IncrementPercent(incrementpercent),
        m_Increment(increment),
        m_StepsOrMaxValue(stepsOrMaxValue),
        m_MaxValueDistanceOrPercentage(maxvalueDistanceOrPercentage),
        m_MaxValuePercent(maxvaluepercent),
        m_MaxValueLimit(maxvaluelimit),
        m_Steps(steps),
        m_Cycles(cycles),
        m_BehaviorAfterStop(behaviorAfterStop),
        m_CurrentState(State::stopState),
        m_CurrentLimit(0),
        m_CurrentStep(0),
        m_CurrentCycle(0),
        m_DecreaseSpeedFlag(false),
        m_CheckDistanceFlag(false),
        m_ExperimentValues(new ContinuousEventValues(stageframe,
                                                     forcesensormessagehandler,
                                                     vector,
                                                     vectoraccessmutex,
                                                     maxlimitvector,
                                                     minlimitvector,
                                                     myframe,
                                                     path,

                                                     type,
                                                     distanceOrStressForce,
                                                     area,

                                                     velocity,
                                                     holdtime,
                                                     cycles,
                                                     steps,
                                                     behaviorAfterStop))
{
  if(Experiment::DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_VelocityPercent / 100) * m_GageLength * 0.00009921875/*mm per micro step*/;
  }
  if(Experiment::DistanceOrPercentage::Percentage == m_IncrementDistanceOrPercentage){
    m_Increment = (m_IncrementPercent / 100) * m_GageLength;
  }
  if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
    if(Experiment::DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
      m_MaxValueLimit = (m_MaxValuePercent / 100) * m_GageLength;
    }
    if((0 < m_MaxValueLimit) && (0 < m_Increment)){
      m_Steps = m_MaxValueLimit / m_Increment;
    }
  }

  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
ContinuousEvent::~ContinuousEvent(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void ContinuousEvent::setPreloadDistance(){
  m_GageLength = m_CurrentDistance;

  if(Experiment::DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_VelocityPercent / 100) * m_GageLength * 0.00009921875/*mm per micro step*/;
  }
  if(Experiment::DistanceOrPercentage::Percentage == m_IncrementDistanceOrPercentage){
    m_Increment = (m_IncrementPercent / 100) * m_GageLength;
  }
  if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
    if(Experiment::DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
      m_MaxValueLimit = (m_MaxValuePercent / 100) * m_GageLength;
    }
    if((0 < m_MaxValueLimit) && (0 < m_Increment)){
      m_Steps = m_MaxValueLimit / m_Increment;
    }
  }
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void ContinuousEvent::getPreview(std::vector<Experiment::PreviewValue>& previewvalue){
  // Calculate time point.
  int timepoint;
  if(0 == previewvalue.size()){
   timepoint = 0;
  } else{
   timepoint = previewvalue.back().getTimepoint();
  }

  for(int i = 0; i < m_Cycles; ++i){
    // Make start point.
    previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_StartLength));
    timepoint++;
    for(int j = 0; j < m_Steps; ++j){
      // Make point if there is a hold time 1.
      if(0 < m_HoldTime){
        if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
          previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StartLength + (j * m_Increment)));
        } else{
          previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, j * m_Increment));
        }
        timepoint++;
      }
      if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StartLength + ((j + 1) * m_Increment)));
      } else{
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, (j + 1) * m_Increment));
        //std::cout << "ContinuousEvent: (j + 1) * m_Increment: " << (j + 1) * m_Increment << ", with m_Increment: " << m_Increment << std::endl;
      }
      timepoint++;
    }
  }
}


/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void ContinuousEvent::process(Event event){

  switch(m_CurrentState){
    case stopState:
      if(Experiment::Event::evStart == event){

        std::cout << "ContinuousEvent: Start experiment." << std::endl;

        // Perform hold if there is a hold time
        if(0 < m_HoldTime){
          std::cout << "ContinuousEvent holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
          std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
          t1.join();
          //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
          std::cout << "ContinuousEvent holding over." << std::endl;
        }

        // Set current limit.
        if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
          m_CurrentLimit = m_StartLength + m_Increment;
        } else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
          m_CurrentLimit = m_CurrentForce/m_Area + m_Increment;
        } else if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          m_CurrentLimit = m_CurrentForce + m_Increment;
          std::cout << "ContinuousEvent: m_CurrentLimit: " << m_CurrentLimit << ", with m_CurrentForce: " << m_CurrentForce << " and m_Increment: " << m_Increment << std::endl;
        }

        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->setSpeed(m_Velocity);
        }
        m_CurrentState = runState;
        m_CheckLimitsFlag = true;
        //m_ExperimentValues->setStartPoint();

        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
          }
        }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
          if((m_CurrentForce/m_Area - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based
          if((m_CurrentDistance) - m_CurrentLimit > m_DistanceThreshold){
            //std::cout << "m_CurrentDistance - m_DistanceLimit: " << (m_CurrentDistance) - m_CurrentLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
            std::cout << "ContinuousEvent: Move forward" << std::endl;
          }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
            //std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_CurrentLimit - (m_CurrentDistance) << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
            std::cout << "ContinuousEvent: Move backward" << std::endl;
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
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        std::cout << "OneStepEvent: Stop." << std::endl;
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If force based
        if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
          //std::cout << "m_CurrentForce: " << m_CurrentForce << " m_CurrentLimit: " <<  m_CurrentLimit << std::endl;
          if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout << "(m_CurrentForce - m_CurrentLimit) >  m_ForceStressThreshold: " << (m_CurrentForce - m_CurrentLimit) << " " << m_ForceStressThreshold << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveBackward(m_Velocity);
              }
            }
          }else if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "(m_CurrentLimit - m_CurrentForce) >  m_ForceStressThreshold: " << (m_CurrentLimit - m_CurrentForce) << " " << m_ForceStressThreshold << std::endl;

            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only reverse motor, if state changed
              m_CurrentDirection = Direction::Forwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveForward(m_Velocity);
              }
            }
          }else{
            if((m_Steps - 1) <= m_CurrentStep){ // If it is the last step.
              // Reset step counter.
              m_CurrentStep = 0;
              // Reset limit
              m_CurrentLimit = 0;
              //std::cout << "Go to preload distance" << std::endl;
              if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

                switch(m_BehaviorAfterStop){
                  case BehaviorAfterStop::GoToL0:
                    m_CurrentState = goBackState;
                    m_CheckDistanceFlag = true;
                    m_CurrentStep = 0;
                    m_CurrentCycle = 0;
                    m_CurrentLimit = m_GageLength;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->gotoStepsDistance(m_GageLength);
                    }
                    break;
                  case BehaviorAfterStop::Stop:
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    std::cout << "ContinuousEvent: Stop." << std::endl;
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CheckDistanceFlag = false;
                    m_CurrentStep = 0;
                    m_CurrentCycle = 0;
                    std::lock_guard<std::mutex> lck(*m_WaitMutex);
                    m_Wait->notify_all();
                    break;
                }
                std::cout << "ContinuousEvent: Got to end length." << std::endl;
                //process(Event::evUpdate);
              } else{
                  m_CurrentCycle++;
                  m_CheckDistanceFlag = true;
                  m_CurrentState = goStartState;

                  // Perform hold if there is a hold time
                  if(0 < m_HoldTime){
                    m_CurrentDirection = Direction::Stop;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                    std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                    t1.join();
                    //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                    std::cout << "ContinuousEvent: Holding over." << std::endl;
                  }
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_StartLength);
                  }
                  std::cout << "ContinuousEvent: Go to start length." << std::endl;
                }
              } else{
                m_CurrentStep++;
                // Update current limit.
                m_CurrentLimit += m_Increment;
                std::cout << "ContinuousEvent: m_CurrentLimit: " << m_CurrentLimit << ", with m_CurrentForce: " << m_CurrentForce << " and m_Increment: " << m_Increment << std::endl;

                // Perform hold if there is a hold time
                if(0 < m_HoldTime){
                  m_CurrentDirection = Direction::Stop;
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->stop();
                  }
                  std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                  std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                  t1.join();
                  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "ContinuousEvent: Holding over." << std::endl;
                }
              }
            }
          }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){ // If stress based
            if((m_CurrentForce/m_Area - m_CurrentLimit) > m_ForceStressThreshold){
              //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;

              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->moveBackward(m_Velocity);
                }
              }
            }else if((m_CurrentLimit - m_CurrentForce/m_Area) > m_ForceStressThreshold){
              //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;

              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only reverse motor, if state changed
                m_CurrentDirection = Direction::Forwards;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->moveForward(m_Velocity);
                }
              }
            }else{
              if((m_Steps - 1) <= m_CurrentStep){ // If it is the last step.
                // Reset step counter.
                m_CurrentStep = 0;
                // Reset limit
                m_CurrentLimit = 0;
                if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

                  switch(m_BehaviorAfterStop){
                    case BehaviorAfterStop::GoToL0:
                      m_CurrentState = goBackState;
                      m_CheckDistanceFlag = true;
                      m_CurrentStep = 0;
                      m_CurrentCycle = 0;
                      m_CurrentLimit = m_GageLength;
                      m_StageFrame->gotoStepsDistance(m_GageLength);
                      break;
                    case BehaviorAfterStop::Stop:
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->stop();
                      }
                      std::cout << "ContinuousEvent: Stop." << std::endl;
                      m_CurrentState = stopState;
                      m_CurrentDirection = Direction::Stop;
                      m_CheckDistanceFlag = false;
                      m_CurrentStep = 0;
                      m_CurrentCycle = 0;
                      std::lock_guard<std::mutex> lck(*m_WaitMutex);
                      m_Wait->notify_all();
                      break;
                  }
                  std::cout << "ContinuousEvent: Got to end length." << std::endl;
                  //process(Event::evUpdate);
                } else{
                  m_CurrentCycle++;
                  m_CheckDistanceFlag = true;
                  m_CurrentState = goStartState;

                  // Perform hold if there is a hold time
                  if(0 < m_HoldTime){
                    m_CurrentDirection = Direction::Stop;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                    std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                    t1.join();
                    //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                    std::cout << "ContinuousEvent: Holding over." << std::endl;
                  }
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_StartLength);
                  }
                  std::cout << "ContinuousEvent: Go to start length." << std::endl;
                }
              } else{
                m_CurrentStep++;
                // Update current limit.
                m_CurrentLimit += m_Increment;

                // Perform hold if there is a hold time
                if(0 < m_HoldTime){
                  m_CurrentDirection = Direction::Stop;
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->stop();
                  }
                  std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                  std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                  t1.join();
                  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "ContinuousEvent: Holding over." << std::endl;
                }
              }
            }
          }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based

            if(((m_CurrentDistance) - m_CurrentLimit) < (200 * m_DistanceThreshold)){
              if(false == m_DecreaseSpeedFlag){
                m_DecreaseSpeedFlag = true;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->setSpeed(m_Velocity/10);
                }
              }
            }
            // Reduce speed to a tenth if stages are close to the turn point.
            else if((m_CurrentLimit - (m_CurrentDistance)) < (200 * m_DistanceThreshold)){
              if(false == m_DecreaseSpeedFlag){
                m_DecreaseSpeedFlag = true;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->setSpeed(m_Velocity/10);
                }
              }
            }
            //std::cout << "m_CurrentDistance : " << m_CurrentDistance << " m_CurrentLimit: " << (m_CurrentLimit) << std::endl;
            if(((m_CurrentDistance) - m_CurrentLimit) > m_DistanceThreshold){
              if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Forwards;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->moveForward();
                }
                std::cout << "OneStepEvent moveForward." << std::endl;
              }
            }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->moveBackward();
                }
                std::cout << "OneStepEvent moveBackward." << std::endl;
              }
            }else{
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->setSpeed(m_Velocity);
              }
              if((m_Steps - 1) <= m_CurrentStep){ // If it is the last step.
                // Reset step counter.
                m_CurrentStep = 0;
                // Reset limit
                m_CurrentLimit = 0;
                if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

                  switch(m_BehaviorAfterStop){
                    case BehaviorAfterStop::GoToL0:
                      m_CurrentState = goBackState;
                      m_CheckDistanceFlag = true;
                      m_CurrentStep = 0;
                      m_CurrentCycle = 0;
                      m_CurrentLimit = m_GageLength;
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->gotoStepsDistance(m_GageLength);
                      }
                      break;

                    case BehaviorAfterStop::Stop:
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->stop();
                      }
                      std::cout << "ContinuousEvent: Stop." << std::endl;
                      m_CurrentState = stopState;
                      m_CurrentDirection = Direction::Stop;
                      m_CheckDistanceFlag = false;
                      m_CurrentStep = 0;
                      m_CurrentCycle = 0;
                      std::lock_guard<std::mutex> lck(*m_WaitMutex);
                      m_Wait->notify_all();
                      break;
                  }
                  std::cout << "ContinuousEvent: Got to end length." << std::endl;
                  //process(Event::evUpdate);
                } else{
                  m_CurrentCycle++;
                  m_CheckDistanceFlag = true;
                  m_CurrentState = goStartState;

                  // Perform hold if there is a hold time
                  if(0 < m_HoldTime){
                    m_CurrentDirection = Direction::Stop;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    //std::cout << "ContinuousEvent: stages should stop." << std::endl;
                    std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                    std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime); t1.join();
                    //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                    std::cout << "ContinuousEvent: Holding over." << std::endl;
                  }
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_StartLength);
                  }
                  std::cout << "ContinuousEvent: Go to start length." << std::endl;
                }
              } else{
                m_CurrentStep++;
                //std::cout << "ContinuousEvent: current cycle: " << m_CurrentStep << ", total steps: " << m_Steps << std::endl;
                // Update current limit.
                m_CurrentLimit += m_Increment;

                // Perform hold if there is a hold time
                if(0 < m_HoldTime){
                  m_CurrentDirection = Direction::Stop;
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->stop();
                  }
                  std::cout << "ContinuousEvent: Holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
                  std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                  t1.join();
                  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  std::cout << "ContinuousEvent: Holding over." << std::endl;
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
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        std::cout << "ContinuousEvent: Stop." << std::endl;
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < 0.5*m_DistanceThreshold){
          std::cout << "ContinuousEvent: goStartState: Start distance reached." << std::endl;

          // Set current limit.
          if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
            m_CurrentLimit = m_StartLength + m_Increment;
          } else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
            m_CurrentLimit = m_CurrentForce/m_Area + m_Increment;
          } else if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
            m_CurrentLimit = m_CurrentForce + m_Increment;
          }

          m_CheckDistanceFlag = false;
          m_CurrentState = runState;

          // Perform hold if there is a hold time
          if(0 < m_HoldTime){
            std::cout << "ContinuousEvent holds for hold time: " << m_HoldTime * 1000 << " ms" << std::endl;
            std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
            t1.join();
            //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
            std::cout << "ContinuousEvent holding over." << std::endl;
          }

          //m_CurrentDirection = Direction::Stop;
          std::cout << "ContinuousEvent:: Go to runState" << std::endl;
          process(Event::evUpdate);
          //m_CurrentDirection = Direction::Stop;
          //m_StageFrame->stop();
        }/*else{
          if((m_CurrentDistance - m_StartLength) > m_DistanceThreshold){
            std::cout << "goStartState: (m_CurrentDistance - m_StartLength) > m_DistanceThreshold" << (m_CurrentDistance - m_StartLength) << " > " << m_DistanceThreshold << std::endl;
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
          }else if((m_StartLength - m_CurrentDistance) > m_DistanceThreshold){
            std::cout << "goStartState: (m_StartLength - m_CurrentDistance) > m_DistanceThreshold" << (m_StartLength - m_CurrentDistance) << " > " << m_DistanceThreshold << std::endl;
            m_DecreaseSpeedFlag = false;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }
        }
        */
      }
      break;

    case goBackState:
      if(Event::evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        std::cout << "ContinuousEvent: Stop." << std::endl;
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < 0.7*m_DistanceThreshold){
          std::cout << "Stop OneStepEvent." << std::endl;
          m_CheckDistanceFlag = false;
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CurrentStep = 0;
          m_CurrentCycle = 0;
          //m_StageFrame->stop();
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }/*else{
          if((m_CurrentDistance - m_CurrentLimit) > m_DistanceThreshold){
            m_DecreaseSpeedFlag = false;
              m_CurrentDirection = Direction::Forwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveForward(m_Velocity);
              }
            }else if((m_CurrentLimit - m_CurrentDistance) > m_DistanceThreshold){
              m_DecreaseSpeedFlag = false;
              m_CurrentDirection = Direction::Backwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveBackward(m_Velocity);
              }
            }
          }
        */
      }
      break;
  }
}


/**
 * @brief Do all the required thing to stop the experiment during process.
 */
void ContinuousEvent::resetExperiment(void){
  m_CurrentCycle = 0;
  m_CurrentState = stopState;
  m_CheckLimitsFlag = false;
  m_CurrentDirection = Direction::Stop;
  m_StageFrame->stop();
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void ContinuousEvent::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      if((true == m_CheckLimitsFlag) && ((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce))){
        std::cout << "OneStepEvent: Force limit exceeded." << std::endl;
        process(Event::evStop);
      } else{
        if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
          process(Event::evUpdate);
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      if((true == m_CheckLimitsFlag) && (m_MaxDistanceLimit < m_CurrentDistance) || (m_MinDistanceLimit > m_CurrentDistance)){
        std::cout << "OneStepEvent: Distance limit exceeded." << std::endl;
        process(Event::evStop);
      } else{
        if((DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce) || (true == m_CheckDistanceFlag)){
          //std::cout << "ContinuousEvent: update." << std::endl;
          process(Event::evUpdate);
        }
      }
      break;
  }
}

/**
 * @brief Sleep for the amount of milliseconds.
 * @param seconds
 */
void ContinuousEvent::sleepForMilliseconds(double seconds){
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
}
