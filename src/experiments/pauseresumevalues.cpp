/**
 * @file pauseresumevalues.cpp
 * @brief Pause-Resume values
 * @author Andreas Ziegler
 */

// Includes
#include "pauseresumevalues.h"

/**
 * @brief Initialize all the required parameters.
 * @param stageframe Shared pointer to the stage frame object.
 * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
 * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
 * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
 * @param *vectoraccessmutex Pointer to the graph access mutex.
 * @param *maxlimitgraph Pointer to the maximum limit graph.
 * @param *minlimitgraph Pointer to the minimum limit graph.
 * @param *myframe Pointer to the main frame object.
 * @param experimentType Experiment type.
 * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
 * @param area Cross section area.
 * @param gagelength The gage length.
 */
PauseResumeValues::PauseResumeValues(std::shared_ptr<StageFrame> stageframe,
                                     std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                     mpFXYVector *forceStressDistanceGraph,
                                     mpFXYVector *forceStressDisplacementGraph,
                                     std::mutex *vectoraccessmutex,
                                     mpFXYVector *maxlimitvector,
                                     mpFXYVector *minlimitvector,
                                     MyFrame *myframe,

                                     ExperimentType experimentType,
                                     DistanceOrStressOrForce distanceOrStressOrForce,
                                     double area,
                                     long gagelength)
  : ExperimentValues(stageframe,
                     forcesensormessagehandler,
                     forceStressDistanceGraph,
                     forceStressDisplacementGraph,
                     vectoraccessmutex,
                     maxlimitvector,
                     minlimitvector,
                     myframe,

                     experimentType,
                     distanceOrStressOrForce,
                     area,
                     gagelength)
{
}


/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string PauseResumeValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString()));
}


/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string PauseResumeValues::experimentSettingsForName(void){
  return(std::string(""));
}
