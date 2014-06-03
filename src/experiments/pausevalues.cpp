#include "pausevalues.h"

PauseValues::PauseValues(std::shared_ptr<StageFrame> stageframe,
                         std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                         mpFXYVector *vector,
                         std::mutex *vectoraccessmutex,
                         mpFXYVector *maxlimitvector,
                         mpFXYVector *minlimitvector,
                         MyFrame *myframe,

                         ExperimentType experimentType,
                         DistanceOrStressOrForce distanceOrStressOrForce,
                         double area, int pausetime)
  : ExperimentValues(stageframe,
                     forcesensormessagehandler,
                     vector,
                     vectoraccessmutex,
                     maxlimitvector,
                     minlimitvector,
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
