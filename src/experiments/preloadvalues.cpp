// Includes
#include <string>
#include "preloadvalues.h"

PreloadValues::PreloadValues(std::shared_ptr<StageFrame> stageframe,
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

                             double stressForceLimit,
                             double speedInMm)
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
    m_StressForceLimit(stressForceLimit/10000.0),
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
                     ", Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Stress/force limit: " + std::to_string(m_StressForceLimit) + " " + getStressOrForce() +
                     ", Velocity: " + std::to_string(m_SpeedInMm) + "mm/s\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string PreloadValues::experimentSettingsForName(void){
  return(std::string("SoF:" + getStressOrForce() +
                     " CSA:" + to_string_wp<double>(m_Area, 2) +
                     " S/F-L:" + to_string_wp<double>(m_StressForceLimit, 2) + " " + getStressOrForce() +
                     " V:" + to_string_wp<double>(m_SpeedInMm, 2) + "mm/s"));
}
