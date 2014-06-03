// Includes
#include <iostream>
#include <fstream>
#include <chrono>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "../gui/myframe.h"
#include "protocols.h"
#include "experiments/preload.h"

// An deleter which doesn't do anything, required for passing shared_ptr.
void do_nothing_deleter(std::vector<double> *){return;}

Protocols::Protocols(wxListBox *listbox,
                     MyFrame *myframe,
                     bool *stagesstoppedflag,
                     std::mutex *stagesstoppedmutex,
                     std::mutex *waitmutex,
                     std::condition_variable *wait,
                     bool *preloaddoneflag,
                     std::mutex *preloaddonemutex,
                     bool loopflag,
                     long maxdistance,
                     long mindistance,
                     long maxforce,
                     long minforce,
                     mpFXYVector *valuesvector,
                     mpFXYVector *stressforcevector,
                     mpFXYVector *distancevector,
                     mpFXYVector *maxstressforcelimitvector,
                     mpFXYVector *minstressforcelimitvector,
                     mpFXYVector *maxdistancelimitvector,
                     mpFXYVector *mindistancelimitvector,
                     std::string path)
  : m_ListBox(listbox),
    m_MyFrame(myframe),
    m_StagesStoppedFlag(stagesstoppedflag),
    m_StagesStoppedMutex(stagesstoppedmutex),
    m_WaitMutex(waitmutex),
    m_Wait(wait),
    m_LoopProtocolFlag(loopflag),
    m_StopProtocolFlag(false),
    m_PreloadDoneFlag(preloaddoneflag),
    m_PreloadDoneMutex(preloaddonemutex),
    m_MaxDistanceLimit(maxdistance),
    m_MinDistanceLimit(mindistance),
    m_MaxForceLimit(maxforce),
    m_MinForceLimit(minforce),
    m_ValuesVector(valuesvector),
    m_StressForcePreviewVector(stressforcevector),
    m_DistancePreviewVector(distancevector),
    m_MaxStressForceLimitVector(maxstressforcelimitvector),
    m_MinStressForceLimitVector(minstressforcelimitvector),
    m_MaxDistanceLimitVector(maxdistancelimitvector),
    m_MinDistanceLimitVector(mindistancelimitvector),
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
  /*
  m_Experiments.clear();
  m_ExperimentValues.clear();
  */
}

/**
 * @brief Create the preview vector and display it in the graph.
 */
void Protocols::makePreview(void){
  m_PreviewValues.clear();
  m_PreviewStressForceLimitTimePoints.clear();
  m_MaxStressForceLimits.clear();
  m_MinStressForceLimits.clear();
  m_MaxDistanceLimits.clear();
  m_MinDistanceLimits.clear();

  getPreviewValues();

  // Create limit vectors
  m_PreviewStressForceLimitTimePoints.push_back(m_PreviewValues.front().timepoint);
  m_PreviewStressForceLimitTimePoints.push_back(m_PreviewValues.back().timepoint);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit);
  m_MinStressForceLimits.push_back(m_MinForceLimit);
  m_MinStressForceLimits.push_back(m_MinForceLimit);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit);

  // Set the the vector data.
  m_DistancePreviewVector->SetData(m_DistanceTimePreviewValues, m_DistancePreviewValues);
  m_StressForcePreviewVector->SetData(m_StressForceTimePreviewValues, m_StressForcePreviewValues);
  m_MaxStressForceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MaxStressForceLimits);
  m_MinStressForceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MinStressForceLimits);
  m_MaxDistanceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MaxDistanceLimits);
  m_MinDistanceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MinDistanceLimits);

  // Show preview in the graph.
  m_MyFrame->showPreviewGraph();
}

/**
 * @brief Get the preview values.
 */
void Protocols::getPreviewValues(void){
  m_DistancePreviewValues.clear();
  m_DistanceTimePreviewValues.clear();
  m_StressForcePreviewValues.clear();
  m_StressForceTimePreviewValues.clear();

  // Collect the preview points from the single experiments.
  for(int i = 0; i < m_Experiments.size(); ++i){
    m_Experiments[i]->getPreview(m_PreviewValues);
  }

  // Split preview point into stressforce and distance points.
  for(auto i : m_PreviewValues){
    if(DistanceOrStressOrForce::Distance ==  i.distanceOrForce){
      m_DistancePreviewValues.push_back(i.value * 0.00009921875/*mm per micro step*/);
      m_DistanceTimePreviewValues.push_back(i.timepoint);
    } else if((DistanceOrStressOrForce::Stress ==  i.distanceOrForce) || (DistanceOrStressOrForce::Force ==  i.distanceOrForce)){
      m_StressForcePreviewValues.push_back(i.value / 10000.0);
      m_StressForceTimePreviewValues.push_back(i.timepoint);
    }
  }
}

