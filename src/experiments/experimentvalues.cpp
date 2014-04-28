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
ExperimentValues::ExperimentValues(ExperimentType experimenttype,
                                   StressOrForce stressOrForce,
                                   StageFrame *stageframe,
                                   ForceSensorMessageHandler *forcesensormessagehandler,
                                   mpFXYVector *vector,
                                   std::mutex *vectoraccessmutex,
                                   MyFrame *myframe,
                                   std::string path,
                                   double diameter)
  : m_ExperimentType(experimenttype),
    m_StressOrForce(stressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_VectorLayer(vector),
    m_VectorLayerMutex(vectoraccessmutex),
    m_MyFrame(myframe),
    m_StoragePath(path),
    m_Diameter(diameter),
    m_DisplayGraphDelay(0)
{
}

/**
 * @brief Registers the update methods to receive the measurement values.
 */
void ExperimentValues::startMeasurement(void){
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Sets the experiment start time point.
 */
void ExperimentValues::setStartPoint(void){
  m_StartTimePoint = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Unregister the update method.
 */
void ExperimentValues::stopMeasurement(void){
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Removes the current graph.
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
      if(StressOrForce::Stress == m_StressOrForce){
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back(ExperimentValues::MeasurementValue((measurementValue.value / 10000.0) / m_Diameter, measurementValue.timestamp));
          m_GraphStressForceValues.push_back((measurementValue.value / 10000.0) / m_Diameter);
        }
      }else{
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_StressForceValues.push_back(ExperimentValues::MeasurementValue(measurementValue.value / 10000.0, measurementValue.timestamp));
          m_GraphStressForceValues.push_back(measurementValue.value / 10000.0);
        }
      }
      break;

    case UpdatedValuesReceiver::ValueType::Distance:
        {
          std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
          m_DistanceValues.push_back(ExperimentValues::MeasurementValue(measurementValue.value * 0.00009921875/*mm per micro step*/, measurementValue.timestamp));
          m_GraphDistanceValues.push_back(measurementValue.value * 0.00009921875/*mm per micro step*/);
        }
      //std::cout << "Conditioning distance update." << std::endl;
      break;
  }

  {
    m_DisplayGraphDelay++;
    if(5 <= m_DisplayGraphDelay){
      m_DisplayGraphDelay = 0;

      std::lock_guard<std::mutex> lck{m_AccessValuesMutex};
      if(m_GraphStressForceValues.size() == m_GraphDistanceValues.size()){
        std::lock_guard<std::mutex> lck{*m_VectorLayerMutex};
        m_VectorLayer->SetData(m_GraphDistanceValues, m_GraphStressForceValues);
      }else{
        //std::cout << "ExperimentValues stress/force: " << m_GraphStressForceValues.size() << " distance: " << m_GraphDistanceValues.size() << std::endl;
        if(m_GraphStressForceValues.size() > m_GraphDistanceValues.size()){
          m_GraphStressForceValues.resize(m_GraphDistanceValues.size());
        }else{
          m_GraphDistanceValues.resize(m_GraphStressForceValues.size());
        }
        m_VectorLayer->SetData(m_GraphDistanceValues, m_GraphStressForceValues);
      }
      m_MyFrame->updateGraphFromExperimentValues();
    }
  }
}

/**
 * @brief Exports the measurement data to a .csv file.
 */
void ExperimentValues::exportCSV(void){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));
  std::cout << mbstr << std::endl;
  std::string pathAndFilename = m_StoragePath + "/" + experimentTypeToString() + "_" + std::string(mbstr) + ".txt";
  std::cout << pathAndFilename << std::endl;

  // Correct the vector size if needed.
  if(m_StressForceValues.size() > m_DistanceValues.size()){
    m_StressForceValues.resize(m_DistanceValues.size());
  }else{
    m_DistanceValues.resize(m_StressForceValues.size());
  }

  std::ofstream file(pathAndFilename);
  //std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  if(false == file.is_open()){
    std::cerr << "Couldn't open file" << std::endl;
    throw "Couldn't open file";
  }
  file << "Experiment: " <<  experimentTypeToString() << "Date/Time: " << std::string(mbstr) << std::endl << std::endl;

  std::string stressforce;
  if(StressOrForce::Stress == m_StressOrForce){
    stressforce = "kPa";
  }else{
    stressforce = "N";
  }

  file << "Distance in mm; Time stamp for the distance in milli seconds; Stress/Force in " << stressforce << "; Time stamp for stress/force in micro seconds" << std::endl;


  for(int i = 0; i < m_StressForceValues.size(); ++i){
    file << m_DistanceValues[i].value << std::string(";")
         << std::chrono::duration_cast<std::chrono::milliseconds>(m_DistanceValues[i].timestamp - m_StartTimePoint).count() << ";"
         << m_StressForceValues[i].value << ";"
         << std::chrono::duration_cast<std::chrono::milliseconds>(m_StressForceValues[i].timestamp - m_StartTimePoint).count() << std::endl;
  }

  file.close();
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

    case ExperimentType::Conditioning:
      return("Conditioning");
      break;

    case ExperimentType::Ramp2Failure:
      return("Ramp2Failure");
      break;

    case ExperimentType::Relaxation:
      return("Relaxation");
      break;

    case ExperimentType::Creep:
      return("Creep");
      break;

    case ExperimentType::FatigueTesting:
      return("FatigueTesting");
      break;

    case ExperimentType::ChamberStretchCells:
      return("ChamberStretch Cells");
      break;

    case ExperimentType::ChamberStretchGel:
      return("ChamberStretch Gel");
      break;
  }

}
