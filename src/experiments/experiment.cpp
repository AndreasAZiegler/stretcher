#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param type Type of experiment.
 * @param forceOrStress Force or stress.
 */
Experiment::Experiment(Experiment::ExperimentType type, Experiment::ForceOrStress forceOrStress)
  : m_ExperimentType(type), m_ForceOrStress(forceOrStress)
{
}
