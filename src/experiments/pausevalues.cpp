#include "pausevalues.h"

PauseValues::PauseValues(std::shared_ptr<StageFrame> stageframe,
                         std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                         mpFXYVector *vector,
                         std::mutex *vectoraccessmutex,
                         mpFXYVector *maxforcelimitvector,
                         mpFXYVector *minforcelimitvector,
                         mpFXYVector *maxdistancelimitvector,
                         mpFXYVector *mindistancelimitvector,
                         MyFrame *myframe,

                         ExperimentType experimentType,
                         DistanceOrStressOrForce distanceOrStressOrForce,
                         double area,
                         int pausetime)
  : ExperimentValues(stageframe,
                     forcesensormessagehandler,
                     vector,
                     vectoraccessmutex,
                     maxforcelimitvector,
                     minforcelimitvector,
                     maxdistancelimitvector,
                     mindistancelimitvector,
                     myframe,

                     experimentType,
                     distanceOrStressOrForce,
                     area),
    m_PauseTime(pausetime)
{
}


/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string PauseValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Pause time: " + std::to_string(m_PauseTime) + "sec\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string PauseValues::experimentSettingsForName(void){
  return(std::string("PT:" + to_string_wp(m_PauseTime, 2) + "sec"));
}
