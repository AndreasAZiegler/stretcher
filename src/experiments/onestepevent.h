#ifndef ONESTEPEVENT_H
#define ONESTEPEVENT_H

// Includes
#include "experiment.h"
#include "onestepeventvalues.h"
#include "../updatedvaluesreceiver.h"

/**
 * @brief Representation of the One step experiment.
 */
class OneStepEvent : public Experiment, virtual public UpdatedValuesReceiver
{
  public:
    OneStepEvent(std::shared_ptr<StageFrame> stageframe,
                 std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                 mpFXYVector *forceStressDistanceGraph, mpFXYVector *forceStressDisplacementGraph,
                 std::mutex *vectoraccessmutex,
                 mpFXYVector *maxforcelimitvector,
                 mpFXYVector *minforcelimitvector, mpFXYVector *maxdistancelimitvector, mpFXYVector *mindistancelimitvector,
                 MyFrame *myframe,
                 std::string path,
                 long maxforcelimit,
                 long minforcelimit,
                 long maxdistancelimit,
                 long mindistancelimit,


                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,

                 ExperimentType type,
                 DistanceOrStressOrForce distanceOrStressForce,
                 long gagelength,
                 long mountinglength,
                 long zerodistance,
                 long currentdistance,
                 double area,

                 DistanceOrPercentage velocityDistanceOrPercentage,
                 double velocitypercent,
                 double velocity,
                 double delay,
                 DistanceOrPercentage upperlimitDistanceOrPercentage,
                 double upperlimitpercent,
                 long upperlimit,
                 double dwell, bool holdupperlimit,
                 DistanceOrPercentage holdDistanceOrPercentage,
                 double holddistancepercent,
                 long holddistance,
                 int cycles,
                 BehaviorAfterStop behaviorAfterStop);

    /**
     * @brief Destructor
     */
    ~OneStepEvent();

    /**
     * @brief Initializes the parameters.
     */
    void initParameters(void);

    /**
     * @brief Sets the preload distance.
     * @param preloaddistance Preload distance
     */
    virtual void setPreloadDistance();

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<Experiment::PreviewValue>& previewvalue);

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event event);

    /**
     * @brief Do all the required thing to stop the experiment during process.
     */
    virtual void resetExperiment(void);

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void){
      if(NULL != m_ExperimentValues){
        return(m_ExperimentValues);
      }else{
        return(NULL);
      }
    }

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type);

  private:

    /**
     * @enum State
     * @brief Defines the states of the Conditioning FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState,       	/**< Run state */
               holdForceState,	/**< Hold force state */
               goStartState,		/**< Go to start state */
               goBackState};		/**< Go back state */

    /**
     * @enum LimitState
     * @brief Defines the limit states.
     */
    enum LimitState{upperLimitState,		/**< Going to upper limit. */
                    lowerLimitState};		/**< Going to lower limit. */

    /**
     * @brief Sleep for the amount of milliseconds.
     * @param milliseconds
     */
    void sleepForMilliseconds(double milliseconds);

    DistanceOrPercentage m_VelocityDistanceOrPercentage;										/**< Incdicates if the velocity is given by value or by % of L0. */
    double m_VelocityPercent;																								/**< % of L0 for calculation of the velocity. */
    double m_Velocity;																											/**< Velocity in mm/s. */
    double m_Delay;																													/**< Hold time 1 in s. */
    DistanceOrPercentage m_UpperLimitDistanceOrPercentage;									/**< Indicates if the upper limit is given by value or by % of L0. */
    double m_UpperLimitPercent;																							/**< % of L0 for calculation of the upper limit. */
    long m_UpperLimit;																											/**< Upper limit in kPa, N or mm. */
    long m_InitRelUpperLimit;																								/**< Initial upper limit to calculate relative upper limit. */
    double m_Dwell;																													/**< Hold time 2 in s. */
    bool m_HoldUpperLimitFlag;																							/**< Indicates if the upper force limit should be hold during waiting. */
    DistanceOrPercentage m_HoldDistanceOrPercentage;												/**< Indicates if the hold distance is given by value or by % of L0. */
    int m_HoldDistancePercent;																							/**< % of L0 for calculation of the hold distance. */
    long m_HoldDistance;																										/**< Hold distance in mm. */
    long m_InitRelHoldDistance;																							/**< Initial hold distance to calculate relative hold distance. */
    int m_Cycles;																														/**< Amount of cycles. */
    BehaviorAfterStop m_BehaviorAfterStop;																	/**< Defines the behavior after the experiment stops. */

    State m_CurrentState;																										/**< The current experiment state. */
    long m_CurrentLimit;																										/**< Value of the current limit. */
    int m_CurrentCycle;																											/**< The current cycle. */

    bool m_WaitActive;																											/**< Indicates if a hold is active. */
    std::mutex m_WaitActiveMutex;																						/**< Mutex to protect m_WaitActive. */

    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */
    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

    std::mutex m_StageFrameAccessMutex;																			/**< Mutex to protect the access to the stage frame. */
    bool m_CheckDistanceFlag;																								/**< Indicates if the distance needs to be updated. */

    std::shared_ptr<OneStepEventValues> m_ExperimentValues;									/**< Pointer to the experiment values */
};

#endif // ONESTEPEVENT_H
