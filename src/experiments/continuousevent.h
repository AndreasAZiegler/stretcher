/**
 * @file continuousevent.h
 * @brief Continuous event experiment
 * @author Andreas Ziegler
 */

#ifndef CONTINUOUSEVENT_H
#define CONTINUOUSEVENT_H

// Includes
#include "experiment.h"
#include "continuouseventvalues.h"
#include "../updatedvaluesreceiver.h"

/**
 * @class ContinuousEvent continuousevent.h "experiments/continuousevent.h"
 * @brief Represents the continuous event experiment with its FSM and the required methods.
 */
class ContinuousEvent : public Experiment, virtual public UpdatedValuesReceiver
{
  public:

    /**
     * @brief Initialize all the required parameters and registers the update methods at the message handlers.
     * @param experimentparameters Common experiment parameters.
     * @param path Path to the folder for exports.
     * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
     * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
     * @param *vectoraccessmutex Pointer to the graph access mutex.
     * @param *maxlimitgraph Pointer to the maximum limit graph.
     * @param *minlimitgraph Pointer to the minimum limit graph.
     * @param *wait Pointer to the wait condition variable.
     * @param *mutex Pointer to the mutex.
     * @param *stagesstopped Pointer to the flag stages stopped.
     * @param *stagesstoppedmutex Pointer to the mutex to protect the stagesstopped flag.
     * @param ramptofailureactiveflag Indicates if a ramp 2 failure experiment is active.
     * @param parameters Parameter struct containing the experiment parameters.
     *
     *
     * @param stageframe Shared pointer to the stage frame object.
     * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
     * @param *myframe Pointer to the main frame object.
     * @param maxforcelimit Maximum force limit value.
     * @param minforcelimit Minimum force limit value.
     * @param maxdistancelimit Maximum distance limit value.
     * @param mindistancelimit Minimum distance limit value.
     * @param type Experiment type.
     * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
     * @param gagelength The gage length.
     * @param mountinglength The mounting length.
     * @param maxposdistance Distance at the maximum stage positions.
     * @param currentdistance The current distance.
     * @param area Cross section area.
     */
    ContinuousEvent(ExperimentParameters experimentparameters,

                    std::string path,
                    mpFXYVector *forceStressDistanceGraph,
                    mpFXYVector *forceStressDisplacementGraph,
                    std::mutex *vectoraccessmutex,
                    mpFXYVector *maxforcelimitvector,
                    mpFXYVector *minforcelimitvector,
                    mpFXYVector *maxdistancelimitvector,
                    mpFXYVector *mindistancelimitvector,
                    long forcestressthreshold,
                    long distancethreshold,

                    std::condition_variable *wait,
                    std::mutex *mutex,

                    ContinuousEventParameters parameters);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(ContinuousEventParameters parameters);

    /**
     * @brief Destructor
     */
    ~ContinuousEvent();

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
     * @brief Returns struct with the parameters for the GUI.
     * @return The parameters for the GUI.
     */
    ContinuousEventParameters getParametersForGUI(void);

    /**
     * @brief Saves the experiment settings in the xml_docuement.
     * @param xml Pointer to the xml_document.
     */
    virtual void getXML(pugi::xml_document &xml);

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
    virtual void updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type);

  private:

    /**
     * @enum State
     * @brief Defines the states of the Conditioning FSM.
     */
    enum State{stopState,					/**< Stop state */
               runState,					/**< Run state */
               goStartState,			/**< Go to start state */
               goBackState,				/**< Go back state */
               goBackForceState}; /**< Go back to force state */

    /**
     * @brief Sleep for the amount of milliseconds.
     * @param milliseconds
     */
    void sleepForMilliseconds(double seconds);

    bool m_Ramp2FailureActiveFlag;																					/**< Indicates if the ramp to failure measurement is active. */
    DistanceOrPercentage m_VelocityDistanceOrPercentage;										/**< Indicates if the velocity is given by value or by % of L0. */
    double m_InitVelocity;																									/**< Velocity in mm/s or %L0. */
    double m_Velocity;																											/**< Velocity in mm/s. */
    double m_HoldTime;																											/**< Hold time 1 in s. */
    DistanceOrPercentage m_IncrementDistanceOrPercentage;										/**< Indicates if the increment is givyen by value or by & ov L0. */
    double m_InitIncrement;																									/**< Increment in N, mm relative, mm or %L0. */
    long m_Increment;																												/**< Increment in mm or N. */
    StepsOrMaxValue m_StepsOrMaxValue;																			/**< Indicates if the steps are given by value or if they have to be calculated from the max. value. */
    DistanceOrPercentage m_MaxValueDistanceOrPercentage;										/**< Incdicates if the max. value is given by value or by % of L0. */
    double m_InitMaxValueLimit;																							/**< Maximum value limit in N, mm relative, mm or %F0. */
    long m_MaxValueLimit;																										/**< Max value in mm or N. */
    int m_Steps;																														/**< Number of steps. */
    int m_Cycles;																														/**< Amount of cycles. */
    BehaviorAfterStop m_BehaviorAfterStop;																	/**< Defines the behavior after the experiment stops. */

    State m_CurrentState;																										/**< The current experiment state. */
    long m_CurrentLimit;																										/**< Value of the current limit. */
    int m_CurrentStep;																											/**< The current step. */
    int m_CurrentCycle;																											/**< The current cycle. */
    long m_MaxForceStress;																									/**< The maximum stress/force .*/
    double m_InitHoldForce;																									/**< Initial hold force. */
    long m_HoldForce;																												/**< The hold force. */

    bool m_WaitActive;																											/**< Indicates if a hold is active. */
    std::mutex m_WaitActiveMutex;																						/**< Mutex to protect m_WaitActive. */

    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */
    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

    std::mutex m_StageFrameAccessMutex;																			/**< Mutex to protect the access to the stage frame. */
    bool m_CheckDistanceFlag;																								/**< Indicates if the distance needs to be updated. */

    std::shared_ptr<ContinuousEventValues> m_ExperimentValues;							/**< Pointer to the experiment values */

};

#endif // CONTINUOUSEVENT_H
