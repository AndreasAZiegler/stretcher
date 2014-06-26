#include "continuouseventvalues.h"

ContinuousEventValues::ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
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
                                             double holdtime,
                                             int steps,
                                             long maxvalue,
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
   m_Velocity(velocity),
   m_HoldTime(holdtime),
   m_Steps(steps),
   m_MaxValue(maxvalue),
   m_Cycles(cycles),
   m_BehaviorAfterStop(behaviorAfterStop)
{
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void ContinuousEventValues::setParameters(ContinuousEventParameters parameters){
  m_DistanceOrStressOrForce = parameters.distanceOrStressOrForce;
  m_Velocity = parameters.velocity;
  m_HoldTime = parameters.holdtime;
  m_Steps = parameters.steps;
  m_Cycles = parameters.cycles;
  m_BehaviorAfterStop = parameters.behaviorAfterStop;
}

/**
 * @brief Sets the increment.
 * @param increment Increment
 */
void ContinuousEventValues::setIncrement(double increment){
  m_Increment = normalizeValue(increment);
}

/**
 * @brief Sets the maximum value.
 * @param maxvalue Maximum value
 */
void ContinuousEventValues::setMaxValue(double maxvalue){
  m_MaxValue = normalizeValue(maxvalue);
}

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string ContinuousEventValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Stress/Force: " + getDistanceOrStressForce() +
                     ", Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Velocity: " + std::to_string(m_Velocity) +
                     ", Hold time: " + std::to_string(m_HoldTime) +
                     ", Steps: " + std::to_string(m_Steps) +
                     ", Increment: " + std::to_string(m_Increment) +
                     ", Max. Value: " + std::to_string(m_MaxValue) +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent() + "\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string ContinuousEventValues::experimentSettingsForName(void){
  return(std::string("DoS/F:" + getDistanceOrStressForce() +
                     " CSA:" + to_string_wp(m_Area, 2) +
                     " V:" + to_string_wp(m_Velocity, 2) +
                     " HT:" + to_string_wp(m_HoldTime, 2) +
                     " S:" + to_string_wp(m_Steps, 2) +
                     " C:" + to_string_wp(m_Cycles, 2) +
                     " EoE:" + getEndOfEvent()));
}

/**
 * @brief Returns the end of event behavior as a std::string.
 * @return The end of event behavior as a std::string.
 */
std::string ContinuousEventValues::getEndOfEvent(void){
  switch(m_BehaviorAfterStop){
    case BehaviorAfterStop::Stop:
      return(std::string("Stop."));
      break;

    case BehaviorAfterStop::GoToL0:
      return(std::string("Go to L0."));
      break;

    case BehaviorAfterStop::GoToML:
      return(std::string("Go to mounting length."));
      break;

    case BehaviorAfterStop::HoldAForce:
      return(std::string("Stop at force/stress."));
      break;
  }
}
