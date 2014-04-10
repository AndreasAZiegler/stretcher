#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param type Type of experiment.
 * @param forceOrStress Force or stress.
 */
Experiment::Experiment(Experiment::ExperimentType type,
                       Experiment::StressOrForce forceOrStress,
                       Direction direction,
                       double forcesStressThreshold,
                       double distanceThreshold)
  : m_ExperimentType(type),
    m_StressOrForce(forceOrStress),
    m_CurrentDirection(direction),
    m_ForceStressThreshold(forcesStressThreshold),
    m_DistanceThreshold(distanceThreshold),
    m_CurrentForce(0),
    m_CurrentPositions{0, 0}
{
}
