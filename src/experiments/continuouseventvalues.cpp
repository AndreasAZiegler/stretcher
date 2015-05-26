/**
 * @file continuouseventvalues.cpp
 * @brief Continuous event experiment values.
 * @author Andreas Ziegler
 */

// Includes
#include "continuouseventvalues.h"

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
 * @param holdtime the hold time in s.
 * @param steps Steps
 * @param maxvalue The maximum value.
 * @param cycles Cycles.
 * @param behaviorAfterStop The behavoir after the experiment ends.
 */
ContinuousEventValues::ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
                                             std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                                             mpFXYVector *forceStressDistanceGraph,
                                             mpFXYVector *forceStressDisplacementGraph,
                                             std::mutex *vectoraccessmutex,
                                             mpFXYVector *maxforcelimitvector,
                                             mpFXYVector *minforcelimitvector,
                                             mpFXYVector *maxdistancelimitvector,
                                             mpFXYVector *mindistancelimitvector,
                                             MyFrame *myframe,
                                             std::string path,

                                             ExperimentType experimentType,
                                             DistanceOrForceOrStress distanceOrStressOrForce,
                                             double area,
                                             long gagelength,

                                             double velocity,
                                             double holdtime,
                                             int steps,
                                             long maxvalue,
                                             long holddistance,
                                             int cycles,
                                             BehaviorAfterStop behaviorAfterStop)
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
   m_Velocity(velocity),
   m_HoldTime(holdtime),
   m_Steps(steps),
   m_MaxValue(maxvalue),
   m_HoldDistance(holddistance),
   m_Cycles(cycles),
   m_BehaviorAfterStop(behaviorAfterStop)
{
}

/**
 * @brief Sets the parameters given by the passed struct.
 * @param parameters The parameters as a struct.
 */
void ContinuousEventValues::setParameters(ContinuousEventParameters parameters){
  m_DistanceOrForceOrStress = parameters.distanceOrStressOrForce;
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
  std::string increment;
  std::string maxvalue;
  if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
    increment = std::string(std::to_string(m_Increment * 0.00009921875/*mm per micro step*/) + "mm");
    maxvalue = std::string(std::to_string(m_MaxValue * 0.00009921875/*mm per micro step*/) + "mm");
  }else if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    increment = std::string(std::to_string(m_Increment / 10000.0) + "N");
    maxvalue = std::string(std::to_string(m_MaxValue / 10000.0) + "N");
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    increment = std::string(std::to_string(m_Increment / (m_Area * 10.0)) + "kPa");
    maxvalue = std::string(std::to_string(m_MaxValue / (m_Area * 10.0)) + "kPa");
  }

  return(std::string("Experiment: " + experimentTypeToString() +
                     ", Distance or Force or Stress: " + getDistanceOrForceOrStress() +
                     ", Stress or Force: " + getForceOrStress() +
                     ", Cross section area: " + std::to_string(m_Area) + "mm2" +
                     ", Velocity: " + std::to_string(m_Velocity) + "mm/s" +
                     ", Hold time: " + std::to_string(m_HoldTime) + "s" +
                     ", Steps: " + std::to_string(m_Steps) +
                     ", Increment: " + increment +
                     ", Max. Value: " + maxvalue +
                     ", Cycles: " + std::to_string(m_Cycles) +
                     ", End of event: " + getEndOfEvent() + "\n\n"));
}

/**
 * @brief Returns the experiment settings in a short form, usable for the experiment name.
 * @return The experiment settings in a short form.
 */
std::string ContinuousEventValues::experimentSettingsForName(void){
  std::string increment;
  std::string maxvalue;
  if(DistanceOrForceOrStress::Distance == m_DistanceOrForceOrStress){
    increment = std::string(to_string_wp(m_Increment * 0.00009921875/*mm per micro step*/) + "mm");
    maxvalue = std::string(to_string_wp(m_MaxValue * 0.00009921875/*mm per micro step*/) + "mm");
  }else if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    increment = std::string(to_string_wp(m_Increment / 10000.0) + "N");
    maxvalue = std::string(to_string_wp(m_MaxValue / 10000.0) + "N");
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    increment = std::string(to_string_wp(m_Increment / (m_Area * 10.0)) + "kPa");
    maxvalue = std::string(to_string_wp(m_MaxValue / (m_Area * 10.0)) + "kPa");
  }

  return(std::string("DoSoF:" + getDistanceOrForceOrStress() +
                     " CSA:" + to_string_wp(m_Area, 2) + "mm2" +
                     " V:" + to_string_wp(m_Velocity, 2) + "mm/s" +
                     " HT:" + to_string_wp(m_HoldTime, 2) + "s" +
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

    case BehaviorAfterStop::HoldADistance:
      return(std::string("Hold a distance: " + to_string_wp(m_HoldDistance * 0.00009921875/*mm per micro step*/, 2) + " mm"));
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
