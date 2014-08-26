/**
 * @file experimentdefinitions.h
 * @brief Global experiment definitions.
 * @author Andreas Ziegler
 */

#ifndef EXPERIMENTSDEFINITIONS_H
#define EXPERIMENTSDEFINITIONS_H

// Includes
#include <memory>

// Forward declaration
class MyFrame;
class StageFrame;
class ForceSensorMessageHandler;

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
 * @brief Indicates whether the experiment is distance or force or stress based.
 */
enum class DistanceOrForceOrStress{
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
 * @brief Parameters for an experiment.
 */
struct ExperimentParameters{
  std::shared_ptr<StageFrame> stageframe;
  std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler;
  MyFrame *myframe;
  long maxforcelimit;
  long minforcelimit;
  long maxdistancelimit;
  long mindistancelimit;
  ExperimentType type;
  DistanceOrForceOrStress distanceOrForceOrStress;
  long gagelength;
  long mountinglength;
  long maxposdistance;
  long currentdistance;
  double area;
};

/**
 * @brief Parameters for the preload experiment.
 */
struct PreloadParameters{
  DistanceOrForceOrStress distanceOrStressOrForce;
  double stressForceLimit;
  double velocity;
};

/**
 * @brief Parameters for the one step event experiment.
 */
struct OneStepEventParameters{
  DistanceOrForceOrStress distanceOrStressOrForce;
  DistanceOrPercentage velocityDistanceOrPercentage;
  double velocity;
  double delay;
  DistanceOrPercentage limitDistanceOrPercentage;
  double limit;
  double dwell;
  bool holdLimit;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
  DistanceOrPercentage holdDistanceOrPercentage;
  double holdDistance;
};

/**
 * @brief Enum to indicate if continuous experiment is defined by the amount of steps or by a maximal value.
 */
enum class StepsOrMaxValue{Steps = 1,
                           MaxValue = 2};

/**
 * @brief Parameters for the continuous event experiment.
 */
struct ContinuousEventParameters{
  DistanceOrForceOrStress distanceOrStressOrForce;
  bool ramp2failure;
  DistanceOrPercentage velocityDistanceOrPercentage;
  double velocity;
  double holdtime;
  DistanceOrPercentage incrementDistanceOrPercentage;
  double increment;
  StepsOrMaxValue stepsOrMaxValue;
  DistanceOrPercentage maxValueDistanceOrPercentage;
  double maxvalue;
  int steps;
  int cycles;
  BehaviorAfterStop behaviorAfterStop;
  double holdForceStress;
};

#endif // EXPERIMENTSDEFINITIONS_H
