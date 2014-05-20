#include <iostream>
#include <fstream>
#include <chrono>
#include <wx/event.h>
#include "../gui/myframe.h"
#include "experimentvalues.h"

/**
 * @brief Initializes all the needed variables.
 * @param stressOrForce Indicates if the experiment is stress or force based.
 * @param stageframe Pointer to the stage frame object.
 * @param forcesensormessagehandler Pointer to the force sensor message handler.
 * @param graph Pointer to the graph object.
 * @param diameter The diameter of the sample.
 */
ExperimentValues::ExperimentValues(StageFrame *stageframe,
                                   ForceSensorMessageHandler *forcesensormessagehandler,
                                   mpFXYVector *vector,
                                   std::mutex *vectoraccessmutex,
                                   MyFrame *myframe,

                                   ExperimentType experimenttype,
                                   DistanceOrStressOrForce distanceOrStressOrForce,
                                   double area)
  : m_ExperimentType(experimenttype),
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_VectorLayer(vector),
    m_VectorLayerMutex(vectoraccessmutex),
    m_MyFrame(myframe),
    m_Area(area * 0.000000000001/*um^2*/),
    m_DisplayGraphDelay(0)
{
}

/*
ExperimentValues::ExperimentValues(const ExperimentValues &experimentvalues)
  : m_ExperimentType(experimentvalues.m_ExperimentType),
    m_StressOrForce(experimentvalues.m_StressOrForce),
    m_StageFrame(experimentvalues.m_StageFrame),
    m_ForceSensorMessageHandler(experimentvalues.m_ForceSensorMessageHandler),
    m_VectorLayer(experimentvalues.m_VectorLayer),
    m_VectorLayerMutex(experimentvalues.m_VectorLayerMutex),
    m_MyFrame(experimentvalues.m_MyFrame),
    m_Area(experimentvalues.m_Area),
    m_DisplayGraphDelay(experimentvalues.m_DisplayGraphDelay)
{}
*/

/**
 * @brief Registers the update methods to receive the measurement values.
 */
void ExperimentValues::startMeasurement(std::shared_ptr<std::vector<double>> graphstressforce, std::shared_ptr<std::vector<double>> graphdistance){
  //std::cout << "Protocol graphstressforce size: " << graphstressforce->size() << " graphdistance size: " << graphdistance->size() << std::endl;
  m_GraphStressForceValues = std::move(graphstressforce);
  m_GraphDistanceValues = std::move(graphdistance);
  //std::cout << "Protocol m_GraphStressForceValue size: " << m_GraphStressForceValues->size() << " m_GraphDistanceValue size: " << m_GraphDistanceValues->size() << std::endl;
  // clear the vectors.
  m_StressForceValues.clear();
  //m_GraphStressForceValues.clear();
  m_DistanceValues.clear();
  //m_GraphDistanceValues.clear();

  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Unregister the update method.
 */
void ExperimentValues::stopMeasurement(void){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);

  m_GraphStressForceValues = NULL;
  m_GraphDistanceValues = NULL;
}

/**
 * @brief Removes the current graph.
 * @todo Remove method.
 */
void ExperimentValues::removeGraph(void){
}

/**
 * @brief Destructor
 */
ExperimentValues::~ExperimentValues(){
  std::cout << "ExperimentValues destructor finished." << std::endl;
}

/**
 * @brief Method which will be calles by the message handlers to update the values. (CallAfter() asynchronously call the updateGraph method)
 * @param value Position of linear stage 1 or 2 or the force.
 * @param type Type of value.
 */
void ExperimentValues::updateValues(UpdatedValues::MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back(ExperimentValues::MeasurementValue((measurementValue.value / 10000.0) / m_Area, measurementValue.timestamp));
          m_GraphStressForceValues->push_back((measurementValue.value / 10000.0) / m_Area);
        }
      }else{
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back(ExperimentValues::MeasurementValue(measurementValue.value / 10000.0, measurementValue.timestamp));
          m_GraphStressForceValues->push_back(measurementValue.value / 10000.0);
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_DistanceValues.push_back(ExperimentValues::MeasurementValue(measurementValue.value * 0.00009921875/*mm per micro step*/, measurementValue.timestamp));
          m_GraphDistanceValues->push_back(measurementValue.value * 0.00009921875/*mm per micro step*/);
        }
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  {
    m_DisplayGraphDelay++;
    if(5 <= m_DisplayGraphDelay){
      m_DisplayGraphDelay = 0;

      std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
      if(m_GraphStressForceValues->size() == m_GraphDistanceValues->size()){
        std::lock_guard<std::mutex> lck{*m_VectorLayerMutex};
        m_VectorLayer->SetData(*m_GraphDistanceValues, *m_GraphStressForceValues);
      }else{
        //std::cout << "ExperimentValues stress/force: " << m_GraphStressForceValues.size() << " distance: " << m_GraphDistanceValues.size() << std::endl;
        if(m_GraphStressForceValues->size() > m_GraphDistanceValues->size()){
          m_GraphStressForceValues->resize(m_GraphDistanceValues->size());
        }else{
          m_GraphDistanceValues->resize(m_GraphStressForceValues->size());
        }
        m_VectorLayer->SetData(*m_GraphDistanceValues, *m_GraphStressForceValues);
      }
      m_MyFrame->updateGraphFromExperimentValues();
    }
  }
}

/**
 * @brief Returns a pointer to the vector containing the stress/force values.
 * @return Pointer to the vector.
 */
std::vector<ExperimentValues::MeasurementValue>* ExperimentValues::getStressForceValues(void){
  return(&m_StressForceValues);
}

/**
 * @brief Returns a pointer to the vector containing the distance values.
 * @return Pointer to the vector.
 */
std::vector<ExperimentValues::MeasurementValue>* ExperimentValues::getDistanceValues(void){
  return(&m_DistanceValues);
}

/**
 * @brief Returns the current experiment type as a string.
 * @return The current experiment type as a string.
 */
std::string ExperimentValues::experimentTypeToString(){
  switch(m_ExperimentType){
    case ExperimentType::Preload:
      return("Preload");
      break;

    case ExperimentType::OneStepEvent:
      return("OneStepEvent");
      break;

    case ExperimentType::ContinuousEvent:
      return("ContinuousEvent");
      break;

    case ExperimentType::Conditioning:
      return("Conditioning");
      break;

    case ExperimentType::Ramp2Failure:
      return("Ramp2Failure");
      break;
  }
}

/**
 * @brief Export the measurement unit (stress/force)
 * @return The unit as std::string.
 */
std::string ExperimentValues::getStressOrForce(void){
  std::string stressforce;
  if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    stressforce = "kPa";
  }else{
    stressforce = "N";
  }
  return(stressforce);
}

/**
 * @brief Returns the the measurement type (distance/stressForce).
 * @return The type as std::string.
 */
std::string ExperimentValues::getDistanceOrStressForce(void){
  std::string distancestressforce;
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    distancestressforce = "mm";
  } else{
    distancestressforce = getStressOrForce();
  }
  return(distancestressforce);
}
