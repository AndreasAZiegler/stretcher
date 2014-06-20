#include "pauseresumevalues.h"

PauseResumeValues::PauseResumeValues(std::shared_ptr<StageFrame> stageframe,
                                     std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                     mpFXYVector *forceStressDistanceGraph,
                                     mpFXYVector *forceStressDisplacementGraph,
                                     std::mutex *vectoraccessmutex,
                                     mpFXYVector *maxforcelimitvector,
                                     mpFXYVector *minforcelimitvector,
                                     mpFXYVector *maxdistancelimitvector,
                                     mpFXYVector *mindistancelimitvector,
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
                     maxforcelimitvector,
                     minforcelimitvector,
                     maxdistancelimitvector,
                     mindistancelimitvector,
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
