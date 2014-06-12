// Includes
#include <thread>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "pauseresume.h"
#include "../gui/myframe.h"

PauseResume::PauseResume(std::shared_ptr<StageFrame> stageframe,
                         std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                         mpFXYVector *vector,
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
                         long zerodistance,
                         long currentdistance,
                         double area)
  : Experiment(stageframe,
               forcesensormessagehandler,
               vector,
               vectoraccessmutex,
               myframe,
               path,
               maxforcelimit,
               minforcelimit,
               maxdistancelimit,
               mindistancelimit,

               type,
               distanceOrStressOrForce,
               Direction::Stop,
               gagelength,
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
                                                             vector,
                                                             vectoraccessmutex,
                                                             maxlimitvector,
                                                             minlimitvector,

                                                             myframe,

                                                             type,
                                                             distanceOrStressOrForce,
                                                             area))
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
        /*
        wxLogMessage(std::string("Pause: Hold for: " + std::to_string(m_PauseTime * 1000) + " ms").c_str());
        std::thread t1(&Pause::sleepForMilliseconds, this, m_PauseTime);
        t1.join();
        */
        std::unique_ptr<wxMessageDialog> popup = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(m_MyFrame, "Push the button OK to resume the protocol."));
        popup->ShowModal();

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
