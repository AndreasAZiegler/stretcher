// Includes
#include <iostream>
#include <thread>
#include <wx/log.h>
#include "onestepevent.h"

OneStepEvent::OneStepEvent(std::shared_ptr<StageFrame> stageframe,
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
                           long mountinglength,
                           long zerodistance,
                           long currentdistance,
                           double area,

                           DistanceOrPercentage velocityDistanceOrPercentage,
                           double velocitypercent,
                           double velocity,
                           double delay,
                           DistanceOrPercentage upperlimitDistanceOrPercentage,
                           double upperlimitpercent,
                           long upperlimit,
                           double dwell,
                           bool holdupperlimit,
                           DistanceOrPercentage holdDistanceOrPercentage,
                           double holddistancepercent,
                           long holddistance,
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
               mountinglength,
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
    m_Delay(delay),
    m_UpperLimitDistanceOrPercentage(upperlimitDistanceOrPercentage),
    m_UpperLimitPercent(upperlimitpercent),
    m_UpperLimit(upperlimit),
    m_InitRelUpperLimit(upperlimit),
    m_Dwell(dwell),
    m_HoldUpperLimitFlag(holdupperlimit),
    m_Cycles(cycles),
    m_HoldDistanceOrPercentage(holdDistanceOrPercentage),
    m_HoldDistancePercent(holddistancepercent),
    m_HoldDistance(holddistance),
    m_InitRelHoldDistance(holddistance),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_CurrentState(State::stopState),
    m_CurrentLimit(0),
    m_CurrentCycle(0),
    m_WaitActive(false),
    m_DecreaseSpeedFlag(false),
    m_CheckDistanceFlag(false),
    m_ExperimentValues(new OneStepEventValues(stageframe,
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
                                              delay,
                                              upperlimit,
                                              dwell,
                                              holddistance,
                                              cycles,
                                              behaviorAfterStop))
{
  initParameters();
  /*
  if(DistanceOrPercentage::Distance == m_HoldDistanceOrPercentage){
    m_HoldDistance *= 10000.0;
  }
  */

  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
OneStepEvent::~OneStepEvent(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Initializes the parameters.
 */
void OneStepEvent::initParameters(void){
  if(DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_VelocityPercent / 100.0) * m_GageLength * 0.00009921875/*mm per micro step*/;
    /*
    wxLogMessage(std::string("OneStepEvent: Velocity percent: " + std::to_string(m_VelocityPercent) +
                             " velocity: " + std::to_string(m_Velocity)).c_str());
    */
    m_ExperimentValues->setVelocity(m_Velocity);
  }
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    if(DistanceOrPercentage::DistanceRelative == m_UpperLimitDistanceOrPercentage){
      m_UpperLimit = m_StartLength + m_InitRelUpperLimit;
      std::cout << "OneStepEvent: upper limit: " << m_UpperLimit * 0.00009921875/*mm per micro step*/ << std::endl;
      m_ExperimentValues->setUpperLimit(m_UpperLimit);
    }else if(DistanceOrPercentage::Percentage == m_UpperLimitDistanceOrPercentage){
      m_UpperLimit = (1 + (m_UpperLimitPercent / 100.0)) * m_GageLength;
      m_ExperimentValues->setUpperLimit(m_UpperLimit);
      //std::cout << "OneStepEvent upper limit percent: " << m_UpperLimitPercent << " upper limit: " << m_UpperLimit * 0.00009921875 << std::endl;
    }
  }

  if(DistanceOrPercentage::DistanceRelative == m_HoldDistanceOrPercentage){
    m_HoldDistance = m_StartLength + m_InitRelHoldDistance;
    m_ExperimentValues->setHoldDistance(m_HoldDistance);
  }else if(DistanceOrPercentage::Percentage == m_HoldDistanceOrPercentage){
    m_HoldDistance = (1 + (m_HoldDistancePercent / 100.0)) * m_GageLength;
    m_ExperimentValues->setHoldDistance(m_HoldDistance);
    //std::cout << "OneStepEvent hold distance percent: " << m_HoldDistance << " m_GageLength: " << m_GageLength/* 0.00009921875*/ << std::endl;
  }
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void OneStepEvent::setPreloadDistance(){
  m_GageLength = m_CurrentDistance;

  initParameters();
  }

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void OneStepEvent::getPreview(std::vector<Experiment::PreviewValue>& previewvalue){
  // Calculate time point.
  int timepoint;
  if(0 == previewvalue.size()){
   timepoint = 0;
  } else{
   timepoint = previewvalue.back().getTimepoint();
  }
  //previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, 0));

  for(int i = 0; i < m_Cycles; ++i){
    // Make start point.
    previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_StartLength));
    timepoint++;

    // Make point if there is a delay.
    if(0 < m_Delay){
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_StartLength));
      //previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, 0));
      timepoint++;
    }
    // Make upper limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
    timepoint++;
    // Make point if there is a dwell.
    if(0 < m_Dwell){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_UpperLimit));
      timepoint++;
    }
  }
  timepoint++;

  // Make last point depending on the stop behavior.
  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::Stop:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_UpperLimit));
        break;
    case BehaviorAfterStop::GoToL0:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_GageLength));
        break;
    case BehaviorAfterStop::GoToML:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_MountingLength));
        break;
    case BehaviorAfterStop::HoldADistance:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrStressOrForce::Distance, m_HoldDistance));
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

        wxLogMessage("OneStepEvent: Start experiment.");

        // Perform hold if there is a delay.
        if(0 < m_Delay){
          wxLogMessage(std::string("OneStepEvent: Delay, hold for: " + std::to_string(m_Delay * 1000) + " ms").c_str());
          std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Delay);
          t1.join();
          //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
          wxLogMessage("OneStepEvent: Holding over.");
        }

        // Change limit.
        m_CurrentLimit = m_UpperLimit;
        // Set stages speed.
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->setSpeed(m_Velocity);
        }
        m_CurrentState = runState;
        // Activate limits checking.
        m_CheckLimitsFlag = true;

        // If force based
        if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
          if((m_CurrentLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_CurrentLimit: " << m_CurrentLimit << " m_CurrentForce: " << m_CurrentForce << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentForce - m_CurrentLimit) > m_ForceStressThreshold){
            //std::cout  << "m_CurrentForce: " << m_CurrentForce<< " m_CurrentLimit: " << m_CurrentLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based
          if((m_CurrentLimit - m_CurrentDistance) > m_DistanceThreshold){
            //std::cout << "m_CurrentDistance - m_DistanceLimit: " << (m_CurrentDistance) - m_CurrentLimit << std::endl;
            m_CurrentDirection = Direction::Forwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveForward(m_Velocity);
            }
            //std::cout << "OneStepEvent moveForward" << std::endl;
          }else if((m_CurrentDistance - m_CurrentLimit) > m_DistanceThreshold){
            //std::cout << "m_DistanceLimit - m_CurrentDistance : " << m_CurrentLimit - (m_CurrentDistance) << std::endl;
            m_CurrentDirection = Direction::Backwards;
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->moveBackward(m_Velocity);
            }
            //std::cout << "OneStepEvent moveBackward" << std::endl;
          }
        }
      }
      break;

    case runState:

      if(Event::evStop == event){
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If force based
        if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
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
            //m_StageFrame->stop();
            //std::cout << "Go to preload distance" << std::endl;

            // Perform hold if there is a dwell
            if(0 < m_Dwell){
              m_CurrentDirection = Direction::Stop;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->stop();
              }
              wxLogMessage(std::string("OneStepEvent: Dwell, hold for: " + std::to_string(m_Dwell * 1000) + " ms").c_str());
              m_CurrentState = holdForceState;
              std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Dwell);
              t1.join();
              m_CurrentState = goStartState;
              //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime2 * 1000)));
              wxLogMessage("OneStepEvent: Holding over.");
            }
            //process(Event::evUpdate);
            if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

              m_CurrentState = goBackState;
              m_CheckDistanceFlag = true;
              m_CurrentCycle = 0;

              switch(m_BehaviorAfterStop){
                case BehaviorAfterStop::Stop:
                  {
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CurrentCycle = 0;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    wxLogMessage("OneStepEvent: Stop.");
                    std::lock_guard<std::mutex> lck(*m_WaitMutex);
                    m_Wait->notify_all();
                    break;
                  }
                case BehaviorAfterStop::GoToL0:
                  m_CurrentLimit = m_GageLength;
                  wxLogMessage(std::string("OneStepEvent: Go to gage length: " + std::to_string(m_GageLength * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_GageLength);
                  break;
                case BehaviorAfterStop::GoToML:
                  m_CurrentLimit = m_MountingLength;
                  wxLogMessage(std::string("OneStepEvent: Go to mounting length: " + std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_MountingLength);
                  break;
                case BehaviorAfterStop::HoldADistance:
                  m_CurrentLimit = m_HoldDistance;
                  wxLogMessage(std::string("OneStepEvent: Go to hold distance: " + std::to_string(m_HoldDistance * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_HoldDistance);
                  break;
              }
              wxLogMessage("OneStepEvent: Go to end length.");
              //process(Event::evUpdate);
            }else{
              m_CurrentCycle++;
              m_CheckDistanceFlag = true;
              m_CurrentState = goStartState;

              // Perform hold if there is a delay.
              if(0 < m_Delay){
                m_CurrentDirection = Direction::Stop;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->stop();
                }
                wxLogMessage(std::string("OneStepEvent: Delay, hold for: " + std::to_string(m_Delay * 1000) + " ms").c_str());
                std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Delay);
                t1.join();
                //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                wxLogMessage("OneStepEvent: Holding over.");
              }
              {
                // Go back to the start lengt.
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
              wxLogMessage("OneStepEvent: Go to start length.");
            }
          }
        }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){ // If distance based

          // Reduce speed to a tenth if stages are close to the turn point.
          if((m_CurrentDistance - m_CurrentLimit) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              if(5 < m_Velocity){
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->setSpeed(m_Velocity/10);
                wxLogMessage("OneStepEvent: Reduced speed.");
              }
            }
          }
          // Reduce speed to a tenth if stages are close to the turn point.
          else if((m_CurrentLimit - m_CurrentDistance) < (200 * m_DistanceThreshold)){
            if(false == m_DecreaseSpeedFlag){
              m_DecreaseSpeedFlag = true;
              if(5 < m_Velocity){
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->setSpeed(m_Velocity/10);
                wxLogMessage("OneStepEvent: Reduced speed.");
              }
            }
          }
          //std::cout << "m_CurrentDistance : " << m_CurrentDistance << " m_CurrentLimit: " << (m_CurrentLimit) << std::endl;
          if((m_CurrentDistance - m_CurrentLimit) > m_DistanceThreshold){
            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Forwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveForward();
              }
              //std::cout << "OneStepEvent moveForward." << std::endl;
            }
          }else if((m_CurrentLimit - m_CurrentDistance) > m_DistanceThreshold){
            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveBackward();
              }
              //std::cout << "OneStepEvent moveBackward." << std::endl;
            }
          }else{
            //m_CurrentState = goBackState;
            // Return to normal speed.
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->setSpeed(m_Velocity);
            }
            //m_StageFrame->stop();

            // Perform hold if there is a dwell
            if(0 < m_Dwell){
              m_CurrentDirection = Direction::Stop;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->stop();
              }
              //std::cout << "OneStepEvent stages should stop." << std::endl;
              wxLogMessage(std::string("OneStepEvent: Dwell, hold for: " + std::to_string(m_Dwell * 1000) + " ms").c_str());
              std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Dwell);
              t1.join();
              //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime2 * 1000)));
              wxLogMessage("OneStepEvent: Holding over.");
            }
            //process(Event::evUpdate);
            if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

              m_CurrentState = goBackState;
              m_CheckDistanceFlag = true;
              m_CurrentCycle = 0;

              switch(m_BehaviorAfterStop){
                case BehaviorAfterStop::Stop:
                  {
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CurrentCycle = 0;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    wxLogMessage("OneStepEvent: Stop.");
                    std::lock_guard<std::mutex> lck(*m_WaitMutex);
                    m_Wait->notify_all();
                    break;
                  }
                case BehaviorAfterStop::GoToL0:
                  m_CurrentLimit = m_GageLength;
                  wxLogMessage(std::string("OneStepEvent: Go to gage length: " + std::to_string(m_GageLength * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_GageLength);
                  break;
                case BehaviorAfterStop::GoToML:
                  m_CurrentLimit = m_MountingLength;
                  wxLogMessage(std::string("OneStepEvent: Go to mounting length: " + std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_MountingLength);
                  break;
                case BehaviorAfterStop::HoldADistance:
                  m_CurrentLimit = m_HoldDistance;
                  wxLogMessage(std::string("OneStepEvent: Go to hold distance: " + std::to_string(m_HoldDistance * 0.00009921875/*mm per micro step*/)).c_str());
                  m_StageFrame->gotoStepsDistance(m_HoldDistance);
                  break;
              }
              wxLogMessage("OneStepEvent: Go to end length.");
              //process(Event::evUpdate);
            }else{
              m_CurrentCycle++;
              m_CheckDistanceFlag = true;
              m_CurrentState = goStartState;

              // Perform hold if there is a delay
              if(0 < m_Delay){
                m_CurrentDirection = Direction::Stop;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->stop();
                }
                //std::cout << "OneStepEvent: stages should stop." << std::endl;
                wxLogMessage(std::string("OneStepEvent: Delay, hold for: " + std::to_string(m_Delay * 1000) + " ms").c_str());
                std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Delay);
                t1.join();
                //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(m_HoldTime1 * 1000)));
                wxLogMessage("OneStepEvent: Holding over.");
              }
              {
                // Go back to the start lengt.
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
              wxLogMessage("OneStepEvent: Go to start length.");
            }
          }
        }
      }
      break;

    case holdForceState:
      if(Event::evStop == event){
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
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
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
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
        wxLogMessage("OneStepEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < 0.5*m_DistanceThreshold){
          wxLogMessage("OneStepEvent: goStartState: Start distance reached.");
          m_CurrentLimit = m_UpperLimit;
          m_CheckDistanceFlag = false;
          m_CurrentState = runState;
          m_CurrentDirection = Direction::Stop;
          wxLogMessage("OneStepEvent:: Go to runState");
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
        wxLogMessage("OneStepEvent: Stop.");
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < 0.7*m_DistanceThreshold){
          m_CheckDistanceFlag = false;
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CurrentCycle = 0;
          //m_StageFrame->stop();
          wxLogMessage("OneStepEvent: Stop.");
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
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void OneStepEvent::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      /*
      if((true == m_CheckLimitsFlag) && ((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce))){
        wxLogWarning("OneStepEvent: Force limit exceeded.");
        std::thread t1(&OneStepEvent::process, this, Event::evStop);
        t1.detach();
        //process(Event::evStop);
      } else{
      */
      m_WaitActiveMutex.lock();
      // Process with the FSM if the experiment is force/stress based,
      // if there is no waiting active or if waiting is active but also holding upper limit is active.
      if(((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) ||
          (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)) && ((false == m_WaitActive) || (true == m_HoldUpperLimitFlag))){
        m_WaitActiveMutex.unlock();
        std::thread t1(&OneStepEvent::process, this, Event::evUpdate);
        t1.detach();
        //process(Event::evUpdate);
      }else{
        m_WaitActiveMutex.unlock();
      }
      //}
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      // Stops the experiment if the limits should be checked and a limit is exceeded.
      /*
      if((true == m_CheckLimitsFlag) && (m_MaxDistanceLimit < m_CurrentDistance) || (m_MinDistanceLimit > m_CurrentDistance)){
        wxLogWarning("OneStepEvent: Distance limit exceeded.");
        std::thread t1(&OneStepEvent::process, this, Event::evStop);
        t1.detach();
        //process(Event::evStop);
      } else{
      */
      m_WaitActiveMutex.lock();
      // Process with the FSM if the experiment is force/stress based and if there is no waiting active.
      if(((DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce) || (true == m_CheckDistanceFlag)) && (false == m_WaitActive)){
        m_WaitActiveMutex.unlock();
        std::thread t1(&OneStepEvent::process, this, Event::evUpdate);
        t1.detach();
        //process(Event::evUpdate);
      }else{
        m_WaitActiveMutex.unlock();
      }
    //}
      break;
  }
  //process(Event::evUpdate);
}

/**
* @brief Do all the required thing to stop the experiment during process.
*/
void OneStepEvent::resetExperiment(void){
  m_StageFrame->stop();
  m_CurrentCycle = 0;
  m_CurrentState = stopState;
  m_CheckLimitsFlag = false;
  m_CurrentDirection = Direction::Stop;
  m_GageLength = m_DefaultGageLength;

  initParameters();
}

/**
 * @brief Sleep for the amount of milliseconds.
 * @param milliseconds
 */
void OneStepEvent::sleepForMilliseconds(double seconds){
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