/**
 * @brief Runs the protocol.
 */
void Protocols::runProtocol(void){
  // Check if the protocol will exceed some limits and returns with a message in this case.
  if(false == checkProtocol()){
    std::unique_ptr<wxMessageDialog> popup = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(m_MyFrame, "Limit will exeed, check your experiment settings."));
    popup->ShowModal();
    return;
  }

  m_StopProtocolFlag = false;
  m_CurrentExperimentNr = 0;

  // Clear vectors
  m_PreviewStressForceLimitTimePoints.clear();
  m_DistanceLimitTimePoints.clear();
  m_MaxStressForceLimits.clear();
  m_MinStressForceLimits.clear();
  m_MaxDistanceLimits.clear();
  m_MinDistanceLimits.clear();

  // Create limit vectors
  m_PreviewStressForceLimitTimePoints.push_back(m_MyFrame->getCurrentDistance() * 0.00009921875/*mm per micro step*/);
  m_MaxStressForceLimits.push_back(m_MaxForceLimit);
  m_MinStressForceLimits.push_back(m_MinForceLimit);
  m_DistanceLimitTimePoints.push_back(m_MyFrame->getCurrentForce() / 10000.0);
  m_MaxDistanceLimits.push_back(m_MaxDistanceLimit);
  m_MinDistanceLimits.push_back(m_MinDistanceLimit);

  m_MaxStressForceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MaxStressForceLimits);
  m_MinStressForceLimitVector->SetData(m_PreviewStressForceLimitTimePoints, m_MinStressForceLimits);
  m_MaxDistanceLimitVector->SetData(m_DistanceLimitTimePoints, m_MaxDistanceLimits);
  m_MinDistanceLimitVector->SetData(m_DistanceLimitTimePoints, m_MinDistanceLimits);
  wxLogMessage(std::string("Protocols: m_PreviewStressForceLimitTimePoints: " + std::to_string(m_PreviewStressForceLimitTimePoints.size()) + " m_MaxStressForceLimits: " + std::to_string(m_MaxStressForceLimits.size())).c_str());
  wxLogMessage(std::string("Protocols: m_DistanceLimitTimePoints: " + std::to_string(m_DistanceLimitTimePoints.size()) + " m_MaxDistanceLimits: " + std::to_string(m_MaxDistanceLimits.size())).c_str());

  m_MyFrame->createValuesGraph();

  // Only continue if there are expeiments in the protocol.
  if(0 < m_Experiments.size()){
    // Run first experiment.

    // Check if the experiment is a preloading experiment where a flag has to be set.
    if(ExperimentType::Preload == m_Experiments[m_CurrentExperimentNr]->getExperimentType()){
      std::lock_guard<std::mutex> lck{*m_PreloadDoneMutex};
      *m_PreloadDoneFlag = false;
    }

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
    // Clear graph vectors.
    m_GraphStressForceValues.clear();
    m_GraphDistanceValues.clear();
    // Create shared_ptr's which are needed.
    std::shared_ptr<std::vector<double>> graphstressforce(&m_GraphStressForceValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphdistance(&m_GraphDistanceValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmaxforcelimit(&m_MaxStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphminforcelimit(&m_MinStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmaxdistancelimit(&m_MaxDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmindistancelimit(&m_MinDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphforcelimitstimepoints(&m_PreviewStressForceLimitTimePoints, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphdistancelimitstimepoints(&m_DistanceLimitTimePoints, do_nothing_deleter);
    // Start recording values.
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement(graphstressforce,
                                                                graphdistance,
                                                                graphmaxforcelimit,
                                                                graphminforcelimit,
                                                                graphmaxdistancelimit,
                                                                graphmindistancelimit,
                                                                graphforcelimitstimepoints,
                                                                graphdistancelimitstimepoints);

    // Mark the running experiment in the list box.
    m_ListBox->SetSelection(m_CurrentExperimentNr);
    // Start the experiment.
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Preload::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.join();
    m_CurrentExperimentNr++;

    // Start checkFinishedExperiment method to catch the end of the experiment.
    m_ExperimentRunningThread.reset(new std::thread(&Protocols::checkFinishedExperiment, this));
    m_ExperimentRunningThread->detach();
  }
}

/**
 * @brief Process the next experiments.
 */
void Protocols::process(void){
  // Return if protocol should be stopped.
  if(true == m_StopProtocolFlag){
    return;
  }

  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }
  if(m_Experiments.size() > m_CurrentExperimentNr){

    // Check if the experiment is a preloading experiment where a flag has to be set.
    if(ExperimentType::Preload == m_Experiments[m_CurrentExperimentNr]->getExperimentType()){
      std::lock_guard<std::mutex> lck{*m_PreloadDoneMutex};
      *m_PreloadDoneFlag = false;
    }
    {
      std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
      m_ExperimentRunningFlag = true;
    }

    // Start recording measurement values.
    {
      std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
      m_MeasurementValuesRecordingFlag = true;
    }

    // Create shared_ptr's which are needed.
    std::shared_ptr<std::vector<double>> graphstressforce(&m_GraphStressForceValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphdistance(&m_GraphDistanceValues, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmaxforcelimit(&m_MaxStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphminforcelimit(&m_MinStressForceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmaxdistancelimit(&m_MaxDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphmindistancelimit(&m_MinDistanceLimits, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphforcelimitstimepoints(&m_PreviewStressForceLimitTimePoints, do_nothing_deleter);
    std::shared_ptr<std::vector<double>> graphdistancelimitstimepoints(&m_DistanceLimitTimePoints, do_nothing_deleter);
    // Start recording values.
    m_ExperimentValues[m_CurrentExperimentNr]->startMeasurement(graphstressforce,
                                                                graphdistance,
                                                                graphmaxforcelimit,
                                                                graphminforcelimit,
                                                                graphmaxdistancelimit,
                                                                graphmindistancelimit,
                                                                graphforcelimitstimepoints,
                                                                graphdistancelimitstimepoints);

    // Mark the running experiment in the list box.
    m_ListBox->SetSelection(m_CurrentExperimentNr);
    //std::cout << "Protocols: start experiment nr.: " << m_CurrentExperimentNr << std::endl;
    // Start the experiment.
    std::thread t1(&Experiment::process, m_Experiments[m_CurrentExperimentNr], Experiment::Event::evStart);
    //std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
    t1.join();
    m_CurrentExperimentNr++;

    // Start checkFinishedExperiment method to catch the end of the experiment.
    m_ExperimentRunningThread.reset(new std::thread(&Protocols::checkFinishedExperiment, this));
    m_ExperimentRunningThread->detach();

  } else{
    m_CurrentExperimentNr = 0;
    m_ListBox->SetSelection(m_CurrentExperimentNr);

    if(true == m_LoopProtocolFlag){
      runProtocol();
    }
  }
}

/**
 * @brief Checks if protocol exceeds some limits.
 */
bool Protocols::checkProtocol(void){
  // Get the preview values.
  getPreviewValues();

  for(double i : m_StressForcePreviewValues){
    if((i > (m_MaxForceLimit)) || (i < (m_MinForceLimit))){
      return(false);
    }
  }
  for(double i : m_DistancePreviewValues){
    if((i > (m_MaxDistanceLimit)) || (i < (m_MinDistanceLimit))){
      return(false);
    }
  }
  return(true);
}

/**
 * @brief Stops the protocol.
 */
void Protocols::stopProtocol(void){
  m_StopProtocolFlag = true;
  {
    std::lock_guard<std::mutex> lck(*m_WaitMutex);
    m_Wait->notify_all();
  }
  //clearGraphStop();
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
 * @brief Returns the experiment names as a vector.
 * @return The experiment names as a vector.
 */
std::vector<std::string> Protocols::getExperimentNames(void){
  std::vector<std::string> stringvector;

  for(int i = 0; i < m_ListBox->GetCount(); ++i){
    stringvector.push_back(m_ListBox->GetString(i).ToStdString());
  }

  return(stringvector);
}

/**
 * @brief Exports the measurement data to a .csv file.
 */
void Protocols::exportCSV(std::vector<bool> disableexport){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));
  std::string pathAndFilename = m_StoragePath + "/" + "Protocol_" + std::string(mbstr) + ".txt";

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
    if(false == disableexport[i]){
      file << m_ExperimentValues[i]->getExperimentSettings();
    }
  }

  file << std::endl << std::endl;

  file << "Distance in mm; Time stamp for the distance in milli seconds; Stress/Force in " << m_ExperimentValues[0]->getStressOrForce() << "; Time stamp for stress/force in micro seconds" << std::endl;

  for(int i = 0; i < m_ExperimentValues.size(); ++i){
    if(false == disableexport[i]){

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

  wxLogMessage(std::string("Saved experiment values to: " + pathAndFilename).c_str());
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

  for(auto i : m_Experiments){
    i->setStartLength();
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
      wxLogMessage(std::string("m_PreloadDistance: " + std::to_string(m_PreloadDistance)).c_str());
      // Set the prelod distance in all the experiments.
      for(auto i : m_Experiments){
        i->setPreloadDistance();
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
  m_Experiments[m_CurrentExperimentNr - 1]->resetExperiment();
  // Reset the current experiment number if protocol should stop.
  if(true == m_StopProtocolFlag){
    m_CurrentExperimentNr = 0;
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
