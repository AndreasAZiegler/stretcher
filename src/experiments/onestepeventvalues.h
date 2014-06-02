#ifndef ONESTEPEVENTVALUES_H
#define ONESTEPEVENTVALUES_H

// Includes
#include "experiment.h"

class OneStepEventValues : public ExperimentValues
{
  public:
    OneStepEventValues(std::shared_ptr<StageFrame> stageframe,
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

    DistanceOrStressOrForce m_DistanceOrStressOrForce;					/**< Defines if the experiment is distance or stress/force based. */
    double m_Velocity;																					/**< Velocity in mm/s. */
    double m_HoldTime1;																					/**< Hold time 1 in s. */
    long m_UpperLimit;																					/**< Upper limit in kPa, N or mm. */
    double m_HoldTime2;																					/**< Hold time 1 in s. */
    long m_LowerLimit;																					/**< Lower limit in kPa, N or mm. */
    long m_HoldDistance;																				/**< Hold distance in mm. */
    int m_Cycles;																								/**< Amount of cycles. */
    Experiment::BehaviorAfterStop m_BehaviorAfterStop;					/**< Defines the behavior after the experiment stops. */
};

#endif // ONESTEPEVENTVALUES_H
