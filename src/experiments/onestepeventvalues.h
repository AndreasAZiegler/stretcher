#ifndef ONESTEPEVENTVALUES_H
#define ONESTEPEVENTVALUES_H

// Includes
#include "experiment.h"

class OneStepEventValues : public ExperimentValues
{
  public:
    OneStepEventValues(StageFrame *stageframe,
                       ForceSensorMessageHandler *forcesensormessagehandler,
                       mpFXYVector *vector,
                       std::mutex *vectoraccessmutex,
                       MyFrame *myframe,
                       std::string path,

                       ExperimentType experimentType,
                       DistanceOrStressOrForce distanceOrStressOrForce,
                       double area,

                       double velocity,
                       double holdtime1,
                       long upperlimit,
                       double holdtime2,
                       long lowerlimit,
                       long holddistance,
                       int cycles,
                       Experiment::BehaviorAfterStop bahaviorAfterStop);

    /**
     * @brief Sets the velocity.
     * @param velocity Velocity in mm/s.
     */
    void setVelocity(double velocity){
      m_Velocity = velocity;
    }

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

    DistanceOrStressOrForce m_DistanceOrStressOrForce;
    double m_Velocity;
    double m_HoldTime1;
    long m_UpperLimit;
    double m_HoldTime2;
    long m_LowerLimit;
    long m_HoldDistance;
    int m_Cycles;
    Experiment::BehaviorAfterStop m_BehaviorAfterStop;
};

#endif // ONESTEPEVENTVALUES_H
