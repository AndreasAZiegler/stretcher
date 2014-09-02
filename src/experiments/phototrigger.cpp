/**
 * @file phototrigger.cpp
 * @brief Photo trigger.
 * @author Andreas Ziegler
 */

// Includes
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
                           std::mutex *mutex)
  : Experiment(experimentparameters,
               0.005 * 10000.0/*stress force threshold*/,
               0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/)
{
}

/**
 * @brief FSM of the experiment
 * @param e Occuring event
 */
void PhotoTrigger::process(Event event){

}

/**
 * @brief Returns a pointer to the experiment values.
 * @return A pointer to the experiment values.
 * @todo throw exception if pointer is NULL.
 */
std::shared_ptr<ExperimentValues> PhotoTrigger::getExperimentValues(void){
  return(m_ExperimentValues);
}
