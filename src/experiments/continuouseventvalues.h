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
                          mpFXYVector *maxlimitvector,
                          mpFXYVector *minlimitvector,
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

    double m_Velocity;
    double m_HoldTime;
    int m_Cycles;
    int m_Steps;
    Experiment::BehaviorAfterStop m_BehaviorAfterStop;
};

#endif // CONTINUESEVENTVALUES_H
