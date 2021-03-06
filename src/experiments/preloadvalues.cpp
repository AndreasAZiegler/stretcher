/**
 * @file preloadvalues.cpp
 * @brief Preload values
 * @author Andreas Ziegler
 */

// Includes
#include <string>
#include <memory>
#include "../gui/myframe.h"
#include "preloadvalues.h"
#include "preload.h"

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
 * @param stressForceLimit Force or stress limit.
 * @param velocity The velocity in mm/s.
 */
PreloadValues::PreloadValues(std::shared_ptr<StageFrame> stageframe,
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
                             long gagelength,

                             double forceStressLimit,
                             double velocity)
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
                     gagelength),
    m_ForceStressLimit(forceStressLimit/10000.0),
    m_Velocity(velocity)
{
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void PreloadValues::setParameters(PreloadParameters parameters){
  m_ForceStressLimit = parameters.stressForceLimit / 10000.0;
  m_Velocity = parameters.velocity;
}

/**
 * @brief Recalculate the displacement point with the new gage length.
 * @param gagelength The gage length in mm.
 */
void PreloadValues::recalculateDisplacement(double gagelength){

  /*
  Should theoretically work but in practice it does not.
  for(double i : *m_DisplacementGraphValues){
    wxLogMessage(std::string("Preload: Old i:" + std::to_string(i)).c_str());
    i = i * m_GageLength / gagelength;
    wxLogMessage(std::string("Preload: New i:" + std::to_string(i)).c_str());
  }
  */
  // Recalculate all the displacement points with the new gage length.
  for(auto it = begin(*m_DisplacementGraphValues); it != end(*m_DisplacementGraphValues); ++it){
    *it = *it * m_GageLength / gagelength;
  }
  m_GageLength = gagelength;

  // Update the graph from the main thread.
  //m_ForceStressDisplacementGraph->SetData(*m_DisplacementGraphValues, *m_ForceStressGraphValues);
  //m_MyFrame->updateGraphFromExperimentValues();
}

/**
 * @brief Sets the gage length.
 * @param gagelength The gage length in mm.
 */
void PreloadValues::setGageLength(double gagelength){

  m_GageLength = gagelength;

}

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string PreloadValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Stress or Force: " + getForceOrStress() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Stress/force limit: " + std::to_string(m_ForceStressLimit) + " " + getForceOrStress() +
                     ", Velocity: " + std::to_string(m_Velocity) + "mm/s\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string PreloadValues::experimentSettingsForName(void){
  return(std::string("SoF:" + getForceOrStress() +
                     " CSA:" + to_string_wp<double>(m_Area, 2) +
                     " S/F-L:" + to_string_wp<double>(m_ForceStressLimit, 2) + " " + getForceOrStress() +
                     " V:" + to_string_wp<double>(m_Velocity, 2) + "mm/s"));
}
