/**
 * @file onestepeventvalues.cpp
 * @brief One step event valules
 * @author Andreas Ziegler
 */

// Includes
#include "onestepeventvalues.h"

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
 * @param path Path to the folder for exports.
 * @param experimentType Experiment type.
 * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
 * @param area Cross section area.
 * @param gagelength The gage length.
 * @param velocity The velocity in mm/s.
 * @param delay The delay time in s.
 * @param limit The limit in mm or N.
 * @param dwell The dwell in s.
 * @param holddistance The hold distance in mm.
 * @param cycles Cycles.
 * @param behaviorAfterStop The behavoir after the experiment ends.
 */
OneStepEventValues::OneStepEventValues(std::shared_ptr<StageFrame> stageframe,
                                       std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                       mpFXYVector *forceStressDistanceGraph,
                                       mpFXYVector *forceStressDisplacementGraph,
                                       std::mutex *vectoraccessmutex,
                                       mpFXYVector *maxlimitvector,
                                       mpFXYVector *minlimitvector,
                                       MyFrame *myframe,
                                       std::string path,

                                       ExperimentType experimentType,
                                       DistanceOrStressOrForce distanceOrStressOrForce,
                                       double area,
                                       long gagelength,

                                       double velocity,
                                       double delay,
                                       long limit,
                                       double dwell,
                                       long holddistance,
                                       int cycles,
                                       BehaviorAfterStop behaviorAfterStop)
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
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_Velocity(velocity),
    m_DelayTime(delay),
    m_Limit(limit),
    m_DwellTime(dwell),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_HoldDistance(holddistance),
    m_Cycles(cycles)
{
  // Normalize limit.
  m_Limit = normalizeValue(m_Limit);
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void OneStepEventValues::setParameters(OneStepEventParameters parameters){
  m_DistanceOrStressOrForce = parameters.distanceOrStressOrForce;
  m_Velocity = parameters.velocity;
  m_DelayTime = parameters.delay;
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    m_Limit = parameters.limit * 0.00009921875/*mm per micro step*/;
  }else{
    m_Limit = parameters.limit / 10000.0;
  }
  m_DwellTime = parameters.dwell;
  m_BehaviorAfterStop = parameters.behaviorAfterStop;
  m_HoldDistance = parameters.holdDistance * 0.00009921875/*mm per micro step*/;
  m_Cycles = parameters.cycles;
}

/**
 * @brief Sets the upper limit.
 * @param upperlimit Upper limit
 */
void OneStepEventValues::setLimit(double upperlimit){
  m_Limit = normalizeValue(upperlimit);
}

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string OneStepEventValues::getExperimentSettings(void){
  std::string limit;
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    limit = std::string(std::to_string(m_Limit) + "mm");
  }else if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    limit = std::string(std::to_string(m_Limit) + "N");
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    limit = std::string(std::to_string(m_Limit) + "kPa");
  }

  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Force or Stress: " + getDistanceOrForceOrStress() +
                     ", Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) + "mm2" +
                     ", Velocity: " + std::to_string(m_Velocity) + "mm/s" +
                     ", Delay: " + std::to_string(m_DelayTime) + "s" +
                     ", Limit: " + limit +
                     ", Dwell: " + std::to_string(m_DwellTime) + "s" +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent() + "\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string OneStepEventValues::experimentSettingsForName(void){
  std::string limit;
  if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    limit = std::string(to_string_wp(m_Limit) + "mm");
  }else if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    limit = std::string(to_string_wp(m_Limit) + "N");
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    limit = std::string(to_string_wp(m_Limit) + "kPa");
  }

  return(std::string("DoSoF:" + getDistanceOrForceOrStress() +
                     " CSA:" + to_string_wp(m_Area, 2) + "mm2" +
                     " V:" + to_string_wp(m_Velocity, 2) + "mm/s" +
                     " De:" + to_string_wp(m_DelayTime, 2) + "s" +
                     " L:" + limit +
                     " Dw:" + to_string_wp(m_DwellTime, 2) + "s" +
                     " C:" + to_string_wp(m_Cycles, 2) +
                     " EoE:" + getEndOfEvent()));
}

/**
 * @brief Returns the end of event behavior as a std::string.
 * @return The end of event behavior as a std::string.
 */
std::string OneStepEventValues::getEndOfEvent(void){
  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::Stop:
      return(std::string("Stop."));
      break;

    case BehaviorAfterStop::HoldADistance:
      return(std::string("Hold a distance: " + to_string_wp(m_HoldDistance * 0.00009921875/*mm per micro step*/, 2) + " mm"));
      break;

    case BehaviorAfterStop::GoToL0:
      return(std::string("Go to L0."));
      break;

    case BehaviorAfterStop::GoToML:
      return(std::string("Go to mounting length."));
      break;
  }
}
