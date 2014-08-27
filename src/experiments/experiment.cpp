/**
 * @file experiment.cpp
 * @brief Experiment base class
 * @author Andreas Ziegler
 */

// Includes
#include <iostream>
#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param experimentparameters The basic experiment parameters as a struct.
 * @param forceOrStress Force or stress.
 * @param distanceThreshold Distance threshold.
 */
Experiment::Experiment(ExperimentParameters experimentparameters,
                       double forcesStressThreshold,
                       double distanceThreshold)
  : m_CheckLimitsFlag(false),
    m_MyFrame(experimentparameters.myframe),
    m_Area(experimentparameters.area),
    m_MaxForceLimit(experimentparameters.maxforcelimit),
    m_MinForceLimit(experimentparameters.minforcelimit),
    m_MaxDistanceLimit(experimentparameters.maxdistancelimit),
    m_MinDistanceLimit(experimentparameters.mindistancelimit),
    m_ExperimentType(experimentparameters.type),
    m_DistanceOrForceOrStress(experimentparameters.distanceOrForceOrStress),
    m_StageFrame(experimentparameters.stageframe),
    m_ForceSensorMessageHandler(experimentparameters.forcesensormessagehandler),
    m_CurrentDirection(Direction::Stop),
    m_ForceStressThreshold(forcesStressThreshold),
    m_DistanceThreshold(distanceThreshold),
    m_CurrentForce(0),
    m_GageLength(experimentparameters.gagelength),
    m_MountingLength(experimentparameters.mountinglength),
    m_DefaultGageLength(experimentparameters.gagelength),
    m_MaxPosDistance(experimentparameters.maxposdistance),
    m_CurrentPositions{0, 0},
    m_StartLength(experimentparameters.currentdistance),
    m_CurrentDistance(experimentparameters.currentdistance)
    //m_Area(area * 0.000000000001/*um^2*/)
{
}

/**
 * @brief Destructor
 */
Experiment::~Experiment(){
  //std::cout << "Experiment destructor finished." << std::endl;
}

/**
 * @brief Set new limits and forwards them to the experiments.
 * @param mindistancelimit Value for the minimal distance limit.
 * @param maxdistancelimit Value for the maximal distance limit.
 * @param minforcelimit Value for the minimal force limit.
 * @param maxforcelimit Value for the maximal force limit.
 */
void Experiment::setLimits(long mindistancelimit, long maxdistancelimit, long minforcelimit, long maxforcelimit){
  m_MinDistanceLimit = mindistancelimit;
  m_MaxDistanceLimit = maxdistancelimit;
  m_MinForceLimit = minforcelimit;
  m_MaxForceLimit = maxdistancelimit;
}

/**
 * @brief Sets the start length.
 */
void Experiment::setStartLength(void){
  m_StartLength = m_CurrentDistance;
}

/**
 * @brief Sets the new sensitivities.
 * @param forcestresssensitivity The force/stress sensitivity.
 * @param distancesensitivity The distance sensitivity.
 */
void Experiment::setSensitivities(long forcestresssensitivity, long distancesensitivity){
  m_ForceStressThreshold = forcestresssensitivity;
  m_DistanceThreshold = distancesensitivity;
}
