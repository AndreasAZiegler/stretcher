// Includes
#include <thread>
#include <condition_variable>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "pauseresume.h"
#include "../gui/myframe.h"

PauseResume::PauseResume(std::shared_ptr<StageFrame> stageframe,
                         std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                         mpFXYVector *forceStressDistanceGraph,
                         mpFXYVector *forceStressDisplacementGraph,
                         std::mutex *vectoraccessmutex,
                         mpFXYVector *maxlimitvector,
                         mpFXYVector *minlimitvector,
                         MyFrame *myframe,
                         std::string path,
                         long maxforcelimit,
                         long minforcelimit,
                         long maxdistancelimit,
                         long mindistancelimit,

                         std::condition_variable *wait,
                         std::mutex *mutex,

                         ExperimentType type,
                         DistanceOrStressOrForce distanceOrStressOrForce,
                         long gagelength,
                         long mountinglength,
                         long zerodistance,
                         long currentdistance,
                         double area)
  : Experiment(stageframe,
               forcesensormessagehandler,
               maxforcelimit,
               minforcelimit,
               maxdistancelimit,
               mindistancelimit,

               type,
               distanceOrStressOrForce,
               Direction::Stop,
               gagelength,
               mountinglength,
               zerodistance,
               currentdistance,
               area,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
      m_MyFrame(myframe),
      m_Wait(wait),
      m_WaitMutex(mutex),
      m_CurrentState(State::stopState),
      m_ExperimentValues(std::make_shared<PauseResumeValues>(stageframe,
                                                             forcesensormessagehandler,
                                                             forceStressDistanceGraph,
                                                             forceStressDisplacementGraph,
                                                             vectoraccessmutex,
                                                             maxlimitvector,
                                                             minlimitvector,

                                                             myframe,

                                                             type,
                                                             distanceOrStressOrForce,
                                                             area,
                                                             gagelength))
{
}

/**
 * @brief Returns a vector containing the points required to cread a preview graph.
 * @return Vector containing the preview points.
 */
void PauseResume::getPreview(std::vector<PreviewValue>& previewvalues){
  int timepoint;
  if(0 == previewvalues.size()){
   timepoint = 1;
  } else{
   timepoint =  previewvalues.back().getTimepoint() + 1;
  }
  previewvalues.push_back(PreviewValue(timepoint, m_DistanceOrStressOrForce, m_StartLength));
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void PauseResume::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){
        wxLogMessage("PauseRelease: Start experiment.");
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
          std::lock_guard<std::mutex> lck(*m_WaitMutex);
          m_Wait->notify_all();
          m_CurrentState = stopState;
          wxLogMessage("Pause: Stopped.");
        }
      }
      break;

    case runState:
      if(Event::evStop == event){
        wxLogMessage("Pause: Stopped.");

        m_CurrentState = stopState;
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
