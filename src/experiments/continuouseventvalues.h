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
                          double holdtime,
                          int steps, long maxvalue,
                          int cycles,
                          BehaviorAfterStop behaviorAfterStop);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(ContinuousEventParameters parameters);

    /**
     * @brief Sets the velocity.
     * @param velocity Velocity in mm/s.
     */
    void setVelocity(double velocity){
      m_Velocity = velocity;
    }

    /**
     * @brief Sets the increment.
     * @param increment Increment
     */
    void setIncrement(double increment);

    /**
     * @brief Sets the maximum value.
     * @param maxvalue Maximum value
     */
    void setMaxValue(double maxvalue);

    /**
     * @brief Steps the number of steps.
     * @param steps Number of steps
     */
    void setSteps(int steps){
      m_Steps = steps;
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

    double m_Velocity;																					/**< Velocity in mm/s. */
    double m_HoldTime;																					/**< Hold time 1 in s. */
    double m_Increment;																					/**< Increments */
    double m_MaxValue;																					/**< Maximum value */
    int m_Steps;																								/**< Number of steps. */
    int m_Cycles;																								/**< Amount of cycles. */
    BehaviorAfterStop m_BehaviorAfterStop;											/**< Defines the behavior after the experiment stops. */
};

#endif // CONTINUESEVENTVALUES_H
