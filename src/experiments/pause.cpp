/**
 * @file pause.cpp
 * @brief Pause experiment
 * @author Andreas Ziegler
 */

// Includes
#include <thread>
#include <wx/log.h>
#include "pause.h"

/**
 * @brief Initialize all the required parameters.
 * @param experimentparameters Common experiment parameters.
 * @param path Path to the folder for exports.
 * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
 * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
 * @param *vectoraccessmutex Pointer to the graph access mutex.
 * @param *maxlimitgraph Pointer to the maximum limit graph.
 * @param *minlimitgraph Pointer to the minimum limit graph.
 * @param *wait Pointer to the wait condition variable.
 * @param *mutex Pointer to the mutex.
 * @param pausetime The pause time in s.
 */
Pause::Pause(ExperimentParameters experimentparameters,

             std::string path,
             mpFXYVector *forceStressDistanceGraph,
             mpFXYVector *forceStressDisplacementGraph,
             std::mutex *vectoraccessmutex,
             mpFXYVector *maxforcelimitvector,
             mpFXYVector *minforcelimitvector,
             mpFXYVector *maxdistancelimitvector,
             mpFXYVector *mindistancelimitvector,
             std::condition_variable *wait,
             std::mutex *mutex,

             int pausetime)
  : Experiment(experimentparameters,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_PauseTime(pausetime),
    m_ExperimentValues(std::make_shared<PauseValues>(experimentparameters.stageframe,
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
                                                     experimentparameters.gagelength))
{
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void Pause::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("Pause");

  node.append_attribute("PauseTime") = m_PauseTime;
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void Pause::getPreview(std::vector<PreviewValue>& previewvalues){
  int timepoint;
  if(0 == previewvalues.size()){
   timepoint = 1;
  }else{
   timepoint =  previewvalues.back().getTimepoint() + 1;
  }

  previewvalues.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StartLength));
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void Pause::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){
        m_CurrentState = runState;
        wxLogMessage("Pause: Start experiment.");
        wxLogMessage(std::string("Pause: Hold for: " + std::to_string(m_PauseTime * 1000) + " ms").c_str());
        std::thread t1(&Pause::sleepForMilliseconds, this, m_PauseTime);
        t1.join();
        wxLogMessage("Pause: Holding over.");

        if(State::runState == m_CurrentState){
          m_CurrentState = stopState;
          wxLogMessage("Pause: Stopped.");
          // Notify that the experiment finished.
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }
      }
      break;

    case runState:
      if(Event::evStop == event){
        wxLogMessage("Pause: Stopped.");

        m_CurrentState = stopState;
        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      break;
  }
}

/**
 * @brief Returns a pointer to the experiment values.
 * @return A pointer to the experiment values.
 * @todo throw exception if pointer is NULL.
 */
std::shared_ptr<ExperimentValues> Pause::getExperimentValues(void){
  return(m_ExperimentValues);
}

/**
 * @brief Sleep for the amount of milliseconds.
 * @param milliseconds The waiting time in milliseconds.
 */
void Pause::sleepForMilliseconds(double seconds){
  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
}
