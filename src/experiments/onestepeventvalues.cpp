#include "onestepeventvalues.h"

OneStepEventValues::OneStepEventValues(std::shared_ptr<StageFrame> stageframe,
                                       std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                       mpFXYVector *vector,
                                       std::mutex *vectoraccessmutex,
                                       MyFrame *myframe,
                                       std::string path,

                                       ExperimentType experimentType,
                                       DistanceOrStressOrForce distanceOrStressOrForce,
                                       double area,

                                       double velocity,
                                       double holdtime1,
                                       long upperlimit,
                                       double holdtime2,
                                       long lowerlimit,
                                       long holddistance,
                                       int cycles,
                                       Experiment::BehaviorAfterStop behaviorAfterStop)
  : ExperimentValues(stageframe,
                     forcesensormessagehandler,
                     vector,
                     vectoraccessmutex,
                     myframe,

                     experimentType,
                     distanceOrStressOrForce,
                     area),
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_Velocity(velocity),
    m_HoldTime1(holdtime1),
    m_UpperLimit(upperlimit),
    m_HoldTime2(holdtime2),
    m_LowerLimit(lowerlimit),
    m_BehaviorAfterStop(behaviorAfterStop),
    m_HoldDistance(holddistance),
    m_Cycles(cycles)
{
}

/**
 * @brief Returns the experiment settings as a std::string.
 * @return Experiment settings as std::string.
 */
std::string OneStepEventValues::getExperimentSettings(void){
  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Stress/Force: " + getDistanceOrStressForce() +
                     " , Stress or Force: " + getStressOrForce() +
                     ", Cross section area: " + std::to_string(m_Area) +
                     ", Velocity: " + std::to_string(m_Velocity) +
                     ", Hold time 1: " + std::to_string(m_HoldTime1) +
                     ", UpperLimit: " + std::to_string(m_UpperLimit) +
                     ", Hold time 2: " + std::to_string(m_HoldTime2) +
                     ", Lower Limit: " + std::to_string(m_LowerLimit) +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent()));
}


/**
 * @brief Returns the end of event behavior as a std::string.
 * @return The end of event behavior as a std::string.
 */
std::string OneStepEventValues::getEndOfEvent(void){
  switch(m_BehaviorAfterStop){
    case Experiment::BehaviorAfterStop::HoldADistance:
      return(std::string("Hold a distance: " + std::to_string(m_HoldDistance * 0.00009921875/*mm per micro step*/) + " mm"));
      break;

    case Experiment::BehaviorAfterStop::GoToL0:
      return(std::string("Go to L0 "));
      break;
  }
}
