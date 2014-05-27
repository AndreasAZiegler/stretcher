#include "continuouseventvalues.h"

ContinuousEventValues::ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
                                             std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                             mpFXYVector *vector,
                                             std::mutex *vectoraccessmutex,
                                             mpFXYVector *maxlimitvector,
                                             mpFXYVector *minlimitvector,
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
                     maxlimitvector,
                     minlimitvector,
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
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string ContinuousEventValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Stress/Force: " + getDistanceOrStressForce() +
                     " , Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Velocity: " + std::to_string(m_Velocity) +
                     ", Hold time: " + std::to_string(m_HoldTime) +
                     ", Steps: " + std::to_string(m_Steps) +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent()));
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
