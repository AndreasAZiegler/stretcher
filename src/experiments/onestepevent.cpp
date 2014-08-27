/**
 * @file onestepevent.cpp
 * @brief One step event experiment
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <thread>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "../gui/myframe.h"
#include "onestepevent.h"

/**
 * @brief Initialize all the required parameters and registers the update methods at the message handlers.
 * @param experimentparameters Common experiment parameters.
 * @param path Path to the folder for exports.
 * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
 * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
 * @param *vectoraccessmutex Pointer to the graph access mutex.
 * @param *maxlimitgraph Pointer to the maximum limit graph.
 * @param *minlimitgraph Pointer to the minimum limit graph.
 * @param *wait Pointer to the wait condition variable.
 * @param *mutex Pointer to the mutex.
 * @param *stagesstopped Pointer to the flag stages stopped.
 * @param *stagesstoppedmutex Pointer to the mutex to protect the stagesstopped flag.
 * @param parameters Parameter struct containing the experiment parameters.
 */
OneStepEvent::OneStepEvent(ExperimentParameters experimentparameters,

                           std::string path,
                           mpFXYVector *forceStressDistanceGraph,
                           mpFXYVector *forceStressDisplacementGraph,
                           std::mutex *vectoraccessmutex,
                           mpFXYVector *maxforcelimitvector,
                           mpFXYVector *minforcelimitvector,
                           mpFXYVector *maxdistancelimitvector,
                           mpFXYVector *mindistancelimitvector,
                           long forcestressthreshold,
                           long distancethreshold,

                           std::condition_variable *wait,
                           std::mutex *mutex,

                           OneStepEventParameters parameters)
  : Experiment(experimentparameters,
               forcestressthreshold,
               distancethreshold),
               //0.01 * 10000.0/*stress force threshold*/,
               //0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_Wait(wait),
    m_WaitMutex(mutex),

    m_VelocityDistanceOrPercentage(parameters.velocityDistanceOrPercentage),
    m_InitVelocity(parameters.velocity),
    m_Velocity(parameters.velocity),
    m_Delay(parameters.delay),
    m_LimitDistanceOrPercentage(parameters.limitDistanceOrPercentage),
    m_InitLimit(parameters.limit),
    m_Limit(parameters.limit),
    m_Dwell(parameters.dwell),
    m_HoldLimitFlag(parameters.holdLimit),
    m_Cycles(parameters.cycles),
    m_HoldDistanceOrPercentage(parameters.holdDistanceOrPercentage),
    m_InitHoldDistance(parameters.holdDistance),
    m_HoldDistance(parameters.holdDistance),
    m_BehaviorAfterStop(parameters.behaviorAfterStop),
    m_CurrentState(State::stopState),
    m_CurrentLimit(0),
    m_CurrentCycle(0),
    m_WaitActive(false),
    m_DecreaseSpeedFlag(false),
    m_CheckDistanceFlag(false),
    m_ExperimentValues(new OneStepEventValues(experimentparameters.stageframe,
                                              experimentparameters.forcesensormessagehandler,
                                              forceStressDistanceGraph,
                                              forceStressDisplacementGraph,
                                              vectoraccessmutex,
                                              maxforcelimitvector,
                                              minforcelimitvector,
                                              maxdistancelimitvector,
                                              mindistancelimitvector,
                                              experimentparameters.myframe,
                                              path,

                                              experimentparameters.type,
                                              experimentparameters.distanceOrForceOrStress,
                                              experimentparameters.area,
                                              experimentparameters.gagelength,

                                              parameters.velocity,
                                              parameters.delay,
                                              parameters.limit,
                                              parameters.dwell,
                                              parameters.holdDistance,
                                              parameters.cycles,
                                              parameters.behaviorAfterStop))
{
  // Initialize parameters.
  initParameters();

  // Registers the update method at the message handlers.
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void OneStepEvent::setParameters(OneStepEventParameters parameters){
  setDistanceOrForceOrStress(parameters.distanceOrStressOrForce);
  m_VelocityDistanceOrPercentage = parameters.velocityDistanceOrPercentage;
  m_InitVelocity = parameters.velocity;
  m_Velocity = parameters.velocity;
  m_Delay = parameters.delay;
  m_LimitDistanceOrPercentage = parameters.limitDistanceOrPercentage;
  m_InitLimit = parameters.limit;
  m_Limit = parameters.limit;
  m_Dwell = parameters.dwell;
  m_Cycles = parameters.cycles;
  m_HoldDistanceOrPercentage = parameters.holdDistanceOrPercentage;
  m_InitHoldDistance = parameters.holdDistance;
  m_HoldDistance = parameters.holdDistance;
  m_BehaviorAfterStop = parameters.behaviorAfterStop;

  // Initialize parameters.
  initParameters();

  // Update the parameters in the experiment values.
  m_ExperimentValues->setParameters(parameters);
}

/**
 * @brief Destructor
 */
OneStepEvent::~OneStepEvent(){
  // Unregisters the update method at the message handlers.
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Initializes the parameters.
 */
void OneStepEvent::initParameters(void){
  // Calculation of the velocity if it is defined in percent.
  if(DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_InitVelocity / 100.0) * m_GageLength * 0.00009921875/*mm per micro step*/;
    /*
    wxLogMessage(std::string("OneStepEvent: Velocity percent: " + std::to_string(m_VelocityPercent) +
                             " velocity: " + std::to_string(m_Velocity)).c_str());
    */
    m_ExperimentValues->setVelocity(m_Velocity);
  }

  // Calculation of the limit.
  if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
    if(DistanceOrPercentage::DistanceRelative == m_LimitDistanceOrPercentage){
      m_Limit = m_StartLength + (m_InitLimit / 0.00009921875/*mm per micro step*/);
      //std::cout << "OneStepEvent: upper limit: " << m_UpperLimit * 0.00009921875/*mm per micro step*/ << " , m_StartLength: "
      //          << m_StartLength * 0.00009921875/*mm per micro step*/ << " , m_InitRelUpperLimit: "
      //          << m_InitRelUpperLimit * 0.00009921875/*mm per micro step*/ << std::endl;
      m_ExperimentValues->setLimit(m_Limit);
    }else if(DistanceOrPercentage::Distance == m_LimitDistanceOrPercentage){
      m_Limit = m_InitLimit / 0.00009921875/*mm per micro step*/;
    }else if(DistanceOrPercentage::Percentage == m_LimitDistanceOrPercentage){
      m_Limit = (1 + (m_InitLimit / 100.0)) * m_GageLength;
      m_ExperimentValues->setLimit(m_Limit);
      //std::cout << "OneStepEvent upper limit percent: " << m_UpperLimitPercent << " upper limit: " << m_UpperLimit * 0.00009921875 << std::endl;
    }
  // Calculation of the limit.
  }else{
    if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
      m_Limit = m_InitLimit * m_Area * 10.0;
    } else if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
      m_Limit = m_InitLimit * 10000.0;
    }
  }

  // Calculation of the hold distance.
  if(DistanceOrPercentage::DistanceRelative == m_HoldDistanceOrPercentage){
    m_HoldDistance = m_StartLength + (m_InitHoldDistance / 0.00009921875/*mm per micro step*/);
    m_ExperimentValues->setHoldDistance(m_HoldDistance);
  }else if(DistanceOrPercentage::Distance == m_HoldDistanceOrPercentage){
    m_HoldDistance = m_InitHoldDistance / 0.00009921875/*mm per micro step*/;
  }else if(DistanceOrPercentage::Percentage == m_HoldDistanceOrPercentage){
    m_HoldDistance = (1 + (m_InitHoldDistance / 100.0)) * m_GageLength;
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

  // Initialize parameters.
  initParameters();
}

