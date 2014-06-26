// Includes
#include "onestepeventvalues.h"

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
                                       double holdtime1,
                                       long upperlimit,
                                       double holdtime2,
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
    m_DelayTime(holdtime1),
    m_UpperLimit(upperlimit),
    m_DwellTime(holdtime2),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_HoldDistance(holddistance),
    m_Cycles(cycles)
{
  m_UpperLimit = normalizeValue(m_UpperLimit);
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
    m_UpperLimit = parameters.limit * 0.00009921875/*mm per micro step*/;
  }else{
    m_UpperLimit = parameters.limit / 10000.0;
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
void OneStepEventValues::setUpperLimit(double upperlimit){
  m_UpperLimit = normalizeValue(upperlimit);
}

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string OneStepEventValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Stress/Force: " + getDistanceOrStressForce() +
                     ", Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Velocity: " + std::to_string(m_Velocity) +
                     ", Delay: " + std::to_string(m_DelayTime) +
                     ", UpperLimit: " + std::to_string(m_UpperLimit) +
                     ", Dwell: " + std::to_string(m_DwellTime) +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent() + "\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string OneStepEventValues::experimentSettingsForName(void){
  return(std::string("DoS/F:" + getDistanceOrStressForce() +
                     " CSA:" + to_string_wp(m_Area, 2) +
                     " V:" + to_string_wp(m_Velocity, 2) +
                     " De:" + to_string_wp(m_DelayTime, 2) +
                     " UL:" + to_string_wp(m_UpperLimit, 2) +
                     " Dw:" + to_string_wp(m_DwellTime, 2) +
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
