/**
 * @file continuouseventvalues.h
 * @brief Continuous event experiment values.
 * @author Andreas Ziegler
 */

#ifndef CONTINUESEVENTVALUES_H
#define CONTINUESEVENTVALUES_H

// Includes
#include <string>
#include "experiment.h"

/**
 * @class ContinuousEventValues continuouseventvalues.h "experiments/continuouseventvalues.h"
 * @brief Responsible for the recording of the measurement values and the parameter names.
 */
class ContinuousEventValues : public ExperimentValues
{
  public:
    /**
     * @brief ContinuousEventValues
     * @brief Initialize all the required parameters.
     * @param stageframe Shared pointer to the stage frame object.
     * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
     * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
     * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
     * @param *vectoraccessmutex Pointer to the graph access mutex.
     * @param *maxlimitgraph Pointer to the maximum limit graph.
     * @param *minlimitgraph Pointer to the minimum limit graph.
     * @param *myframe Pointer to the main frame object.
     * @param path Path to the folder for exports.
     * @param experimentType Experiment type.
     * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
     * @param area Cross section area.
     * @param gagelength The gage length.
     * @param velocity The velocity in mm/s.
     * @param holdtime the hold time in s.
     * @param steps Steps
     * @param maxvalue The maximum value.
     * @param cycles Cycles.
     * @param behaviorAfterStop The behavoir after the experiment ends.
     */
    ContinuousEventValues(std::shared_ptr<StageFrame> stageframe,
                          std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                          mpFXYVector *forceStressDistanceGraph,
                          mpFXYVector *forceStressDisplacementGraph,
                          std::mutex *vectoraccessmutex,
                          mpFXYVector *maxforcelimitvector,
                          mpFXYVector *minforcelimitvector,
                          mpFXYVector *maxdistancelimitvector,
                          mpFXYVector *mindistancelimitvector,
                          MyFrame *myframe,
                          std::string path,

                          ExperimentType experimentType, int experimentnumber,
                          DistanceOrForceOrStress distanceOrStressOrForce,
                          double area,
                          long gagelength,

                          double velocity,
                          double holdtime,
                          int steps,
                          long maxvalue,
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
