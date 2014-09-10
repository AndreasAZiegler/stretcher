/**
 * @file continuousevent.cpp
 * @brief Continuous event experiment
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "../gui/myframe.h"
#include "continuousevent.h"

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
 * @param ramptofailureactiveflag Indicates if a ramp 2 failure experiment is active.
 * @param parameters Parameter struct containing the experiment parameters.
 */
ContinuousEvent::ContinuousEvent(ExperimentParameters experimentparameters,

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

                                 ContinuousEventParameters parameters)
      : Experiment(experimentparameters,
                   forcestressthreshold,
                   distancethreshold),
                   //0.01 * 10000.0/*stress force threshold*/,
                   //0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
        m_Wait(wait),
        m_WaitMutex(mutex),

        m_Ramp2FailureActiveFlag(parameters.ramp2failure),
        m_VelocityDistanceOrPercentage(parameters.velocityDistanceOrPercentage),
        m_InitVelocity(parameters.velocity),
        m_Velocity(parameters.velocity),
        m_HoldTime(parameters.holdtime),
        m_IncrementDistanceOrPercentage(parameters.incrementDistanceOrPercentage),
        m_InitIncrement(parameters.increment),
        m_Increment(parameters.increment),
        m_StepsOrMaxValue(parameters.stepsOrMaxValue),
        m_MaxValueDistanceOrPercentage(parameters.maxValueDistanceOrPercentage),
        m_InitMaxValueLimit(parameters.maxvalue),
        m_MaxValueLimit(parameters.maxvalue),
        m_Steps(parameters.steps),
        m_Cycles(parameters.cycles),
        m_BehaviorAfterStop(parameters.behaviorAfterStop),
        m_CurrentState(State::stopState),
        m_CurrentLimit(0),
        m_CurrentStep(0),
        m_CurrentCycle(0),
        m_MaxForceStress(0),
        m_InitHoldForce(parameters.stopAtForceStress),
        m_StopAtForce(parameters.stopAtForceStress),
        m_WaitActive(false),
        m_DecreaseSpeedFlag(false),
        m_CheckDistanceFlag(false),
        m_ExperimentValues(new ContinuousEventValues(experimentparameters.stageframe,
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
                                                     parameters.holdtime,
                                                     parameters.steps,
                                                     parameters.maxvalue,
                                                     parameters.cycles,
                                                     parameters.behaviorAfterStop))
{
  // Initialize parameters.
  initParameters();

  // Registers update method at the message handlers.
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void ContinuousEvent::setParameters(ContinuousEventParameters parameters){
  m_DistanceOrForceOrStress = parameters.distanceOrStressOrForce;
  m_Ramp2FailureActiveFlag = parameters.ramp2failure;
  m_VelocityDistanceOrPercentage = parameters.velocityDistanceOrPercentage;
  m_InitVelocity = parameters.velocity;
  m_Velocity = parameters.velocity;
  m_HoldTime = parameters.holdtime;
  m_IncrementDistanceOrPercentage = parameters.incrementDistanceOrPercentage;
  m_InitIncrement = parameters.increment;
  m_Increment = parameters.increment;
  m_StepsOrMaxValue = parameters.stepsOrMaxValue;
  m_MaxValueDistanceOrPercentage = parameters.maxValueDistanceOrPercentage;
  m_MaxValueLimit = parameters.maxvalue;
  m_InitMaxValueLimit = parameters.maxvalue;
  m_Steps = parameters.steps;
  m_Cycles = parameters.cycles;
  m_BehaviorAfterStop = parameters.behaviorAfterStop;
  m_InitHoldForce = parameters.stopAtForceStress;
  m_StopAtForce = parameters.stopAtForceStress;

  // Initialize parameters.
  initParameters();

  // Update parameters in the experiment values.
  m_ExperimentValues->setParameters(parameters);
}

/**
 * @brief Destructor
 */
ContinuousEvent::~ContinuousEvent(){
  // Unregister update method at the message handlers.
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Initializes the parameters.
 */
void ContinuousEvent::initParameters(void){
  // Calculate the velocity, if defined as percentage.
  if(DistanceOrPercentage::Percentage == m_VelocityDistanceOrPercentage){
    m_Velocity = (m_InitVelocity / 100) * m_GageLength * 0.00009921875/*mm per micro step*/;
    m_ExperimentValues->setVelocity(m_Velocity);
  }

  // Show a warning pop up dialog if the velocity is high.
  if(11 < m_Velocity){
    m_MyFrame->showHighVelocityWarningFromExperiments();
  }

  // Only set increment and steps parameter if experiment is not a ramp to failure experiment.
  if(false == m_Ramp2FailureActiveFlag){

    // Calculate increment and maximum value limit and steps.
    if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
      if(DistanceOrPercentage::Distance == m_IncrementDistanceOrPercentage){
        m_Increment = m_InitIncrement / 0.00009921875/*mm per micro step*/;
        m_ExperimentValues->setIncrement(m_Increment);
      }else if(DistanceOrPercentage::Percentage == m_IncrementDistanceOrPercentage){
        m_Increment = (m_InitIncrement / 100) * m_GageLength;
        m_ExperimentValues->setIncrement(m_Increment);
      }

      if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
        if(DistanceOrPercentage::Distance == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_InitMaxValueLimit / 0.00009921875/*mm per micro step*/;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }else if(DistanceOrPercentage::DistanceRelative == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_StartLength + (m_InitMaxValueLimit / 0.00009921875/*mm per micro step*/);
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }else if(DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = (1 + (m_InitMaxValueLimit / 100)) * m_GageLength;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }

        m_Steps = (m_MaxValueLimit - m_CurrentDistance) / m_Increment;
        wxLogMessage(std::string("ContinuousEvent: Steps: " + std::to_string(m_Steps)).c_str());
      }else if(StepsOrMaxValue::Steps == m_StepsOrMaxValue){

      }

    // Calculate increment and maximum value limit and steps.
    }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
      m_Increment = m_InitIncrement * m_Area * 10.0;
      m_StopAtForce = m_InitHoldForce * m_Area * 10.0;

      if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
        if(DistanceOrPercentage::Distance == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_InitMaxValueLimit * m_Area * 10.0;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
          m_Steps = (m_MaxValueLimit - m_CurrentForce) / m_Increment;
          m_ExperimentValues->setSteps(m_Steps);
          wxLogMessage(std::string("ContinuousEvent: Steps: " + std::to_string(m_Steps)).c_str());
        }else if(DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_InitMaxValueLimit;
          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }
      }else if(StepsOrMaxValue::Steps == m_StepsOrMaxValue){

      }
    // Calculate increment and maximum value limit and steps.
    }else if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
      m_Increment = m_InitIncrement * 10000.0;
      m_StopAtForce = m_InitHoldForce * 10000.0;

      if(StepsOrMaxValue::MaxValue == m_StepsOrMaxValue){
        if(DistanceOrPercentage::Distance == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_InitMaxValueLimit * 10000.0;
          m_Steps = (m_MaxValueLimit - m_CurrentForce) / m_Increment;

          m_ExperimentValues->setSteps(m_Steps);
          wxLogMessage(std::string("ContinuousEvent: Steps: " + std::to_string(m_Steps)).c_str());
        }else if(DistanceOrPercentage::Percentage == m_MaxValueDistanceOrPercentage){
          m_MaxValueLimit = m_InitMaxValueLimit;

          m_ExperimentValues->setMaxValue(m_MaxValueLimit);
        }
      }else if(StepsOrMaxValue::Steps == m_StepsOrMaxValue){

      }
    }
  }else if(true == m_Ramp2FailureActiveFlag){ // Set increment and steps for ramp 2 failure mode.
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
void ContinuousEvent::setPreloadDistance(void){
  m_GageLength = m_CurrentDistance;
  m_ExperimentValues->setGageLength(m_GageLength * 0.00009921875/*mm per micro step*/);

  // Initialize parameters.
  initParameters();
}

/**
 * @brief Sets the start length.
 */
void ContinuousEvent::setStartLength(void){
  m_StartLength = m_CurrentDistance;

  // Initialize parameters.
  initParameters();
}

/**
 * @brief Returns struct with the parameters for the GUI.
 * @return The parameters for the GUI.
 */
ContinuousEventParameters ContinuousEvent::getParametersForGUI(void){
  // Create parameter struct.
  ContinuousEventParameters params;

  params.distanceOrStressOrForce = m_DistanceOrForceOrStress;
  params.ramp2failure = m_Ramp2FailureActiveFlag;
  params.velocityDistanceOrPercentage = m_VelocityDistanceOrPercentage;
  params.velocity = m_InitVelocity;
  params.holdtime = m_HoldTime;
  params.incrementDistanceOrPercentage = m_IncrementDistanceOrPercentage;
  params.increment = m_InitIncrement;
  params.maxValueDistanceOrPercentage = m_MaxValueDistanceOrPercentage;
  params.maxvalue = m_InitMaxValueLimit;
  params.stepsOrMaxValue = m_StepsOrMaxValue;
  params.steps = m_Steps;
  params.cycles = m_Cycles;
  params.behaviorAfterStop = m_BehaviorAfterStop;
  params.stopAtForceStress = m_InitHoldForce;

  return(params);
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void ContinuousEvent::getXML(pugi::xml_document &xml){
  // Add the parameters to the xml configuration.
  pugi::xml_node node = xml.append_child("ContinuousEvent");

  node.append_attribute("DistanceOrStressOrForce") = static_cast<int>(m_DistanceOrForceOrStress);
  node.append_attribute("Ramp2Failure") = m_Ramp2FailureActiveFlag;
  node.append_attribute("VelocityDistanceOrPercentage") = static_cast<int>(m_VelocityDistanceOrPercentage);
  node.append_attribute("Velocity") = m_InitVelocity;
  node.append_attribute("HoldTime") = m_HoldTime;
  node.append_attribute("IncrementDistanceOrPercentage") = static_cast<int>(m_IncrementDistanceOrPercentage);
  node.append_attribute("Increment") = m_InitIncrement;
  node.append_attribute("MaxValueDistanceOrPercentage") = static_cast<int>(m_MaxValueDistanceOrPercentage);
  node.append_attribute("MaxValue") = m_InitMaxValueLimit;
  node.append_attribute("StepsOrMaxValue") = static_cast<int>(m_StepsOrMaxValue);
  node.append_attribute("Steps") = m_Steps;
  node.append_attribute("Cycles") = m_Cycles;
  node.append_attribute("BehaviorAfterStop") = static_cast<int>(m_BehaviorAfterStop);
  node.append_attribute("HoldForce") = m_InitHoldForce;
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
    previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_StartLength));
    timepoint++;
    for(int j = 0; j < m_Steps; ++j){
      // Make point if there is a hold time.
      if(0 < m_HoldTime){
        if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
          previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, (m_StartLength + (j * m_Increment))));
        } else{
          previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, j * m_Increment));
        }
        timepoint++;
      }
      if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, (m_StartLength + ((j + 1) * m_Increment))));
      } else{
        previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, (j + 1) * m_Increment));
        //std::cout << "ContinuousEvent: (j + 1) * m_Increment: " << (j + 1) * m_Increment << ", with m_Increment: " << m_Increment << std::endl;
      }
      timepoint++;

      // Make last point depending on the stop behavior.
      switch(m_BehaviorAfterStop){
        case BehaviorAfterStop::Stop:
          if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
            previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_StartLength + ((j + 1) * m_Increment)));
          } else{
            previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, (j + 1) * m_Increment));
            //std::cout << "ContinuousEvent: (j + 1) * m_Increment: " << (j + 1) * m_Increment << ", with m_Increment: " << m_Increment << std::endl;
          }
          break;
        case BehaviorAfterStop::GoToL0:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_GageLength));
          break;
        case BehaviorAfterStop::GoToML:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Distance, m_MountingLength));
          break;
        case BehaviorAfterStop::HoldAForce:
          previewvalue.push_back(PreviewValue(timepoint, DistanceOrForceOrStress::Force, m_StopAtForce));
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

        // Show a warning pop up dialog if the velocity is high.
        if(11 < m_Velocity){
          if(true == m_MyFrame->showHighVelocityWarningFromExperiments()){
            m_Velocity = 11;
            wxLogMessage("ContinuousEvent: Velocity set to 11 mm/s.");
          }
        }

        wxLogMessage("ContinuousEvent: Start");

        // Set current limit.
        if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
          m_CurrentLimit = m_StartLength + m_Increment;
        } else if((DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress)){
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
          wxLogMessage("ContinuousEvent: Ramp to failure started.");
        }
        // If force/stress based
        else if((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)){
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
        }else if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){ // If distance based
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
        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        m_CurrentCycle = 0;
        {
          std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
          m_StageFrame->stop();
        }
        wxLogMessage("ContinuousEvent: Stop");
        // Notify that experiment is finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If ramp2failure
        if(true == m_Ramp2FailureActiveFlag){
          // If the failure point is reached.
          if(m_CurrentForce < ((m_MaxValueLimit / 100.0) * m_MaxForceStress)){
            wxLogMessage(std::string("ContinuousEvent: Force/Stress dropped under: " + std::to_string(m_MaxValueLimit) + " %%").c_str());
            if((m_Cycles - 1) <= m_CurrentCycle){ // If it is the last cycle.

              m_CurrentStep = 0;
              m_CurrentCycle = 0;

              switch(m_BehaviorAfterStop){
                case BehaviorAfterStop::GoToL0:
                  m_CurrentState = goBackState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_GageLength;
                  wxLogMessage(std::string("ContinuousEvent: Go to L0: " + std::to_string(m_GageLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
                  {
                    std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                    m_StageFrame->gotoStepsDistance(m_GageLength);
                  }
                  break;
                case BehaviorAfterStop::GoToML:
                  m_CurrentState = goBackState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_MountingLength;
                  wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
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
                  m_CurrentState = stopState;
                  m_CurrentDirection = Direction::Stop;
                  m_CheckDistanceFlag = false;
                  wxLogMessage("ContinuousEvent: Stop");
                  {
                    std::lock_guard<std::mutex> lck(*m_WaitMutex);
                    m_Wait->notify_all();
                  }
                  break;
                case BehaviorAfterStop::HoldAForce:
                  m_CurrentState = goBackForceState;
                  m_CheckDistanceFlag = true;
                  m_CurrentLimit = m_StopAtForce;
                  wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_StopAtForce / 10000.0) + " N").c_str());

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
              //wxLogMessage("ContinuousEvent: Went to end length.");
              //process(Event::evUpdate);
            }else{ // If it is not the last cycle.
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

                std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
                t1.join();

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
        else if((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)){
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

            // Perform hold if there is a hold time
            if(0 < m_HoldTime){
              m_CurrentDirection = Direction::Stop;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->stop();
              }
              wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

              std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
              t1.join();

              wxLogMessage("ContinuousEvent: Holding over.");
            }

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
                    wxLogMessage(std::string("ContinuousEvent: Go to gage length: " + std::to_string(m_GageLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->gotoStepsDistance(m_GageLength);
                    }
                    break;
                  case BehaviorAfterStop::GoToML:
                    m_CurrentState = goBackState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_MountingLength;
                    wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
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
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CheckDistanceFlag = false;
                    wxLogMessage("ContinuousEvent: Stop");
                    {
                      std::lock_guard<std::mutex> lck(*m_WaitMutex);
                      m_Wait->notify_all();
                    }
                    break;
                  case BehaviorAfterStop::HoldAForce:
                    m_CurrentState = goBackForceState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_StopAtForce;
                    wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_StopAtForce / 10000.0) + " N").c_str());

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
                //wxLogMessage("ContinuousEvent: Went to end length.");
              }else{ // If it is not the last cycle.
                m_CurrentCycle++;
                m_CheckDistanceFlag = true;
                m_CurrentState = goStartState;

                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->gotoStepsDistance(m_StartLength);
                }
                wxLogMessage("ContinuousEvent: Go to start length.");
              }
            }else{ // If it is not the last step.
              m_CurrentStep++;
              // Update current limit.
              m_CurrentLimit += m_Increment;
              //std::cout << "ContinuousEvent: m_CurrentLimit: " << m_CurrentLimit << ", with m_CurrentForce: " << m_CurrentForce << " and m_Increment: " << m_Increment << std::endl;
            }
          }
        }else if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){ // If distance based

          // Reduce speed to a tenth if stages are close to the turn point.
          if(m_Velocity > 2/*mm/s*/){
            if((m_CurrentDistance - m_CurrentLimit) < (100 * m_DistanceThreshold)){
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
            else if((m_CurrentLimit - m_CurrentDistance) < (100 * m_DistanceThreshold)){
              if(false == m_DecreaseSpeedFlag){
                m_DecreaseSpeedFlag = true;
                {
                  std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                  m_StageFrame->setSpeed(m_Velocity/10);
                }
                wxLogMessage("ContinuousEvent: Reduced speed.");
              }
            }
          }
          //std::cout << "m_CurrentDistance : " << m_CurrentDistance << " m_CurrentLimit: " << (m_CurrentLimit) << std::endl;
          if((m_CurrentDistance - m_CurrentLimit) > m_DistanceThreshold){
            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Forwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveForward(m_Velocity);
              }
              //std::cout << "ContinuousEvent moveForward." << std::endl;
            }
          }else if((m_CurrentLimit - m_CurrentDistance) > m_DistanceThreshold){
            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->moveBackward(m_Velocity);
              }
              //std::cout << "ContinuousEvent moveBackward." << std::endl;
            }
          }else{

            // Reset velocity.
            {
              std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
              m_StageFrame->setSpeed(m_Velocity);
            }

            // Perform hold if there is a hold time
            if(0 < m_HoldTime){
              m_CurrentDirection = Direction::Stop;
              {
                std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                m_StageFrame->stop();
              }
              wxLogMessage(std::string("ContinuousEvent: Holds for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

              std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
              t1.join();

              wxLogMessage("ContinuousEvent: Holding over.");
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
                    wxLogMessage(std::string("ContinuousEvent: Go to gage length: " + std::to_string(m_GageLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
                    {
                      std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
                      m_StageFrame->gotoStepsDistance(m_GageLength);
                    }
                    break;
                  case BehaviorAfterStop::GoToML:
                    m_CurrentState = goBackState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_MountingLength;
                    wxLogMessage(std::string("ContinuousEvent: Go to mounting length: " + std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/) + " mm").c_str());
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
                    m_CurrentState = stopState;
                    m_CurrentDirection = Direction::Stop;
                    m_CheckDistanceFlag = false;
                    wxLogMessage("ContinuousEvent: Stop");
                    {
                      std::lock_guard<std::mutex> lck(*m_WaitMutex);
                      m_Wait->notify_all();
                    }
                    break;
                  case BehaviorAfterStop::HoldAForce:
                    m_CurrentState = goBackForceState;
                    m_CheckDistanceFlag = true;
                    m_CurrentLimit = m_StopAtForce;
                    wxLogMessage(std::string("ContinuousEvent: Go to force: " + std::to_string(m_StopAtForce / 10000.0) + " N").c_str());

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
                //wxLogMessage("ContinuousEvent: Went to end length.");
              } else{
                m_CurrentCycle++;
                m_CheckDistanceFlag = true;
                m_CurrentState = goStartState;

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
              m_DecreaseSpeedFlag = false;

              process(Event::evUpdate);
            }
          }
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
        wxLogMessage("ContinuousEvent: Stop");
        // Notify that experiment is finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        if(std::abs(m_StartLength - m_CurrentDistance) < 0.5*m_DistanceThreshold){
          wxLogMessage("ContinuousEvent: Start distance reached.");

          // Set current limit.
          if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
            m_CurrentLimit = m_StartLength + m_Increment;
          } else if((DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress)){
            m_CurrentLimit = m_CurrentForce + m_Increment;
          }
          m_MaxForceStress = 0;

          m_CheckDistanceFlag = false;
          m_CurrentState = runState;

          // Perform hold if there is a hold time.
          if(0 < m_HoldTime){
            wxLogMessage(std::string("ContinuousEvent: Hold for hold time: " + std::to_string(m_HoldTime * 1000) + " ms").c_str());

            std::thread t1(&ContinuousEvent::sleepForMilliseconds, this, m_HoldTime);
            t1.join();

            wxLogMessage("ContinuousEvent: Holding over.");
          }

          wxLogMessage("ContinuousEvent: Run");
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
        wxLogMessage("ContinuousEvent: Stop");
        // Notify that experiment is finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        //std::cout << "abs(m_StartLength - m_CurrentDistance) < m_DistanceThreshold): " << std::abs(m_StartLength - m_CurrentDistance) << " < " << m_DistanceThreshold << std::endl;
        // Stop experiment if end is reached.
        if(std::abs(m_CurrentLimit - m_CurrentDistance) < 0.7*m_DistanceThreshold){
          wxLogMessage("ContinuousEvent: Stop");
          m_CheckDistanceFlag = false;
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CurrentStep = 0;
          m_CurrentCycle = 0;
          //m_StageFrame->stop();
          // Notify that experiment is finished.
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }
      }
      break;

    case goBackForceState:
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
        wxLogMessage("ContinuousEvent: Stop");
        // Notify that experiment is finished.
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
        }else{ // Stop experiment if force/stress is reached.

          // Stop stage.
          {
            std::lock_guard<std::mutex> lck{m_StageFrameAccessMutex};
            m_StageFrame->stop();
          }
          m_CurrentState = stopState;
          m_CurrentDirection = Direction::Stop;
          m_CheckDistanceFlag = false;
          wxLogMessage("ContinuousEvent: Stop");
          // Notify that experiment is finished.
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
  m_MaxForceStress = 0;
  m_CurrentState = stopState;
  m_CurrentDirection = Direction::Stop;
  m_CheckLimitsFlag = false;
  m_DecreaseSpeedFlag = false;
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
      // Update the maximum force/stress value if current force value is higher than the current maximum force/stress value.
      if((State::runState == m_CurrentState) && (measurementValue.value > m_MaxForceStress)){
        m_MaxForceStress = measurementValue.value;
      }

      {
        // Run process if the experiment is force or stress based or a ramp 2 failure experiment is running and not in a waiting mode.
        m_WaitActiveMutex.lock();
        if(((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) ||
           (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress) ||
           (true == m_Ramp2FailureActiveFlag)) && (false == m_WaitActive)){
          m_WaitActiveMutex.unlock();
          std::thread t1(&ContinuousEvent::process, this, Event::evUpdate);
          t1.detach();
        }else{
          m_WaitActiveMutex.unlock();
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      {
        // Run process if the experiment is distance based or if the distance has the be checked.
        m_WaitActiveMutex.lock();
        if(((DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress) || (true == m_CheckDistanceFlag)) && (false == m_WaitActive)){
          m_WaitActiveMutex.unlock();
          std::thread t1(&ContinuousEvent::process, this, Event::evUpdate);
          t1.detach();
        }else{
          m_WaitActiveMutex.unlock();
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
  {
    // Indicate a waiting mode.
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = true;
  }
  // Sleep
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
  {
    // Stop waiting mode.
    std::lock_guard<std::mutex> lck{m_WaitActiveMutex};
    m_WaitActive = false;
  }
}
