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

#endif // EXPERIMENTSDEFINITIONS_H
