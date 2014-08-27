/**
 * @file preload.cpp
 * @brief Preload experiment
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <mutex>
#include <wx/log.h>
#include "../gui/myframe.h"
#include "pugixml/pugixml.hpp"
#include "preload.h"

/**
 * @brief Initializes all the needed variables and registers the update method at the message handelers.
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
Preload::Preload(ExperimentParameters experimentparameters,

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
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,

                 PreloadParameters parameters)
  : Experiment(experimentparameters,
               forcestressthreshold,
               distancethreshold),
               //0.005 * 10000.0/*stress force threshold*/,
               //0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
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
                                                       maxforcelimitvector,
                                                       minforcelimitvector,
                                                       maxdistancelimitvector,
                                                       mindistancelimitvector,

                                                       experimentparameters.myframe,

                                                       experimentparameters.type,
                                                       experimentparameters.distanceOrForceOrStress,
                                                       experimentparameters.area,
                                                       experimentparameters.gagelength,

                                                       parameters.stressForceLimit,
                                                       parameters.velocity))
{
  // Registers the update method at the message handlers.
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  // Calculates the limit.
  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    m_StressForceLimit = m_InitStressForceLimit * 10000.0;
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    m_StressForceLimit = m_InitStressForceLimit * m_Area * 10;
  }
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void Preload::setParameters(PreloadParameters parameters){

  m_DistanceOrForceOrStress = parameters.distanceOrForceOrStress;
  m_InitStressForceLimit = parameters.stressForceLimit;
  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    m_StressForceLimit = m_InitStressForceLimit * 10000.0;
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    m_StressForceLimit = m_InitStressForceLimit * m_Area * 10;
  }
  m_Velocity = parameters.velocity;

  // Updates the parameters in the experiment values.
  m_ExperimentValues->setParameters(parameters);
}

/**
 * @brief Sets the preload distance.
 * @param preloaddistance Preload distance
 */
void Preload::setPreloadDistance(void){

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

  parameters.distanceOrForceOrStress = m_DistanceOrForceOrStress;

  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    parameters.stressForceLimit = m_InitStressForceLimit;
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
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

  node.append_attribute("StressOrForce") = static_cast<int>(m_DistanceOrForceOrStress);
  node.append_attribute("CrossSectionArea") = m_Area;
  node.append_attribute("ForceOrStress") = static_cast<int>(m_DistanceOrForceOrStress);

  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    node.append_attribute("ForceStressLimit") = m_InitStressForceLimit;
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
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
  previewvalue.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_StressForceLimit));
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
      process(Event::evUpdate);
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
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
void Preload::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){

        // Show a warning pop up dialog if the velocity is high.
        if(11 < m_Velocity){
          if(true == m_MyFrame->showHighVelocityWarningFromExperiments()){
            m_Velocity = 11;
            wxLogMessage("Preload: Velocity set to 11 mm/s.");
          }
        }

        wxLogMessage("Preload: Start");
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
      if(Event::evStop == event){

        // Stop stage.
        m_CurrentState = stopState;
        m_CurrentDirection = Direction::Stop;
        {
          std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
          *m_StagesStoppedFlag = false;
        }
        m_StageFrame->stop();

        wxLogMessage("Preload: Stop");

        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      if(Event::evUpdate == event){
        // If force or stress based
        if((DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress) || (DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress)){
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
              // Stop stage.
              m_CurrentState = stopState;
              m_CurrentDirection = Direction::Stop;
              {
                std::unique_lock<std::mutex> lck(*m_StagesStoppedMutex);
                *m_StagesStoppedFlag = false;
              }
              m_StageFrame->stop();

              wxLogMessage("Preload: Stop");

              // Notify that the experiment finished.
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
