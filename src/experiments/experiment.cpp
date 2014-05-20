// Includes
#include <iostream>
#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param type Type of experiment.
 * @param forceOrStress Force or stress.
 */
Experiment::Experiment(StageFrame *stageframe,
                       ForceSensorMessageHandler *forcesensormessagehandler,
                       mpFXYVector *vector,
                       std::mutex *vectoraccessmutex,
                       MyFrame *myframe,
                       std::string path,

                       ExperimentType type,
                       DistanceOrStressOrForce distanceOrStressOrForce,
                       Direction direction,
                       long gagelength,
                       long currentdistance,
                       double area,
                       double forcesStressThreshold,
                       double distanceThreshold)
  : m_ExperimentType(type),
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_CurrentDirection(direction),
    m_ForceStressThreshold(forcesStressThreshold),
    m_DistanceThreshold(distanceThreshold),
    m_CurrentForce(0),
    m_GageLength(gagelength),
    m_CurrentPositions{0, 0},
    m_StartLength(currentdistance),
    m_CurrentDistance(currentdistance),
    m_Area(area * 0.000000000001/*um^2*/)
{
}

/**
 * @brief Destructor
 */
Experiment::~Experiment(){
  std::cout << "Experiment destructor finished." << std::endl;
}
