// Includes
#include <iostream>
#include <mutex>
#include <wx/log.h>
#include "../gui/myframe.h"
#include "pugixml/pugixml.hpp"
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
Preload::Preload(ExperimentParameters experimentparameters,

                 std::string path,
                 mpFXYVector *forceStressDistanceGraph,
                 mpFXYVector *forceStressDisplacementGraph,
                 std::mutex *vectoraccessmutex,
                 mpFXYVector *maxlimitvector,
                 mpFXYVector *minlimitvector,
                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,

                 PreloadParameters parameters)
  : Experiment(experimentparameters,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_StageFrame(experimentparameters.stageframe),
    m_ForceSensorMessageHandler(experimentparameters.forcesensormessagehandler),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_StagesStoppedFlag(stagesstopped),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_InitStressForceLimit(parameters.stressForceLimit),
    m_StressForceLimit(parameters.stressForceLimit),
    m_Velocity(parameters.velocity),
    m_ExperimentValues(std::make_shared<PreloadValues>(experimentparameters.stageframe,
                                                       experimentparameters.forcesensormessagehandler,
                                                       forceStressDistanceGraph,
                                                       forceStressDisplacementGraph,
                                                       vectoraccessmutex,
                                                       maxlimitvector,
                                                       minlimitvector,

                                                       experimentparameters.myframe,

                                                       experimentparameters.type,
                                                       experimentparameters.distanceOrForceOrStress,
                                                       experimentparameters.area,
                                                       experimentparameters.gagelength,

                                                       parameters.stressForceLimit,
                                                       parameters.velocity))
{
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    m_StressForceLimit = m_InitStressForceLimit * 10000.0;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    m_StressForceLimit = m_InitStressForceLimit * m_Area * 10;
  }
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void Preload::setParameters(PreloadParameters parameters){

  m_DistanceOrStressOrForce = parameters.distanceOrStressOrForce;
  m_InitStressForceLimit = parameters.stressForceLimit;
  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    m_StressForceLimit = m_InitStressForceLimit * 10000.0;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    m_StressForceLimit = m_InitStressForceLimit * m_Area * 10;
  }
  m_Velocity = parameters.velocity;

  m_ExperimentValues->setParameters(parameters);
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void Preload::setPreloadDistance(){

}

Preload::~Preload(){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  // Delete the experiment values because we don't need them for the preloading.
  //delete m_ExperimentValues;
  std::cout << "Preload destructor finished." << std::endl;
}

/**
 * @brief Returns struct with the parameters for the GUI.
 * @return The parameters for the GUI.
 */
PreloadParameters Preload::getParametersForGUI(void){
  PreloadParameters parameters;

  parameters.distanceOrStressOrForce = m_DistanceOrStressOrForce;

  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    parameters.stressForceLimit = m_InitStressForceLimit;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    parameters.stressForceLimit = m_InitStressForceLimit;
  }

  parameters.velocity = m_Velocity;

  return(parameters);
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void Preload::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("Preload");

  node.append_attribute("StressOrForce") = static_cast<int>(m_DistanceOrStressOrForce);
  node.append_attribute("CrossSectionArea") = m_Area;
  node.append_attribute("ForceOrStress") = static_cast<int>(m_DistanceOrStressOrForce);

  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    node.append_attribute("ForceStressLimit") = m_InitStressForceLimit;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    node.append_attribute("ForceStressLimit") = m_InitStressForceLimit;
  }

  node.append_attribute("Velocity") = m_Velocity;
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
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void Preload::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;
      /*
      if((true == m_CheckLimitsFlag) && ((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce))){
        wxLogWarning(std::string("Preload: Force limit exceeded, current force: " + std::to_string(m_CurrentForce) +
                                 " m_MaxForceLimit: " + std::to_string(m_MaxForceLimit)).c_str());
        process(Event::evStop);
      } else{
      */
        process(Event::evUpdate);
      //}
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      /*
      if((true == m_CheckLimitsFlag) && ((m_MaxDistanceLimit < m_CurrentDistance) || (m_MinDistanceLimit > m_CurrentDistance))){
        wxLogWarning(std::string("Preload: Distance limit exceeded, current distance: " + std::to_string(m_CurrentDistance)).c_str());
        process(Event::evStop);
      }
      */
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

        // Show a warning pop up dialog if the velocity is high.
        if(11 < m_Velocity){
          if(true == m_MyFrame->showHighVelocityWarningFromExperiments()){
            m_Velocity = 11;
            std::cout << "OneStepEvent: Velocity set to 11." << std::endl;
          }
        }

        wxLogMessage("Preload: Start experiment.");
        //std::cout << "Preload FSM switched to state: runState." << std::endl;
        m_StageFrame->setSpeed(m_Velocity);
        m_CurrentState = runState;
        m_CheckLimitsFlag = true;

        if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){
          //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_StressForceLimit << std::endl;
          m_CurrentDirection = Direction::Backwards;
          m_StageFrame->moveBackward(m_Velocity);
        }else if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){
          //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_StressForceLimit - m_CurrentForce << std::endl;
          m_CurrentDirection = Direction::Forwards;
          m_StageFrame->moveForward(m_Velocity);
        }
      }
      break;
    case runState:
      if(Event::evStop == e){
        wxLogMessage("Preload FSM switched to state: stopState.");

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
        // If force or stress based
        if((DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce)){
          if((m_StressForceLimit - m_CurrentForce) > m_ForceStressThreshold){
            //std::cout << "m_CurrentForce - m_ForceStressLimit: " << m_CurrentForce - m_StressForceLimit << std::endl;

            if((Direction::Forwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){ // Only start motor, if state changed
              m_CurrentDirection = Direction::Backwards;
              m_StageFrame->moveBackward(m_Velocity);
            }
          }else if((m_CurrentForce - m_StressForceLimit) > m_ForceStressThreshold){ // Only reverse motor, if state changed
            //std::cout << "m_ForceStressLimit - m_CurrentForce: " << m_StressForceLimit - m_CurrentForce << std::endl;

            if((Direction::Backwards == m_CurrentDirection) || (Direction::Stop == m_CurrentDirection)){
              m_CurrentDirection = Direction::Forwards;
              m_StageFrame->moveForward(m_Velocity);
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
  m_GageLength = m_DefaultGageLength;
  m_StageFrame->stop();
}
