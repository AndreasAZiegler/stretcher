#include "pauseresumevalues.h"

PauseResumeValues::PauseResumeValues(std::shared_ptr<StageFrame> stageframe,
                                     std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                     mpFXYVector *vector,
                                     std::mutex *vectoraccessmutex,
                                     mpFXYVector *maxlimitvector,
                                     mpFXYVector *minlimitvector,
                                     MyFrame *myframe,

                                     ExperimentType experimentType,
                                     DistanceOrStressOrForce distanceOrStressOrForce,
                                     double area)
  : ExperimentValues(stageframe,
                     forcesensormessagehandler,
                     vector,
                     vectoraccessmutex,
                     maxlimitvector,
                     minlimitvector,
                     myframe,

                     experimentType,
                     distanceOrStressOrForce,
                     area)
{
}


/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string PauseResumeValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString()));
}
