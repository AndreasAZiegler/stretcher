#include <iostream>
#include <fstream>
#include <chrono>
#include "../gui/myframe.h"
#include "protocols.h"
#include "experiments/preload.h"

Protocols::Protocols(wxListBox *listbox,
                     MyFrame *myframe,
                     bool *stagesstoppedflag,
                     std::mutex *stagesstoppedmutex,
                     std::mutex *waitmutex,
                     std::condition_variable *wait,
                     bool *preloaddoneflag,
                     std::mutex *preloaddonemutex,
                     mpFXYVector *valuesvector,
                     mpFXYVector *stressforcevector,
                     mpFXYVector *distancevector,
                     std::string path)
  : m_ListBox(listbox),
    m_MyFrame(myframe),
    m_StagesStoppedFlag(stagesstoppedflag),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_WaitMutex(waitmutex),
    m_Wait(wait),
    m_PreloadDoneFlag(preloaddoneflag),
    m_PreloadDoneMutex(preloaddonemutex),
    m_ValuesVector(valuesvector),
    m_StressForcePreviewVector(stressforcevector),
    m_DistancePreviewVector(distancevector),
    m_StoragePath(path),
    m_PreloadDistance(0),
    m_ExperimentRunningFlag(false),
    m_MeasurementValuesRecordingFlag(false),
    m_CurrentExperimentNr(0)
{
}

/**
 * @brief Destructor
 */
Protocols::~Protocols(){
  delete m_ExperimentRunningThread;

  /*
  m_Experiments.clear();
  m_ExperimentValues.clear();
  */
}

/**
 * @brief Create the preview vector and display it in the graph.
 */
void Protocols::makePreview(void){

  // Collect the preview points from the single experiments.
  for(int i = 0; i < m_Experiments.size(); ++i){
    m_Experiments[i]->getPreview(m_PreviewValues);
  }

  // Split preview point into stressforce and distance points.
  for(auto i : m_PreviewValues){
    if(Experiment::DistanceOrStressForce::Distance ==  i.distanceOrForce){
      m_DistancePreviewValues.push_back(i.value);
      m_DistanceTimePreviewValues.push_back(i.timepoint);
    } else if(Experiment::DistanceOrStressForce::StressForce ==  i.distanceOrForce){
      m_StressForcePreviewValues.push_back(i.value);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    }
  }

  // Set the the vector data.
  m_StressForcePreviewVector->SetData(m_DistanceTimePreviewValues, m_DistancePreviewValues);
  m_DistancePreviewVector->SetData(m_StressForceTimePreviewValues, m_StressForcePreviewValues);

  // Show preview in the graph.
  m_MyFrame->showPreviewGraph();
}

/**
 * @brief Runs the protocol.
 */
void Protocols::runProtocol(void){
  m_MyFrame->showValuesGraph();

  // Only continue if there are expeiments in the protocol.
  if(0 < m_Experiments.size()){
    // Run first experiment.
    {
      std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
      m_ExperimentRunningFlag = true;
    }
    // Set start point.
    m_StartTimePoint = std::chrono::high_resolution_clock::now();

    // Start recording measurement values.
    {
      std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
      m_MeasurementValuesRecordingFlag = true;
    }
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement();

    m_ListBox->SetSelection(m_CurrentExperimentNr);
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Preload::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.join();
    m_CurrentExperimentNr++;

    if(NULL == m_ExperimentRunningThread){
      delete m_ExperimentRunningThread;
      m_ExperimentRunningThread = NULL;
    }
    m_ExperimentRunningThread = new std::thread(&Protocols::checkFinishedExperiment, this);
    m_ExperimentRunningThread->detach();
  }
}

/**
 * @brief Process the next experiments.
 */
void Protocols::process(void){
  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }
  if(m_Experiments.size() > m_CurrentExperimentNr){
    {
      std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
      m_ExperimentRunningFlag = true;
    }

    // Start recording measurement values.
    {
      std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
      m_MeasurementValuesRecordingFlag = true;
    }
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement();

    m_ListBox->SetSelection(m_CurrentExperimentNr);
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Preload::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.join();
    m_CurrentExperimentNr++;

    if(NULL == m_ExperimentRunningThread){
      delete m_ExperimentRunningThread;
      m_ExperimentRunningThread = NULL;
    }
    m_ExperimentRunningThread = new std::thread(&Protocols::checkFinishedExperiment, this);
    m_ExperimentRunningThread->detach();

  } else{
    m_CurrentExperimentNr = 0;
  }
}

/**
 * @brief Stops the protocol.
 */
void Protocols::stopProtocol(void){
  m_CurrentExperimentNr = 0;
}

