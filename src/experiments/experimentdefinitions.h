#ifndef EXPERIMENTSDEFINITIONS_H
#define EXPERIMENTSDEFINITIONS_H

/**
 * @brief Defines the experiment types.
 */
enum class ExperimentType{
  Preload = 0,
  OneStepEvent = 1,
  ContinuousEvent = 2,
  Pause = 3,
  PauseResume = 4
};

/**
 * @brief Indicates whether the experiment is distance or stress/force based.
 */
enum class DistanceOrStressOrForce{
  Distance = 0,
  Stress = 1,
  Force = 2,
};

/**
 * @enum DistanceOrPercentage
 * @brief Defines if the distance is given as distance or as percentage of the preload.
 */
enum class DistanceOrPercentage{DistanceRelative = 0,
                                Distance = 1,
                                Percentage = 2};

/**
 * @brief Indicates the behavior after the experiment stops.
 */
enum class BehaviorAfterStop{Stop = 0,
                             HoldADistance = 1,
                             HoldAForce = 2,
                             Repeat = 3,
                             GoToL0 = 4,
                             GoToML = 5};

/**
 * @brief Parameters for the preload experiment.
 */
struct PreloadParameters{
  long stressForceLimit;
  double velocity;
};

/**
 * @brief Parameters for the preload tab.
 */
struct PreloadParametersGUI{
  DistanceOrStressOrForce distanceOrStressOrForce;
  double stressForceLimit;
  double velocity;
};

/**
 * @brief Parameters for the one step event experiment.
 */
struct OneStepEventParameters{
  DistanceOrStressOrForce distanceOrStressOrForce;
  DistanceOrPercentage velocityDistanceOrPercentage;
  double velocity;
  double delay;
  DistanceOrPercentage limitDistanceOrPercentage;
  long limit;
  double dwell;
  DistanceOrPercentage holdDistanceOrPercentage;
  long holdDistance;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
};

/**
 * @brief Parameters for the one step event tab.
 */
struct OneStepEventParametersGUI{
  DistanceOrStressOrForce distanceOrStressOrForce;
  double velocity;
  double delay;
  double upperlimit;
  double dwell;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
  double holdDistance;
};

enum class StepsOrMaxValue{Steps = 1,
                           MaxValue = 2};

/**
 * @brief Parameters for the one step experiment.
 */
struct ContinuousEventParameters{
  DistanceOrStressOrForce distanceOrStressOrForce;
  DistanceOrPercentage velocityDistanceOrPercentage;
  bool ramp2failure;
  double velocity;
  double holdtime;
  DistanceOrPercentage incrementDistanceOrPercentage;
  double incrementpercentage;
  long increment;
  StepsOrMaxValue stepsOrMaxValue;
  long maxvalue;
  int steps;
  double ramp2failurePercentage;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
  long holdForceStress;
};

/**
 * @brief Parameters for the one step event tab.
 */
struct ContinuousEventParametersGUI{
  DistanceOrStressOrForce distanceOrStressOrForce;
  bool ramp2failure;
  double velocity;
  double holdtime;
  double increment;
  StepsOrMaxValue stepsOrMaxValue;
  double maxvalue;
  int steps;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
  double holdForceStress;
};

#endif // EXPERIMENTSDEFINITIONS_H
