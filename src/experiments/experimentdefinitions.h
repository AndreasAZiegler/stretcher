#ifndef EXPERIMENTSDEFINITIONS_H
#define EXPERIMENTSDEFINITIONS_H

/**
 * @brief Defines the experiment types.
 */
enum class ExperimentType{
  Preload = 0,
  OneStepEvent = 1,
  ContinuousEvent = 2,
  Conditioning = 3,
  Ramp2Failure = 4
};

/**
 * @brief Indicates whether the experiment is distance or stress/force based.
 */
enum class DistanceOrStressForce{
  Distance = 0,
  StressForce = 1
};

/**
 * @brief Force or stress
 */
enum class StressOrForce{
  Stress = 0,
  Force = 1
};

#endif // EXPERIMENTSDEFINITIONS_H