/**
 * @brief Moves the experiment from the current position to the one before.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentUp(int experimentPosition){
  // Check if the poition is not the first position.
  if(0 < experimentPosition){
    // Swap the elements in the vector.
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition - 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition - 1]);

    // Swap the elements in the list box.
    const wxString stringPos(m_ListBox->GetString(experimentPosition));
    const wxString stringUp(m_ListBox->GetString(experimentPosition - 1));
    m_ListBox->SetString(experimentPosition, stringUp);
    m_ListBox->SetString(experimentPosition - 1, stringPos);

    // Swap the selection in the list box.
    m_ListBox->SetSelection(experimentPosition, false);
    m_ListBox->SetSelection(experimentPosition - 1, true);
  }
}

/**
 * @brief Moves the experiment from the current position to the one after.
 * @param experimentPosition The current experiment position.
 */
void Protocols::moveExperimentDown(int experimentPosition){
  // Check if the poition is not the last position.
  if(m_Experiments.size() > experimentPosition){
    // Swap the elements in the vector.
    std::swap(m_Experiments[experimentPosition], m_Experiments[experimentPosition + 1]);
    std::swap(m_ExperimentValues[experimentPosition], m_ExperimentValues[experimentPosition + 1]);

    // Swap the elements in the list box.
    const wxString stringPos(m_ListBox->GetString(experimentPosition));
    const wxString stringDown(m_ListBox->GetString(experimentPosition + 1));
    m_ListBox->SetString(experimentPosition, stringDown);
    m_ListBox->SetString(experimentPosition + 1, stringPos);

    // Swap the selection in the list box.
    m_ListBox->SetSelection(experimentPosition, false);
    m_ListBox->SetSelection(experimentPosition + 1, true);
  }
}

/**
 * @brief Adds an experiment.
 * @param experiment Pointer to the experiment object.
 */
void Protocols::addExperiment(std::unique_ptr<Experiment> &experiment){
  m_Experiments.push_back(std::move(experiment));
  // Add experiment value object.
  m_ExperimentValues.push_back(m_Experiments.back()->getExperimentValues());

  // Add string in list box.
  const wxString tmp((m_ExperimentValues.back())->experimentTypeToString());
  m_ListBox->Append(tmp);
}

/**
 * @brief Removes the Experiment at the desired position.
 * @param experimentPosition Position of the experiment.
 */
void Protocols::removeExperiment(int experimentPosition){
  m_Experiments[experimentPosition].reset();
  m_Experiments.erase(m_Experiments.begin() + experimentPosition);
  m_ExperimentValues[experimentPosition].reset();
  m_ExperimentValues.erase(m_ExperimentValues.begin() + experimentPosition);

  m_ListBox->Delete(experimentPosition);
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
void Protocols::exportCSV(std::vector<bool> *disableexport){
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
    if(false == disableexport->operator [](i)){
      //file << m_ExperimentValues[i]->getExperimentSettings();
    }
  }

  file << std::endl << std::endl;

  file << "Distance in mm; Time stamp for the distance in milli seconds; Stress/Force in " << m_ExperimentValues[0]->getStressOrForce() << "; Time stamp for stress/force in micro seconds" << std::endl;

  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    if(false == disableexport->operator [](i)){

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

  delete disableexport;
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


/**
 * @brief Sets the m_ExperimentRunningFlag false if experiment is finished and the stages stopped and record preload distance if a preloading happend.
 */
void Protocols::checkFinishedExperiment(void){
  {
    // Wait until experiment is finised.
    std::unique_lock<std::mutex> lck1(*m_WaitMutex);
    m_Wait->wait(lck1);
  }
  {
    // Indicate that the experiment is not longer running.
    std::lock_guard<std::mutex> lck4{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = false;
  }
  {
    // If preloading is active.
    std::lock_guard<std::mutex> lck2{*m_PreloadDoneMutex};
    if(false == *m_PreloadDoneFlag){
      // Wait until the stages stopped.
      {
        bool tmp = false;
        while(false == tmp){
          std::unique_lock<std::mutex> lck3(*m_StagesStoppedMutex);
          tmp = *m_StagesStoppedFlag;
        }
      }

      *m_PreloadDoneFlag = true;
      // Set preload distance.
      m_PreloadDistance = m_MyFrame->getCurrentDistance();
      std::cout << "m_PreloadDistance: " << m_PreloadDistance << std::endl;
      // Set the prelod distance in all the experiments.
      for(auto i : m_Experiments){
        i->setPreloadDistance(m_PreloadDistance);
      }
    }
  }
  {
    // Stop the recording of the measurement values if it is running.
    std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
    if(true == m_MeasurementValuesRecordingFlag){
      m_MeasurementValuesRecordingFlag = false;
      //m_CurrentExperimentValues->stopMeasurement();
      m_ExperimentValues[m_CurrentExperimentNr-1]->stopMeasurement();
    }
  }
  process();
  //delete m_CurrentExperiment;
  //m_CurrentExperiment = NULL;
}

/**
 * @brief Executed by the object main frame when the clear graph button is pressed. Stops the measurement.
 */
void Protocols::clearGraphStop(void){
  // Stop the recording of the measurement values if it is running.
  std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
  if(true == m_MeasurementValuesRecordingFlag){
    m_MeasurementValuesRecordingFlag = false;
    //m_CurrentExperimentValues->stopMeasurement();
    m_ExperimentValues[m_CurrentExperimentNr-1]->stopMeasurement();
  }
}
