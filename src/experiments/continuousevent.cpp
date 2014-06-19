// Includes
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <wx/log.h>
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
                                 bool ramptofailureactiveflag,
                                 long gagelength,
                                 long mountinglength,
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
                                 double ramptofailurepercent,
                                 int cycles,
                                 BehaviorAfterStop behaviorAfterStop,
                                 long holdforce)
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
                   mountinglength,
                   zerodistance,
                   currentdistance,
                   area,
                   0.01 * 10000.0/*stress force threshold*/,
                   0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
        m_Wait(wait),
        m_WaitMutex(mutex),

        m_Ramp2FailureActiveFlag(ramptofailureactiveflag),
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
        m_InitRelMaxValueLimit(maxvaluelimit),
        m_Steps(steps),
        m_Ramp2FailurePercentage(ramptofailurepercent),
        m_Cycles(cycles),
        m_BehaviorAfterStop(behaviorAfterStop),
        m_CurrentState(State::stopState),
        m_CurrentLimit(0),
        m_CurrentStep(0),
        m_CurrentCycle(0),
        m_MaxStressForce(0),
        m_HoldForce(holdforce),
        m_WaitActive(false),
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
                                                     steps,
                                                     cycles,
                                                     behaviorAfterStop))
{
  initParameters();

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
 * @brief Initializes the parameters.
 */
void ContinuousEvent::initParameters(void){
  if(Experiment::DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_VelocityPercent / 100) * m_GageLength * 0.00009921875/*mm per micro step*/;
    m_ExperimentValues->setVelocity(m_Velocity);
  }

  // Only set increment and steps parameter if experiment is not a ramp to failure experiment.
  if(false == m_Ramp2FailureActiveFlag){
    if(Experiment::DistanceOrPercentage::Percentage == m_IncrementDistanceOrPercentage){
      m_Increment = (m_IncrementPercent / 100) * m_GageLength;
      m_ExperimentValues->setIncrement(m_Increment);
    }
    if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
      if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
        if(Experiment::DistanceOrPercentage::DistanceRelative == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_StartLength + m_InitRelMaxValueLimit;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }else if(Experiment::DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = (1 + (m_MaxValuePercent / 100)) * m_GageLength;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }
      }
      if((0 != m_MaxValueLimit) && (0 != m_Increment)){
        if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
          m_Steps = (m_MaxValueLimit - m_CurrentForce) / m_Increment;
          wxLogMessage(std::string("ContinuousEvent: m_Steps: " + std::to_string(m_Steps) +
                                   " m_MaxValueLimit: " + std::to_string(m_MaxValueLimit) +
                                   " m_CurrentForce: " + std::to_string(m_CurrentForce) +
                                   " m_Increment: " + std::to_string(m_Increment)).c_str());
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
          m_Steps = (m_MaxValueLimit - m_CurrentDistance) / m_Increment;
        }
        m_ExperimentValues->setSteps(m_Steps);
      }
    }
  } else if(true == m_Ramp2FailureActiveFlag){
    m_Increment = 0;
    m_ExperimentValues->setIncrement(m_Increment);
    m_Steps = 1;
    m_ExperimentValues->setSteps(m_Steps);
  }
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void ContinuousEvent::setPreloadDistance(){
  m_GageLength = m_CurrentDistance;

  initParameters();
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
      // Make point if there is a hold time.
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

      // Make last point depending on the stop behavior.
      switch(m_BehaviorAfterStop){
        case BehaviorAfterStop::Stop:
          if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
            previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StartLength + ((j + 1) * m_Increment)));
          } else{
            previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, (j + 1) * m_Increment));
            //std::cout << "ContinuousEvent: (j + 1) * m_Increment: " << (j + 1) * m_Increment << ", with m_Increment: " << m_Increment << std::endl;
          }
          break;
        case BehaviorAfterStop::GoToL0:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_GageLength));
          break;
        case BehaviorAfterStop::GoToML:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_MountingLength));
          break;
        case BehaviorAfterStop::HoldAForce:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Force, m_HoldForce));
          break;
      }
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

        wxLogMessage("ContinuousEvent: Start experiment.");

        // Perform hold if there is a hold time
        if(0 < m_HoldTime){
          wxLogMessage(std::string("ContinuousEvent: Hold for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

          /*
          {
            std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
            m_WaitActive = true;
          }
          */
          std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
          t1.join();
          /*
          {
            std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
            m_WaitActive = false;
          }
          */

          //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
          wxLogMessage("ContinuousEvent: Holding over.");
        }

        // Set current limit.
        if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
          m_CurrentLimit = m_StartLength + m_Increment;
        } else if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
          m_CurrentLimit = m_CurrentForce + m_Increment;
        }

        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->setSpeed(m_Velocity);
        }
        m_CurrentState = runState;
        m_CheckLimitsFlag = true;
        //m_ExperimentValues->setStartPoint();

        // If ramp2failure
        if(true == m_Ramp2FailureActiveFlag){
          m_CurrentDirection = Direction::Backwards;
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->moveBackward(m_Velocity);
        }
        // If force/stress based
        else if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
          if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
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
            //std::cout << "ContinuousEvent: Move forward" << std::endl;
          }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
            //std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_CurrentLimit - (m_CurrentDistance) << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
            //std::cout << "ContinuousEvent: Move backward" << std::endl;
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
        wxLogMessage("ContinuousEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If ramp2failure
        if(true == m_Ramp2FailureActiveFlag){
          if(std::abs(m_CurrentForce) < ((m_Ramp2FailurePercentage / 100.0) * std::abs(m_MaxStressForce))){
            if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

              m_CurrentStep = 0;
              m_CurrentCycle = 0;

              switch(m_BehaviorAfterStop){
                case BehaviorAfterStop::GoToL0:
                  m_CurrentState = goBackState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_GageLength;
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_GageLength);
                  }
                  break;
                case BehaviorAfterStop::GoToML:
                  m_CurrentState = goBackState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_MountingLength;
                  wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength)).c_str());
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_MountingLength);
                  }
                  break;
                case BehaviorAfterStop::Stop:
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->stop();
                  }
                  wxLogMessage("ContinuousEvent: Stop.");
                  m_CurrentState = stopState;
                  m_CurrentDirection = Direction::Stop;
                  m_CheckDistanceFlag = false;
                  {
                    std::lock_guard<std::mutex> lck(*m_WaitMutex);
                    m_Wait->notify_all();
                  }
                  break;
                case BehaviorAfterStop::HoldAForce:
                  m_CurrentState = goBackForceState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_HoldForce;
                  wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_HoldForce / 10000.0)).c_str());

                  if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
                    //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
                    m_CurrentDirection = Direction::Backwards;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->moveBackward(m_Velocity);
                    }
                  }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
                    //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
                    m_CurrentDirection = Direction::Forwards;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->moveForward(m_Velocity);
                    }
                  }
                  break;
              }
              wxLogMessage("ContinuousEvent: Went to end length.");
              //process(Event::evUpdate);
            }else{
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
                wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

                /*
                {
                  std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                  m_WaitActive = true;
                }
                */
                std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                t1.join();
                /*
                {
                  std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                  m_WaitActive = false;
                }
                */

                //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                wxLogMessage("ContinuousEvent: Holding over.");
              }
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
              wxLogMessage("ContinuousEvent: Go to start length.");
            }
          }
        }
        // If force/stress based
        else if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
          //std::cout << "m_CurrentForce: " << m_CurrentForce << " m_CurrentLimit: " <<  m_CurrentLimit << std::endl;
          if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "(m_CurrentForce - m_CurrentLimit) >  m_ForceStressThreshold: " << (m_CurrentForce - m_CurrentLimit) << " " << m_ForceStressThreshold << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveBackward(m_Velocity);
              }
            }
          }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
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

                m_CurrentStep = 0;
                m_CurrentCycle = 0;

                switch(m_BehaviorAfterStop){
                  case BehaviorAfterStop::GoToL0:
                    m_CurrentState = goBackState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_GageLength;
                    wxLogMessage(std::string("ContinuousEvent: Go to gage length: " + std::to_string(m_GageLength)).c_str());
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->gotoStepsDistance(m_GageLength);
                    }
                    break;
                  case BehaviorAfterStop::GoToML:
                    m_CurrentState = goBackState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_MountingLength;
                    wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength)).c_str());
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->gotoStepsDistance(m_MountingLength);
                    }
                    break;
                  case BehaviorAfterStop::Stop:
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    wxLogMessage("ContinuousEvent: Stop.");
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CheckDistanceFlag = false;
                    {
                      std::lock_guard<std::mutex> lck(*m_WaitMutex);
                      m_Wait->notify_all();
                    }
                    break;
                  case BehaviorAfterStop::HoldAForce:
                    m_CurrentState = goBackForceState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_HoldForce;
                    wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_HoldForce / 10000.0)).c_str());

                    if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
                      //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
                      m_CurrentDirection = Direction::Backwards;
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->moveBackward(m_Velocity);
                      }
                    }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
                      //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
                      m_CurrentDirection = Direction::Forwards;
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->moveForward(m_Velocity);
                      }
                    }
                    break;
                }
                wxLogMessage("ContinuousEvent: Went to end length.");
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
                    wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

                    /*
                    {
                      std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                      m_WaitActive = true;
                    }
                    */
                    std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                    t1.join();
                    /*
                    {
                      std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                      m_WaitActive = false;
                    }
                    */

                    //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                    wxLogMessage("ContinuousEvent: Holding over.");
                  }
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_StartLength);
                  }
                  wxLogMessage("ContinuousEvent: Go to start length.");
                }
              } else{
                m_CurrentStep++;
                // Update current limit.
                m_CurrentLimit += m_Increment;
                //std::cout << "ContinuousEvent: m_CurrentLimit: " << m_CurrentLimit << ", with m_CurrentForce: " << m_CurrentForce << " and m_Increment: " << m_Increment << std::endl;

                // Perform hold if there is a hold time
                if(0 < m_HoldTime){
                  m_CurrentDirection = Direction::Stop;
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->stop();
                  }
                  wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

                  /*
                  {
                    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                    m_WaitActive = true;
                  }
                  */
                  std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                  t1.join();
                  /*
                  {
                    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                    m_WaitActive = false;
                  }
                  */

                  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  wxLogMessage("ContinuousEvent: Holding over.");
                }
              }
            }
          }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based

            // Reduce speed to a tenth if stages are close to the turn point.
            if(((m_CurrentDistance) - m_CurrentLimit) < (200 * m_DistanceThreshold)){
              if(false == m_DecreaseSpeedFlag){
                m_DecreaseSpeedFlag = true;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->setSpeed(m_Velocity/10);
                }
                wxLogMessage("ContinuousEvent: Reduced speed.");
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
                wxLogMessage("ContinuousEvent: Reduced speed.");
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
                //std::cout << "ContinuousEvent moveForward." << std::endl;
              }
            }else if((m_CurrentLimit - (m_CurrentDistance)) > m_DistanceThreshold){
              if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
                m_CurrentDirection = Direction::Backwards;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->moveBackward();
                }
                //std::cout << "ContinuousEvent moveBackward." << std::endl;
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

                  m_CurrentStep = 0;
                  m_CurrentCycle = 0;

                  switch(m_BehaviorAfterStop){
                    case BehaviorAfterStop::GoToL0:
                      m_CurrentState = goBackState;
                      m_CheckDistanceFlag = true;
                      m_CurrentLimit = m_GageLength;
                      wxLogMessage(std::string("ContinuousEvent: Go to gage length: " + std::to_string(m_GageLength)).c_str());
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->gotoStepsDistance(m_GageLength);
                      }
                      break;
                    case BehaviorAfterStop::GoToML:
                      m_CurrentState = goBackState;
                      m_CheckDistanceFlag = true;
                      m_CurrentLimit = m_MountingLength;
                      wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength)).c_str());
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->gotoStepsDistance(m_MountingLength);
                      }
                      break;
                    case BehaviorAfterStop::Stop:
                      {
                        std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                        m_StageFrame->stop();
                      }
                      wxLogMessage("ContinuousEvent: Stop.");
                      m_CurrentState = stopState;
                      m_CurrentDirection = Direction::Stop;
                      m_CheckDistanceFlag = false;
                      {
                        std::lock_guard<std::mutex> lck(*m_WaitMutex);
                        m_Wait->notify_all();
                      }
                      break;
                    case BehaviorAfterStop::HoldAForce:
                      m_CurrentState = goBackForceState;
                      m_CheckDistanceFlag = true;
                      m_CurrentLimit = m_HoldForce;
                      wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_HoldForce / 10000.0)).c_str());

                      if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
                        //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_ForceStressLimit << std::endl;
                        m_CurrentDirection = Direction::Backwards;
                        {
                          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                          m_StageFrame->moveBackward(m_Velocity);
                        }
                      }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
                        //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_ForceStressLimit - m_CurrentForce << std::endl;
                        m_CurrentDirection = Direction::Forwards;
                        {
                          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                          m_StageFrame->moveForward(m_Velocity);
                        }
                      }
                      break;
                  }
                  wxLogMessage("ContinuousEvent: Went to end length.");
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
                    wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

                    /*
                    {
                      std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                      m_WaitActive = true;
                    }
                    */
                    std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                    t1.join();
                    /*
                    {
                      std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                      m_WaitActive = false;
                    }
                    */

                    //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                    wxLogMessage("ContinuousEvent: Holding over.");
                  }
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_StartLength);
                  }
                  wxLogMessage("ContinuousEvent: Go to start length.");
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
                  wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

                  /*
                  {
                    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                    m_WaitActive = true;
                  }
                  */
                  std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                  t1.join();
                  /*
                  {
                    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
                    m_WaitActive = false;
                  }
                  */

                  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                  wxLogMessage("ContinuousEvent: Holding over.");
                  process(Event::evUpdate);
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
        wxLogMessage("ContinuousEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < 0.5*m_DistanceThreshold){
          wxLogMessage("ContinuousEvent: goStartState: Start distance reached.");

          // Set current limit.
          if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
            m_CurrentLimit = m_StartLength + m_Increment;
          } else if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
            m_CurrentLimit = m_CurrentForce + m_Increment;
          }
          m_MaxStressForce = 0;

          m_CheckDistanceFlag = false;
          m_CurrentState = runState;

          // Perform hold if there is a hold time
          if(0 < m_HoldTime){
            wxLogMessage(std::string("ContinuousEvent: Hold for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

            /*
            {
              std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
              m_WaitActive = true;
            }
            */
            std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
            t1.join();
            /*
            {
              std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
              m_WaitActive = false;
            }
            */

            //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
            wxLogMessage("ContinuousEvent: Holding over.");
          }

          //m_CurrentDirection = Direction::Stop;
          wxLogMessage("ContinuousEvent: Go to runState");
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
        wxLogMessage("ContinuousEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < 0.7*m_DistanceThreshold){
          wxLogMessage("ContinuousEvent: Stop.");
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

    case goBackForceState:
      if(Event::evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("ContinuousEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){

        if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
          //std::cout << "(m_CurrentForce - m_CurrentLimit) >  m_ForceStressThreshold: " << (m_CurrentForce - m_CurrentLimit) << " " << m_ForceStressThreshold << std::endl;

          if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }
        }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
          //std::cout << "(m_CurrentLimit - m_CurrentForce) >  m_ForceStressThreshold: " << (m_CurrentLimit - m_CurrentForce) << " " << m_ForceStressThreshold << std::endl;

          if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only reverse motor, if state changed
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
          }
        }else{

          {
            std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
            m_StageFrame->stop();
          }
          wxLogMessage("ContinuousEvent: Stop.");
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CheckDistanceFlag = false;
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }
      }

      break;
  }
}


