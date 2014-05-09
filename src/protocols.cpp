#include <iostream>
#include <fstream>
#include <chrono>
#include "protocols.h"

Protocols::Protocols(wxListBox *listbox,
                     MyFrame *myframe,
                     mpFXYVector *vector,
                     std::string path)
  : m_ListBox(listbox),
    m_MyFrame(myframe),
    m_Vector(vector),
    m_StoragePath(path)
{
}

/**
 * @brief Moves the experiment from the current position to the one before.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentUp(int experimentPosition){
  // Check if the poition is not the first position.
  if(0 < experimentPosition){
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition - 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition - 1]);
  }
}

/**
 * @brief Moves the experiment from the current position to the one after.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentDown(int experimentPosition){
  // Check if the poition is not the last position.
  if(m_Experiments.size() > experimentPosition){
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition + 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition + 1]);
  }
}

/**
 * @brief Sets the experiment start time point.
 */
void Protocols::setStartPoint(void){
  m_StartTimePoint = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Exports the measurement data to a .csv file.
 */
void Protocols::exportCSV(void){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));
  std::cout << mbstr << std::endl;
  //std::string pathAndFilename = m_StoragePath + "/" + experimentTypeToString() + "_" + std::string(mbstr) + ".txt";
  std::string pathAndFilename = m_StoragePath + "/" + "_" + std::string(mbstr) + ".txt";
  std::cout << pathAndFilename << std::endl;

  // Creat the file
  std::ofstream file(pathAndFilename);
  //std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  // Open the file
  if(false == file.is_open()){
    std::cerr << "Couldn't open file" << std::endl;
    throw "Couldn't open file";
  }

  // Printing header
  file << "Protocol: " <<  "Date/Time: " << std::string(mbstr) << std::endl << std::endl;

  // Printing the experiment settings.
  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    if(false == m_ExperimentValues[i]->isExportDisabled()){
      //file << m_ExperimentValues[i]->getExperimentSettings();
    }
  }

  file << std::endl << std::endl;

  file << "Distance in mm; Time stamp for the distance in milli seconds; Stress/Force in " << m_ExperimentValues[0]->getStressOrForce() << "; Time stamp for stress/force in micro seconds" << std::endl;

  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    if(false == m_ExperimentValues[i]->isExportDisabled()){

      // Get the pointer to the vectors containing the measurement values.
      std::vector<ExperimentValues::MeasurementValue>* stressForceValues = m_ExperimentValues[i]->getStressForceValues();
      std::vector<ExperimentValues::MeasurementValue>* distanceValues = m_ExperimentValues[i]->getDistanceValues();

      // Correct the vector size if needed.
      if(stressForceValues->size() > distanceValues->size()){
        stressForceValues->resize(distanceValues->size());
      }else{
        distanceValues->resize(stressForceValues->size());
      }

      // Print the measured values.
      for(int i = 0; i < stressForceValues->size(); ++i){
        file << distanceValues->operator [](i).value << std::string(";")
             << std::chrono::duration_cast<std::chrono::milliseconds>(distanceValues->operator [](i).timestamp - m_StartTimePoint).count() << ";"
             << stressForceValues->operator [](i).value << ";"
             << std::chrono::duration_cast<std::chrono::milliseconds>(stressForceValues->operator [](i).timestamp - m_StartTimePoint).count() << std::endl;
      }
    }
  }

  file.close();
  /*
/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
  /*
std::string ExperimentValues::getExperimentSettings(void){
  std::string str;
  str = "Experiment: " + experimentTypeToString() + "\n";

}

  // Correct the vector size if needed.
  if(m_StressForceValues.size() > m_DistanceValues.size()){
    m_StressForceValues.resize(m_DistanceValues.size());
  }else{
    m_DistanceValues.resize(m_StressForceValues.size());
  }

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
  */
}
