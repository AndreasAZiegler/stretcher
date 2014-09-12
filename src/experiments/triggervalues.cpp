/**
 * @file phototriggervalues.cpp
 * @brief Photo trigger
 * @author Andreas Ziegler
 */

// Includes
#include "triggervalues.h"

TriggerValues::TriggerValues(std::shared_ptr<StageFrame> stageframe,
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
                             DistanceOrForceOrStress distanceOrStressOrForce,
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
std::string TriggerValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() + "\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string TriggerValues::experimentSettingsForName(void){
  return(std::string("Trigger"));
}
