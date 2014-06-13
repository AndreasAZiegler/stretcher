#include "continuouseventvalues.h"

ContinuousEventValues::ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
                                             std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                             mpFXYVector *vector,
                                             std::mutex *vectoraccessmutex,
                                             mpFXYVector *maxforcelimitvector,
                                             mpFXYVector *minforcelimitvector,
                                             mpFXYVector *maxdistancelimitvector,
                                             mpFXYVector *mindistancelimitvector,
                                             MyFrame *myframe,
                                             std::string path,

                                             ExperimentType experimentType,
                                             DistanceOrStressOrForce distanceOrStressOrForce,
                                             double area,

                                             double velocity,
                                             double holdtime,
                                             int steps,
                                             int cycles,
                                             Experiment::BehaviorAfterStop behaviorAfterStop)
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
   m_Velocity(velocity),
   m_HoldTime(holdtime),
   m_Steps(steps),
   m_Cycles(cycles),
   m_BehaviorAfterStop(behaviorAfterStop)
{
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
    case Experiment::BehaviorAfterStop::Stop:
      return(std::string("Stop."));
      break;

    case Experiment::BehaviorAfterStop::GoToL0:
      return(std::string("Go to L0."));
      break;
  }
}