/**
 * @brief Returns struct with the parameters for the GUI.
 * @return The parameters for the GUI.
 */
OneStepEventParameters OneStepEvent::getParametersForGUI(void){
  OneStepEventParameters parameters;

  parameters.distanceOrStressOrForce = m_DistanceOrForceOrStress;
  parameters.velocityDistanceOrPercentage = m_VelocityDistanceOrPercentage;
  parameters.velocity = m_InitVelocity;
  parameters.delay = m_Delay;

  parameters.limitDistanceOrPercentage = m_LimitDistanceOrPercentage;
  parameters.limit = m_InitLimit;

  parameters.dwell = m_Dwell;
  parameters.cycles = m_Cycles;
  parameters.behaviorAfterStop = m_BehaviorAfterStop;
  parameters.holdDistanceOrPercentage = m_HoldDistanceOrPercentage;
  parameters.holdDistance = m_InitHoldDistance;

  return(parameters);
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void OneStepEvent::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("OneStepEvent");

  node.append_attribute("DistanceOrStressOrForce") = static_cast<int>(m_DistanceOrForceOrStress);
  node.append_attribute("CrossSectionArea") = m_Area;
  node.append_attribute("VelocityDistanceOrPercentage") = static_cast<int>(m_VelocityDistanceOrPercentage);
  node.append_attribute("Velocity") = m_Velocity;
  node.append_attribute("Delay") = m_Delay;
  node.append_attribute("LimitDistanceOrPercentage") = static_cast<int>(m_LimitDistanceOrPercentage);
  node.append_attribute("Limit") = m_InitLimit;
  node.append_attribute("Dwell") = m_Dwell;
  node.append_attribute("Cycles") = m_Cycles;
  node.append_attribute("BehaviorAfterStop") = static_cast<int>(m_BehaviorAfterStop);
  node.append_attribute("HoldDistance") = m_InitHoldDistance;
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
    previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_StartLength));
    timepoint++;

    // Make point if there is a delay.
    if(0 < m_Delay){
      previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_StartLength));
      //previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, 0));
      timepoint++;
    }
    // Make upper limit point.
    previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_Limit));
    timepoint++;
    // Make point if there is a dwell.
    if(0 < m_Dwell){
      previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_Limit));
      timepoint++;
    }
  }
  timepoint++;

  // Make last point depending on the stop behavior.
  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::Stop:
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_Limit));
        break;
    case BehaviorAfterStop::GoToL0:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_GageLength));
        break;
    case BehaviorAfterStop::GoToML:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_MountingLength));
        break;
    case BehaviorAfterStop::HoldADistance:
        previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_HoldDistance));
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

        // Show a warning pop up dialog if the velocity is high.
        if(11 < m_Velocity){
          if(true == m_MyFrame->showHighVelocityWarningFromExperiments()){
            m_Velocity = 11;
            std::cout << "OneStepEvent: Velocity set to 11." << std::endl;
          }
        }

        wxLogMessage("OneStepEvent: Start experiment.");

        // Perform hold if there is a delay.
        if(0 < m_Delay){
          wxLogMessage(std::string("OneStepEvent: Delay, hold for: " + std::to_string(m_Delay * 1000) + " ms").c_str());
          std::thread t1(&OneStepEvent::sleepForMilliseconds, this, m_Delay);
          t1.join();
          wxLogMessage("OneStepEvent: Holding over.");
        }

        // Change limit.
        m_CurrentLimit = m_Limit;
        // Set stages speed.
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->setSpeed(m_Velocity);
        }
        m_CurrentState = runState;
        // Activate limits checking.
        m_CheckLimitsFlag = true;

        // If force based
        if((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)){
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
        }else if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){ // If distance based
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
        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
        // Notify that experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If force based
        if((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)){
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
                    // Stop stage.
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CurrentCycle = 0;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    wxLogMessage("OneStepEvent: Stop.");
                    // Notify that experiment finished.
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

              {
                // Go back to the start lengt.
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->gotoStepsDistance(m_StartLength);
              }
              wxLogMessage("OneStepEvent: Go to start length.");
            }
          }
        }else if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){ // If distance based

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
            // Return to normal speed.
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->setSpeed(m_Velocity);
            }

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
              wxLogMessage("OneStepEvent: Holding over.");
            }
            if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

              m_CurrentState = goBackState;
              m_CheckDistanceFlag = true;
              m_CurrentCycle = 0;

              switch(m_BehaviorAfterStop){
                case BehaviorAfterStop::Stop:
                  {
                    // Stop stasge.
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CurrentCycle = 0;
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->stop();
                    }
                    wxLogMessage("OneStepEvent: Stop.");
                    // Notify that the experiment finished.
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
            }else{
              m_CurrentCycle++;
              m_CheckDistanceFlag = true;
              m_CurrentState = goStartState;

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
        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
        // Notify that the experiment finished.
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
          // Stop stage.
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
      }
      break;

    case goStartState:
      if(Event::evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < 0.5*m_DistanceThreshold){
          wxLogMessage("OneStepEvent: goStartState: Start distance reached.");
          m_CurrentLimit = m_Limit;
          m_CheckDistanceFlag = false;

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
            wxLogMessage("OneStepEvent: Holding over.");
          }

          m_CurrentState = runState;
          m_CurrentDirection = Direction::Stop;
          wxLogMessage("OneStepEvent:: Go to runState");
          process(Event::evUpdate);
        }
      }
    break;

    case goBackState:
      if(Event::evStop == event){
        //std::cout << "Conditioning FSM switched to state: stopState." << std::endl;
        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("OneStepEvent: Stop.");
        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < 0.7*m_DistanceThreshold){
          // Stop stage.
          m_CheckDistanceFlag = false;
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CurrentCycle = 0;
          //m_StageFrame->stop();
          wxLogMessage("OneStepEvent: Stop.");
          // Notify that the experiment finished.
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
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
      m_WaitActiveMutex.lock();
      // Process with the FSM if the experiment is force/stress based,
      // if there is no waiting active or if waiting is active but also holding upper limit is active.
      if(((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) ||
          (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)) && ((false == m_WaitActive) || (true == m_HoldLimitFlag))){
        m_WaitActiveMutex.unlock();
        std::thread t1(&OneStepEvent::process, this, Event::evUpdate);
        t1.detach();
      }else{
        m_WaitActiveMutex.unlock();
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      // Process with the FSM if the experiment is force/stress based and if there is no waiting active.
      m_WaitActiveMutex.lock();
      if(((DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress) || (true == m_CheckDistanceFlag)) && (false == m_WaitActive)){
        m_WaitActiveMutex.unlock();
        std::thread t1(&OneStepEvent::process, this, Event::evUpdate);
        t1.detach();
      }else{
        m_WaitActiveMutex.unlock();
      }
      break;
  }
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

  // Initialize parameters.
  initParameters();
}

/**
 * @brief Sleep for the amount of milliseconds.
 * @param milliseconds
 */
void OneStepEvent::sleepForMilliseconds(double seconds){
  {
    // Activate waiting state.
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = true;
  }
  // Sleep
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
  {
    // Deactivate waiting state.
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = false;
  }
}
