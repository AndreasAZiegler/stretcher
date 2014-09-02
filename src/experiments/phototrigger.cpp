/**
 * @file phototrigger.cpp
 * @brief Photo trigger.
 * @author Andreas Ziegler
 */

// Includes
#include <wx/log.h>
#include "phototrigger.h"

PhotoTrigger::PhotoTrigger(ExperimentParameters experimentparameters,
                           mpFXYVector *forceStressDistanceGraph,
                           mpFXYVector *forceStressDisplacementGraph,
                           std::mutex *vectoraccessmutex,
                           mpFXYVector *maxforcelimitvector,
                           mpFXYVector *minforcelimitvector,
                           mpFXYVector *maxdistancelimitvector,
                           mpFXYVector *mindistancelimitvector,
                           std::condition_variable *wait,
                           std::mutex *mutex,
                           std::shared_ptr<SerialTrigger> serialtrigger)
  : Experiment(experimentparameters,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_SerialTrigger(serialtrigger),
    m_Wait(wait),
    m_WaitMutex(mutex)
{
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void PhotoTrigger::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("Photo");
}

/**
 * @brief Returns a pointer to the experiment values.
 * @return A pointer to the experiment values.
 * @todo throw exception if pointer is NULL.
 */
std::shared_ptr<ExperimentValues> PhotoTrigger::getExperimentValues(void){
  return(m_ExperimentValues);
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void PhotoTrigger::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){
        m_CurrentState = runState;
        m_SerialTrigger->trigger();
        wxLogMessage("Photo: taken");

        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      break;

    case runState:
      if(Event::evStop == event){
        m_CurrentState = stopState;

        wxLogMessage("Photo: Stop");

        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      break;
  }
}
