// Includes
#include <iostream>
#include "experiment.h"

/**
 * @brief Initializes the experiment type and if experiment is force or stress based.
 * @param type Type of experiment.
 * @param forceOrStress Force or stress.
 */
Experiment::Experiment(std::shared_ptr<StageFrame> stageframe,
                       std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                       MyFrame *myframe,
                       long maxforcelimit,
                       long minforcelimit,
                       long maxdistancelimit,
                       long mindistancelimit,

                       ExperimentType type,
                       DistanceOrStressOrForce distanceOrStressOrForce,
                       Direction direction,
                       long gagelength,
                       long mountinglength,
                       long maxposdistance,
                       long currentdistance,
                       double area,
                       double forcesStressThreshold,
                       double distanceThreshold)
  : m_CheckLimitsFlag(false),
    m_MyFrame(myframe),
    m_Area(area),
    m_MaxForceLimit(maxforcelimit),
    m_MinForceLimit(minforcelimit),
    m_MaxDistanceLimit(maxdistancelimit),
    m_MinDistanceLimit(mindistancelimit),
    m_ExperimentType(type),
    m_DistanceOrStressOrForce(distanceOrStressOrForce),
    m_StageFrame(stageframe),
    m_ForceSensorMessageHandler(forcesensormessagehandler),
    m_CurrentDirection(direction),
    m_ForceStressThreshold(forcesStressThreshold),
    m_DistanceThreshold(distanceThreshold),
    m_CurrentForce(0),
    m_GageLength(gagelength),
    m_MountingLength(mountinglength),
    m_DefaultGageLength(gagelength),
    m_MaxPosDistance(maxposdistance),
    m_CurrentPositions{0, 0},
    m_StartLength(currentdistance),
    m_CurrentDistance(currentdistance)
    //m_Area(area * 0.000000000001/*um^2*/)
{
}

/**
 * @brief Destructor
 */
Experiment::~Experiment(){
  std::cout << "Experiment destructor finished." << std::endl;
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
