// Includes
#include <iostream>
#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param type Type of experiment.
 * @param forceOrStress Force or stress.
 */
Experiment::Experiment(ExperimentType type,
                       StressOrForce stressOrForce,
                       StageFrame *stageframe,
                       ForceSensorMessageHandler *forcesensormessagehandler,
                       mpFXYVector *vector,
                       std::mutex *vectoraccessmutex,
                       MyFrame *myframe,
                       std::string path,
                       Direction direction,
                       double forcesStressThreshold,
                       double distanceThreshold, double area, long currentdistance)
  : m_ExperimentType(type),
    m_StressOrForce(stressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_CurrentDirection(direction),
    m_ForceStressThreshold(forcesStressThreshold),
    m_DistanceThreshold(distanceThreshold),
    m_CurrentForce(0),
    m_PreloadDistance(0),
    m_CurrentPositions{0, 0},
    m_CurrentDistance(currentdistance),
    m_ExperimentValues{new ExperimentValues(type,
                                            stressOrForce,
                                            stageframe,
                                            forcesensormessagehandler,
                                            vector,
                                            vectoraccessmutex,
                                            myframe,
                                            area)}
{
}

/**
 * @brief Destructor
 */
Experiment::~Experiment(){
  std::cout << "Experiment destructor finished." << std::endl;
}
