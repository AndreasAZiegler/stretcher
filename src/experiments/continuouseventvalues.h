#ifndef CONTINUESEVENTVALUES_H
#define CONTINUESEVENTVALUES_H

// Includes
#include <string>
#include "experiment.h"

class ContinuousEventValues : public ExperimentValues
{
  public:
    ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
                          std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                          mpFXYVector *vector,
                          std::mutex *vectoraccessmutex,
                          mpFXYVector *maxforcelimitvector,
                          mpFXYVector *minforcelimitvector, mpFXYVector *maxdistancelimitvector, mpFXYVector *mindistancelimitvector,
                          MyFrame *myframe,
                          std::string path,

                          ExperimentType experimentType,
                          DistanceOrStressOrForce distanceOrStressOrForce,
                          double area,

                          double velocity,
                          double holdtime,
                          int steps,
                          int cycles,
                          Experiment::BehaviorAfterStop behaviorAfterStop);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

  private:

    /**
     * @brief Returns the end of event behavior as a std::string.
     * @return The end of event behavior as a std::string.
     */
    std::string getEndOfEvent(void);

    double m_Velocity;																					/**< Velocity in mm/s. */
    double m_HoldTime;																					/**< Hold time 1 in s. */
    int m_Cycles;																								/**< Amount of cycles. */
    int m_Steps;																								/**< Number of steps. */
    Experiment::BehaviorAfterStop m_BehaviorAfterStop;					/**< Defines the behavior after the experiment stops. */
};

#endif // CONTINUESEVENTVALUES_H
