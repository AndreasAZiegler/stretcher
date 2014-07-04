/**
 * @file experimentvalues.cpp
 * @brief Experiment values base class
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include <fstream>
#include <chrono>
#include <wx/event.h>
#include "../gui/myframe.h"
#include "experimentvalues.h"

/**
 * @brief Initializes all the needed variables.
 * @param stageframe Shared pointer to the stage frame object.
 * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
 * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
 * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
 * @param *vectoraccessmutex Pointer to the graph access mutex.
 * @param *maxlimitgraph Pointer to the maximum limit graph.
 * @param *minlimitgraph Pointer to the minimum limit graph.
 * @param *myframe Pointer to the main frame object.
 * @param experimenttype Experiment type.
 * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
 * @param area Cross section area.
 * @param gagelength The gage length.
 */
ExperimentValues::ExperimentValues(std::shared_ptr<StageFrame> stageframe,
                                   std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                   mpFXYVector *forceStressDistanceGraph,
                                   mpFXYVector *forceStressDisplacementGraph,
                                   std::mutex *vectoraccessmutex,
                                   mpFXYVector *maxlimitgraph,
                                   mpFXYVector *minlimitgraph,
                                   MyFrame *myframe,

                                   ExperimentType experimenttype,
                                   DistanceOrStressOrForce distanceOrStressOrForce,
                                   double area,
                                   long gagelength)
  : m_ExperimentType(experimenttype),
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_ForceStressDistanceGraph(forceStressDistanceGraph),
    m_ForceStressDisplacementGraph(forceStressDisplacementGraph),
    m_VectorLayerMutex(vectoraccessmutex),
    m_MaxLimitVectorLayer(maxlimitgraph),
    m_MinLimitVectorLayer(minlimitgraph),
    m_MyFrame(myframe),
    m_Area(area),
    m_GageLength(gagelength * 0.00009921875/*mm per micro step*/),
    m_CurrentProtocolCycle(0),
    m_ResetProtocolFlag(false),
    m_DisplayGraphDelay(0)
{
}

/**
 * @brief Normalizes the value according to the experiment.
 * @param value The value.
 * @return The normalized value.
 */
double ExperimentValues::normalizeValue(double value){
  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    value /= 10000.0;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    value = (value / 10) / m_Area;
  }else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    value *= 0.00009921875/*mm per micro step*/;
  }
  return(value);
}

/**
 * @brief Registers the update methods to receive the measurement values.
 * @param forcestressvector Vector for the force values.
 * @param distancevector Vector for the distance alues.
 * @param displacementvector Vector for the displacment values
 * @param maxforcelimitvector Vector for the maximum force limit values.
 * @param minforcelimitvector Vector for the minimum force limit values.
 * @param maxdistancelimitvector Vector for the maximum distance limit values.
 * @param mindistancelimitvector Vector for the minimum distance limit values.
 * @param limittimepointvector Vector for the limit timepoints.
 */
void ExperimentValues::startMeasurement(std::shared_ptr<std::vector<double>> forcestressvector,
                                        std::shared_ptr<std::vector<double>> distancevector,
                                        std::shared_ptr<std::vector<double>> displacementvector,
                                        std::shared_ptr<std::vector<double>> maxforcelimitvector,
                                        std::shared_ptr<std::vector<double>> minforcelimitvector,
                                        std::shared_ptr<std::vector<double>> maxdistancelimitvector,
                                        std::shared_ptr<std::vector<double>> mindistancelimitvector,
                                        std::shared_ptr<std::vector<double>> limittimepointvector){
  //std::cout << "Protocol graphstressforce size: " << graphstressforce->size() << " graphdistance size: " << graphdistance->size() << std::endl;
  m_ForceStressGraphValues = std::move(forcestressvector);
  m_DistanceGraphValues = std::move(distancevector);
  m_DisplacementGraphValues = std::move(displacementvector);
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    m_GraphMaxLimitValues = maxforcelimitvector;
    m_GraphMinLimitValues = minforcelimitvector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    m_GraphMaxLimitValues = maxdistancelimitvector;
    m_GraphMinLimitValues = mindistancelimitvector;
  }
  m_GraphLimitTimePoints = limittimepointvector;
  //std::cout << "Protocol m_GraphStressForceValue size: " << m_GraphStressForceValues->size() << " m_GraphDistanceValue size: " << m_GraphDistanceValues->size() << std::endl;

  // Clear the vectors if the protocol is resetted.
  if(true == m_ResetProtocolFlag){
    m_ResetProtocolFlag = false;
    m_CurrentProtocolCycle = 0;
    m_StressForceValues.clear();
    m_DistanceValues.clear();
  }

  std::vector<ExperimentValues::MeasurementValue> vec;
  m_StressForceValues.push_back(vec);
  m_DistanceValues.push_back(vec);

  // Register update method at the message handlers.
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Stops the measurement. Unregisters the update method and increases the current protocol cycle.
 */
