#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
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
ExperimentValues::ExperimentValues(std::shared_ptr<StageFrame> stageframe,
                                   std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                   mpFXYVector *vector,
                                   std::mutex *vectoraccessmutex,
                                   mpFXYVector *maxforcelimitvector,
                                   mpFXYVector *minforcelimitvector,
                                   mpFXYVector *maxdistancelimitvector,
                                   mpFXYVector *mindistancelimitvector,
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
    m_MaxForceLimitVectorLayer(maxforcelimitvector),
    m_MinForceLimitVectorLayer(minforcelimitvector),
    m_MaxDistanceLimitVectorLayer(maxdistancelimitvector),
    m_MinDistanceLimitVectorLayer(mindistancelimitvector),
    m_MyFrame(myframe),
    m_Area(area),
    m_CurrentProtocolCycle(0),
    m_ResetProtocolFlag(false),
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
void ExperimentValues::startMeasurement(std::shared_ptr<std::vector<double>> graphstressforce,
                                        std::shared_ptr<std::vector<double>> graphdistance,
                                        std::shared_ptr<std::vector<double>> graphmaxforcelimitvalues,
                                        std::shared_ptr<std::vector<double>> graphminforcelimitvalues,
                                        std::shared_ptr<std::vector<double>> graphmaxdistancelimitvalues,
                                        std::shared_ptr<std::vector<double>> graphmindistancelimitvalues,
                                        std::shared_ptr<std::vector<double>> graphforcelimittimepoints,
                                        std::shared_ptr<std::vector<double>> graphdistancelimittimepoints){
  //std::cout << "Protocol graphstressforce size: " << graphstressforce->size() << " graphdistance size: " << graphdistance->size() << std::endl;
  m_GraphStressForceValues = std::move(graphstressforce);
  m_GraphDistanceValues = std::move(graphdistance);

  m_GraphMaxForceLimitValues = graphmaxforcelimitvalues;
  m_GraphMinForceLimitValues = graphminforcelimitvalues;
  m_GraphMaxDistanceLimitValues = graphmaxdistancelimitvalues;
  m_GraphMinDistanceLimitValues = graphmindistancelimitvalues;

  m_GraphForceLimitXAxisPoints = graphforcelimittimepoints;
  m_GraphDistanceLimitYAxisPoints = graphdistancelimittimepoints;
  //std::cout << "Protocol m_GraphStressForceValue size: " << m_GraphStressForceValues->size() << " m_GraphDistanceValue size: " << m_GraphDistanceValues->size() << std::endl;
  // clear the vectors.
  //m_GraphStressForceValues.clear();
  //m_GraphDistanceValues.clear();

  /*
  m_GraphForceLimitXAxisPoints->clear();
  m_GraphMaxLimitValues->resize(1);
  m_GraphMinLimitValues->resize(1);
  */
  if(true == m_ResetProtocolFlag){
    m_ResetProtocolFlag = false;
    m_CurrentProtocolCycle = 0;
    m_StressForceValues.clear();
    m_DistanceValues.clear();
  }

  std::vector<ExperimentValues::MeasurementValue> vec;
  m_StressForceValues.push_back(vec);
  m_DistanceValues.push_back(vec);

  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Unregister the update method.
 */
void ExperimentValues::stopMeasurement(void){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  m_CurrentProtocolCycle++;
}

/**
 * @brief Reset recorded values, executed from the protocol.
 */
void ExperimentValues::resetProtocol(void){
  m_ResetProtocolFlag = true;
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
          // Add new stress value.
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues[m_CurrentProtocolCycle].push_back(ExperimentValues::MeasurementValue((measurementValue.value / 10.0) / m_Area, measurementValue.timestamp));
          m_GraphStressForceValues->push_back((measurementValue.value / 10.0) / m_Area);
          /*
          wxLogMessage(std::string("ExperimentValues: Value: " + std::to_string((measurementValue.value / 10.0) / m_Area) +
                                   " value: " + std::to_string(measurementValue.value) +
                                   " m_Area: " + std::to_string(m_Area)).c_str());
          */
        }
      }else{
        {
          // Add new force value.
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues[m_CurrentProtocolCycle].push_back(ExperimentValues::MeasurementValue(measurementValue.value / 10000.0, measurementValue.timestamp));
          m_GraphStressForceValues->push_back(measurementValue.value / 10000.0);
        }
      }

      // Update the max distance values, if the range changed.
      if((*std::max_element(m_GraphDistanceLimitYAxisPoints->begin(), m_GraphDistanceLimitYAxisPoints->end()) < m_GraphStressForceValues->back()) ||
         (*std::min_element(m_GraphDistanceLimitYAxisPoints->begin(), m_GraphDistanceLimitYAxisPoints->end()) > m_GraphStressForceValues->back())){
        m_GraphDistanceLimitYAxisPoints->push_back(m_GraphStressForceValues->back());
        m_GraphMaxDistanceLimitValues->push_back(m_GraphMaxDistanceLimitValues->back());
        m_GraphMinDistanceLimitValues->push_back(m_GraphMinDistanceLimitValues->back());
        /*
        wxLogMessage(std::string("ExperimentValues: m_GraphDistanceLimitYAxisPoints: " + std::to_string(m_GraphDistanceLimitYAxisPoints->size()) +
                                 " m_GraphMaxDistanceLimitValues: " + std::to_string(m_GraphMaxDistanceLimitValues->size())).c_str());
        wxLogMessage(std::string("ExperimentalValues: m_GraphStressForceValues->back(): " + std::to_string(m_GraphStressForceValues->back()) +
                                 " m_GraphMaxDistanceLimitValues->back(): " + std::to_string(m_GraphMaxDistanceLimitValues->back()) +
                                 " m_GraphMinDistanceLimitValues->back(): " + std::to_string(m_GraphMinDistanceLimitValues->back())).c_str());
        */
        m_MaxDistanceLimitVectorLayer->SetData(*m_GraphMaxDistanceLimitValues, *m_GraphDistanceLimitYAxisPoints);
        m_MinDistanceLimitVectorLayer->SetData(*m_GraphMinDistanceLimitValues, *m_GraphDistanceLimitYAxisPoints);
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
      {
        // Add new distance value.
        std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
        m_DistanceValues[m_CurrentProtocolCycle].push_back(ExperimentValues::MeasurementValue(measurementValue.value * 0.00009921875/*mm per micro step*/, measurementValue.timestamp));
        m_GraphDistanceValues->push_back(measurementValue.value * 0.00009921875/*mm per micro step*/);
      }

      // Update the max force values, if the range changed.
      if((*std::max_element(m_GraphForceLimitXAxisPoints->begin(), m_GraphForceLimitXAxisPoints->end()) < m_GraphDistanceValues->back()) ||
         (*std::min_element(m_GraphForceLimitXAxisPoints->begin(), m_GraphForceLimitXAxisPoints->end()) > m_GraphDistanceValues->back())){
        m_GraphForceLimitXAxisPoints->push_back(m_GraphDistanceValues->back());
        m_GraphMaxForceLimitValues->push_back(m_GraphMaxForceLimitValues->back());
        m_GraphMinForceLimitValues->push_back(m_GraphMinForceLimitValues->back());
        //wxLogMessage(std::string("ExperimentValues: m_GraphLimitTimePoints: " + std::to_string(m_GraphForceLimitXAxisPoints->size()) + " m_GraphMaxLimitValues: " + std::to_string(m_GraphMaxForceLimitValues->size())).c_str());
        m_MaxForceLimitVectorLayer->SetData(*m_GraphForceLimitXAxisPoints, *m_GraphMaxForceLimitValues);
        m_MinForceLimitVectorLayer->SetData(*m_GraphForceLimitXAxisPoints, *m_GraphMinForceLimitValues);
      }
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  {
    // Update graph only every 5th update.
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
std::vector<std::vector<ExperimentValues::MeasurementValue>>* ExperimentValues::getStressForceValues(void){
  return(&m_StressForceValues);
}

/**
 * @brief Returns a pointer to the vector containing the distance values.
 * @return Pointer to the vector.
 */
std::vector<std::vector<ExperimentValues::MeasurementValue>>* ExperimentValues::getDistanceValues(void){
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

    case ExperimentType::Pause:
      return("Pause");
      break;

    case ExperimentType::PauseResume:
      return("PauseResume");
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
