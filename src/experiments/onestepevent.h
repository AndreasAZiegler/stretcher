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

    OneStepEvent(ExperimentParameters experimentparameters, std::string path,
                 mpFXYVector *forceStressDistanceGraph, mpFXYVector *forceStressDisplacementGraph,
                 std::mutex *vectoraccessmutex,
                 mpFXYVector *maxlimitvector,
                 mpFXYVector *minlimitvector,


                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,
                 OneStepEventParameters parameters);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(OneStepEventParameters parameters);

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
     * @brief Returns struct with the parameters for the GUI.
     * @return The parameters for the GUI.
     */
    OneStepEventParameters getParametersForGUI(void);

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
    double m_InitVelocity;																									/**< Velocity in mm/s or %L0. */
    double m_Velocity;																											/**< Velocity in mm/s. */
    double m_Delay;																													/**< Hold time 1 in s. */
    DistanceOrPercentage m_LimitDistanceOrPercentage;									/**< Indicates if the upper limit is given by value or by % of L0. */
    double m_InitLimit;																											/**< Limit in N, kPa, mm relative, mm or %L0. */
    long m_Limit;																														/**< Limit in kPa, N or mm. */
    double m_Dwell;																													/**< Hold time 2 in s. */
    bool m_HoldLimitFlag;																							/**< Indicates if the upper force limit should be hold during waiting. */
    DistanceOrPercentage m_HoldDistanceOrPercentage;												/**< Indicates if the hold distance is given by value or by % of L0. */
    double m_InitHoldDistance;																							/**< Hold distance in mm relative, mm or %L0. */
    long m_HoldDistance;																										/**< Hold distance in mm. */
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
