#ifndef ONESTEPEVENTVALUES_H
#define ONESTEPEVENTVALUES_H

// Includes
#include "experiment.h"

class OneStepEventValues : public ExperimentValues
{
  public:
    OneStepEventValues(std::shared_ptr<StageFrame> stageframe,
                       std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                       mpFXYVector *forceStressDistanceGraph,
                       mpFXYVector *forceStressDisplacementGraph,
                       std::mutex *vectoraccessmutex,
                       mpFXYVector *maxlimitvector,
                       mpFXYVector *minlimitvector,
                       MyFrame *myframe,
                       std::string path,

                       ExperimentType experimentType,
                       DistanceOrStressOrForce distanceOrStressOrForce,
                       double area, long gagelength,

                       double velocity,
                       double holdtime1,
                       long upperlimit,
                       double holdtime2,
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
     * @brief Sets the upper limit.
     * @param upperlimit Upper limit
     */
    void setUpperLimit(double upperlimit);

    /**
     * @brief Sets the hold distance.
     * @param holddistance Hold distance
     */
    void setHoldDistance(double holddistance){
      m_HoldDistance = holddistance * 0.00009921875/*mm per micro step*/;
    }

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

    /**
     * @brief Returns the experiment settings in a short form, usable for the experiment name.
     * @return The experiment settings in a short form.
     */
    virtual std::string experimentSettingsForName(void);


  private:

    /**
     * @brief Returns the end of event behavior as a std::string.
     * @return The end of event behavior as a std::string.
     */
    std::string getEndOfEvent(void);

    DistanceOrStressOrForce m_DistanceOrStressOrForce;					/**< Defines if the experiment is distance or stress/force based. */
    double m_Velocity;																					/**< Velocity in mm/s. */
    double m_DelayTime;																					/**< Hold time 1 in s. */
    double m_UpperLimit;																				/**< Upper limit in kPa, N or mm. */
    double m_DwellTime;																					/**< Hold time 1 in s. */
    long m_HoldDistance;																				/**< Hold distance in mm. */
    int m_Cycles;																								/**< Amount of cycles. */
    Experiment::BehaviorAfterStop m_BehaviorAfterStop;					/**< Defines the behavior after the experiment stops. */
};

#endif // ONESTEPEVENTVALUES_H
