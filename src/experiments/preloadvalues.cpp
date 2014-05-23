// Includes
#include <string>
#include "preloadvalues.h"

PreloadValues::PreloadValues(std::shared_ptr<StageFrame> stageframe,
                             std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                             mpFXYVector *vector,
                             std::mutex *vectoraccessmutex,
                             mpFXYVector *maxlimitvector,
                             mpFXYVector *minlimitvector,
                             MyFrame *myframe,

                             ExperimentType experimentType,
                             DistanceOrStressOrForce distanceOrStressOrForce,
                             double area,

                             double stressForceLimit,
                             double speedInMm)
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
    m_StressForceLimit(stressForceLimit),
    m_SpeedInMm(speedInMm)
{
}

/*
PreloadValues::PreloadValues(const PreloadValues &preloadvalues)
  :	m_StressForceLimit(preloadvalues.m_StressForceLimit),
    m_SpeedInMm(preloadvalues.m_SpeedInMm)
{}
*/

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string PreloadValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     " , Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Stress/force limit: " + std::to_string(m_StressForceLimit) + " " + getStressOrForce() +
                     ", Velocity: " + std::to_string(m_SpeedInMm) + "mm/s"));
}