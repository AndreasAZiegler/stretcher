/**
 * @file onestepeventvalues.h
 * @brief One step event experiment values.
 * @author Andreas Ziegler
 */

#ifndef ONESTEPEVENTVALUES_H
#define ONESTEPEVENTVALUES_H

// Includes
#include "experiment.h"

/**
 * @class OneStepEventValues onestepeventvalues.h "experiments/onestepeventvalues.h"
 * @brief The one step event values.
 */
class OneStepEventValues : public ExperimentValues
{
  public:

    /**
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
     * @param delay The delay time in s.
     * @param limit The limit in mm or N.
     * @param dwell The dwell in s.
     * @param holddistance The hold distance in mm.
     * @param cycles Cycles.
     * @param behaviorAfterStop The behavoir after the experiment ends.
     */
    OneStepEventValues(std::shared_ptr<StageFrame> stageframe,
                       std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                       mpFXYVector *forceStressDistanceGraph,
                       mpFXYVector *forceStressDisplacementGraph,
                       std::mutex *vectoraccessmutex,
                       mpFXYVector *maxforcelimitvector,
                       mpFXYVector *minforcelimitvector, mpFXYVector *maxdistancelimitvector, mpFXYVector *mindistancelimitvector,
                       MyFrame *myframe,
                       std::string path,

                       ExperimentType experimentType,
                       DistanceOrStressOrForce distanceOrStressOrForce,
                       double area,
                       long gagelength,

                       double velocity,
                       double delay,
                       long limit,
                       double dwell,
                       long holddistance,
                       int cycles,
                       BehaviorAfterStop bahaviorAfterStop);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(OneStepEventParameters parameters);

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
    void setLimit(double upperlimit);

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
    double m_Limit;																							/**< Upper limit in kPa, N or mm. */
    double m_DwellTime;																					/**< Hold time 1 in s. */
    long m_HoldDistance;																				/**< Hold distance in mm. */
    int m_Cycles;																								/**< Amount of cycles. */
    BehaviorAfterStop m_BehaviorAfterStop;											/**< Defines the behavior after the experiment stops. */
};

#endif // ONESTEPEVENTVALUES_H
