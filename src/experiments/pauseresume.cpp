/**
 * @file pauseresume.cpp
 * @brief Pause experiment
 * @author Andreas Ziegler
 */

// Includes
#include <thread>
#include <condition_variable>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "pauseresume.h"
#include "../gui/myframe.h"

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
 */
PauseResume::PauseResume(ExperimentParameters experimentparameters,

                         mpFXYVector *forceStressDistanceGraph,
                         mpFXYVector *forceStressDisplacementGraph,
                         std::mutex *vectoraccessmutex,
                         mpFXYVector *maxforcelimitvector,
                         mpFXYVector *minforcelimitvector,
                         mpFXYVector *maxdistancelimitvector,
                         mpFXYVector *mindistancelimitvector,
                         std::condition_variable *wait,
                         std::mutex *mutex)
  : Experiment(experimentparameters,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_MyFrame(experimentparameters.myframe),
    m_Wait(wait),
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_ExperimentValues(std::make_shared<PauseResumeValues>(experimentparameters.stageframe,
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
  // Registers the update method at the message handlers.
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
* @brief Destructor unregisters the update method.
*/
PauseResume::~PauseResume(){
  // Unregisters the update method at the message handlers.
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
}

/**
 * @brief Sets the preload distance.
 */
void PauseResume::setPreloadDistance(void){
   m_GageLength = m_CurrentDistance;

   m_ExperimentValues->setGageLength(m_GageLength * 0.00009921875/*mm per micro step*/);
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void PauseResume::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("PauseResume");
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void PauseResume::getPreview(std::vector<PreviewValue>& previewvalues){
  int timepoint;
  if(0 == previewvalues.size()){
   timepoint = 1;
  }else{
   timepoint =  previewvalues.back().getTimepoint() + 1;
  }

  previewvalues.push_back(PreviewValue(timepoint, m_DistanceOrForceOrStress, m_StartLength));
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void PauseResume::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){
        wxLogMessage("PauseRelease: Start");
        m_CurrentState = runState;

        // Create conditonal variable and mutex to wait for resume.
        std::condition_variable wait;
        std::mutex mutex;
        // Open dialog from the GUI class in the GUI thread.
        m_MyFrame->showPauseResumeDialogFromPauseResume(&wait, &mutex);
        // Wait for resume
        std::unique_lock<std::mutex> lck(mutex);
        wait.wait(lck);

        if(State::runState == m_CurrentState){
          m_CurrentState = stopState;

          wxLogMessage("Pause: Stop");

          // Notify that the experiment finished.
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
        }
      }
      break;

    case runState:
      if(Event::evStop == event){
        m_CurrentState = stopState;

        wxLogMessage("Pause: Stop");

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
std::shared_ptr<ExperimentValues> PauseResume::getExperimentValues(void){
  return(m_ExperimentValues);
}

/**
 * @brief Abstract method which will be calles by the message handlers to update the values
 * @param value Position of linear stage 1 or 2 or the force
 * @param type Type of value.
 */
void PauseResume::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){

  switch(type){
    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      break;
  }
}
