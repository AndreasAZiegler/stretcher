// Includes
#include <string>
#include "preloadvalues.h"
#include "preload.h"

PreloadValues::PreloadValues(std::shared_ptr<StageFrame> stageframe,
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
                             long gagelength,

                             double stressForceLimit,
                             double speedInMm)
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
                     gagelength),
    m_StressForceLimit(stressForceLimit/10000.0),
    m_SpeedInMm(speedInMm)
{
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void PreloadValues::setParameters(PreloadParameters parameters){
  m_StressForceLimit = parameters.stressForceLimit / 10000.0;
  m_SpeedInMm = parameters.velocity;
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
