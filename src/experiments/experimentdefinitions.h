#ifndef EXPERIMENTSDEFINITIONS_H
#define EXPERIMENTSDEFINITIONS_H

/**
 * @brief Defines the experiment types.
 */
enum class ExperimentType{
  Preload = 0,
  Conditioning = 1,
  Ramp2Failure = 2,
  Relaxation = 3,
  Creep = 4,
  FatigueTesting = 5,
  ChamberStretchGel = 6,
  ChamberStretchCells = 7
};

/**
 * @brief Force or stress
 */
enum class StressOrForce{
  Stress = 0,
  Force = 1
};

#endif // EXPERIMENTSDEFINITIONS_H
