/**
 * @file phototrigger.cpp
 * @brief Photo trigger.
 * @author Andreas Ziegler
 */

// Includes
#include <thread>
#include <wx/log.h>
#include "trigger.h"

Trigger::Trigger(ExperimentParameters experimentparameters,
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
    m_WaitMutex(mutex),
    m_CurrentState(State::stopState),
    m_ExperimentValues(std::make_shared<TriggerValues>(experimentparameters.stageframe,
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
                                                            experimentparameters.experimentnumber,
                                                            experimentparameters.distanceOrForceOrStress,
                                                            experimentparameters.area,
                                                            experimentparameters.gagelength))
{
}

/**
 * @brief Saves the experiment settings in the xml_docuement.
 * @param xml Pointer to the xml_document.
 */
void Trigger::getXML(pugi::xml_document &xml){
  pugi::xml_node node = xml.append_child("Trigger");
}

/**
 * @brief Returns a pointer to the experiment values.
 * @return A pointer to the experiment values.
 * @todo throw exception if pointer is NULL.
 */
std::shared_ptr<ExperimentValues> Trigger::getExperimentValues(void){
  return(m_ExperimentValues);
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void Trigger::process(Event event){
  switch(m_CurrentState){
    case stopState:
      if(Event::evStart == event){
        m_CurrentState = runState;
        m_SerialTrigger->trigger();
        wxLogMessage("Trigger: triggered");

        // Sleep for one milli second, that the "check finished experiments" has time to be started.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        m_CurrentState = stopState;
        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      break;

    case runState:
      if(Event::evStop == event){
        m_CurrentState = stopState;

        wxLogMessage("Trigger: Stop");

        // Notify that the experiment finished.
        std::lock_guard<std::mutex> lck(*m_WaitMutex);
        m_Wait->notify_all();
      }
      break;
  }
}