/**
 * @brief Do all the required thing to stop the experiment during process.
 */
void ContinuousEvent::resetExperiment(void){
  m_StageFrame->stop();
  m_CurrentStep = 0;
  m_CurrentCycle = 0;
  m_MaxStressForce = 0;
  m_CurrentState = stopState;
  m_CurrentDirection = Direction::Stop;
  m_CheckLimitsFlag = false;
  m_GageLength = m_DefaultGageLength;

  initParameters();
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
      /*
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      if((true == m_CheckLimitsFlag) && ((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce))){
        wxLogWarning("OneStepEvent: Force limit exceeded.");
        std::thread t1(&ContinuousEvent::process, this, Event::evStop);
        t1.detach();
        //process(Event::evStop);
      } else{
      */
      if(measurementValue.value > m_MaxStressForce){
        m_MaxStressForce = measurementValue.value;
      }

      {
        //std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
        m_WaitActiveMutex.lock();
        if(((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) ||
           (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) ||
           (true == m_Ramp2FailureActiveFlag)) && (false == m_WaitActive)){
          m_WaitActiveMutex.unlock();
          std::thread t1(&ContinuousEvent::process, this, Event::evUpdate);
          t1.detach();
          //t1.join();
          //process(Event::evUpdate);
        }else{
          m_WaitActiveMutex.unlock();
        }
      }
      //}
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      /*
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      if((true == m_CheckLimitsFlag) && (m_MaxDistanceLimit < m_CurrentDistance) || (m_MinDistanceLimit > m_CurrentDistance)){
        wxLogWarning("OneStepEvent: Distance limit exceeded.");
        std::thread t1(&ContinuousEvent::process, this, Event::evStop);
        t1.detach();
        //process(Event::evStop);
      } else{
      */
      {
        //std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
        m_WaitActiveMutex.lock();
        if(((DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce) || (true == m_CheckDistanceFlag)) && (false == m_WaitActive)){
          m_WaitActiveMutex.unlock();
          std::thread t1(&ContinuousEvent::process, this, Event::evUpdate);
          t1.detach();
          //t1.join();
          //process(Event::evUpdate);
        }else{
          m_WaitActiveMutex.unlock();
        }
      }
      //}
      break;
  }
}

/**
 * @brief Sleep for the amount of milliseconds.
 * @param seconds
 */
void ContinuousEvent::sleepForMilliseconds(double seconds){
  {
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = true;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
  {
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = false;
  }
}