void ExperimentValues::stopMeasurement(void){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  m_CurrentProtocolCycle++;
}

/**
 * @brief Reset recorded values, executed from the protocol. Sets the reset protocol flag true.
 */
void ExperimentValues::resetProtocol(void){
  m_ResetProtocolFlag = true;
}

/**
 * @brief Destructor
 */
ExperimentValues::~ExperimentValues(){
  //std::cout << "ExperimentValues destructor finished." << std::endl;
}

/**
 * @brief Method which will be calles by the message handlers to update the values. (CallAfter() asynchronously call the updateGraph method)
 * @param measurementValue Position of linear stage 1 or 2 or the force.
 * @param type Type of the value.
 */
void ExperimentValues::updateValues(UpdatedValues::MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Force:
      if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
        {
          // Add new stress value.
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues[m_CurrentProtocolCycle].push_back(ExperimentValues::MeasurementValue((measurementValue.value / 10.0) / m_Area, measurementValue.timestamp));
          m_ForceStressGraphValues->push_back((measurementValue.value / 10.0) / m_Area);
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
          m_ForceStressGraphValues->push_back(measurementValue.value / 10000.0);
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
        {
          // Add new distance value.
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_DistanceValues[m_CurrentProtocolCycle].push_back(ExperimentValues::MeasurementValue(measurementValue.value * 0.00009921875/*mm per micro step*/, measurementValue.timestamp));
          m_DistanceGraphValues->push_back(measurementValue.value * 0.00009921875/*mm per micro step*/);
          m_DisplacementGraphValues->push_back(measurementValue.value *  0.00009921875/*mm per micro step*/ / m_GageLength);
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
      // Sets data for the graph if the stress/force vector and the distance vector have the same lengths.
      if(m_ForceStressGraphValues->size() == m_DistanceGraphValues->size()){
        std::lock_guard<std::mutex> lck{*m_VectorLayerMutex};
        m_ForceStressDistanceGraph->SetData(*m_DistanceGraphValues, *m_ForceStressGraphValues);
        m_ForceStressDisplacementGraph->SetData(*m_DisplacementGraphValues, *m_ForceStressGraphValues);
      }else{ // Otherwise correct the length.
        //std::cout << "ExperimentValues stress/force: " << m_GraphStressForceValues.size() << " distance: " << m_GraphDistanceValues.size() << std::endl;
        if(m_ForceStressGraphValues->size() > m_DistanceGraphValues->size()){ // Correct the length of the force vector
          //m_GraphStressForceValues->resize(m_GraphDistanceValues->size());
          if(false == m_DistanceGraphValues->empty()){ // Correct the length of the distance/displacement vectors if the vectors aren't empty.
            while(m_ForceStressGraphValues->size() > m_DistanceGraphValues->size()){
              m_DistanceGraphValues->push_back(m_DistanceGraphValues->back());
              m_DisplacementGraphValues->push_back(m_DisplacementGraphValues->back());
            }
          }else{ // Otherwise just resize the force vector.
            m_ForceStressGraphValues->resize(m_DistanceGraphValues->size());
          }
        }else{ // Otherwise correct the length of the distance and displacement vectors.
          m_DistanceGraphValues->resize(m_ForceStressGraphValues->size());
          m_DisplacementGraphValues->resize(m_ForceStressGraphValues->size());
        }
        // Set the graphs.
        m_ForceStressDistanceGraph->SetData(*m_DistanceGraphValues, *m_ForceStressGraphValues);
        m_ForceStressDisplacementGraph->SetData(*m_DisplacementGraphValues, *m_ForceStressGraphValues);
      }
      // Update the graph from the main thread.
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
 * @brief Returns the the measurement type (distance/force/stress).
 * @return The type as std::string.
 */
std::string ExperimentValues::getDistanceOrForceOrStress(void){
  std::string distancestressforce;
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    distancestressforce = "mm";
  } else{
    distancestressforce = getStressOrForce();
  }
  return(distancestressforce);
}
