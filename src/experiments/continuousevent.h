#ifndef CONTINUOUSEVENT_H
#define CONTINUOUSEVENT_H

// Includes
#include "experiment.h"
#include "continuouseventvalues.h"
#include "../updatedvaluesreceiver.h"

class ContinuousEvent : public Experiment, virtual public UpdatedValuesReceiver
{
  public:
    enum class StepsOrMaxValue{Steps = 1,
                               MaxValue = 2};

    ContinuousEvent(std::shared_ptr<StageFrame> stageframe,
                    std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                    mpFXYVector *vector,
                    std::mutex *vectoraccessmutex,
                    mpFXYVector *maxlimitvector,
                    mpFXYVector *minlimitvector,
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
                    long zerodistance,
                    long currentdistance,
                    double area,

                    DistanceOrPercentage velocityDistanceOrPercentage,
                    double velocitypercent,
                    double velocity,
                    double holdtime,
                    DistanceOrPercentage incrementDistanceOrPercentage,
                    double incrementpercent,
                    long increment,
                    StepsOrMaxValue stepsOrMaxValue,
                    DistanceOrPercentage maxvalueDistanceOrPercentage,
                    double maxvaluepercent,
                    long maxvaluelimit,
                    int steps,
                    int cycles,
                    BehaviorAfterStop behaviorAfterStop);

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
     * @brief Sleep for the amount of milliseconds.
     * @param milliseconds
     */
    void sleepForMilliseconds(double seconds);

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
    enum State{stopState,       /**< Stop state */
               runState,       	/**< Run state */
               goStartState,		/**< Go to start state */
               goBackState};		/**< Go back state */

    DistanceOrPercentage m_VelocityDistanceOrPercentage;										/**< Incdicates if the velocity is given by value or by % of L0. */
    double m_VelocityPercent;																								/**< % of L0 for calculation of the velocity. */
    double m_Velocity;																											/**< Velocity in mm/s. */
    double m_HoldTime;																											/**< Hold time 1 in s. */
    DistanceOrPercentage m_IncrementDistanceOrPercentage;
    double m_IncrementPercent;
    long m_Increment;
    StepsOrMaxValue m_StepsOrMaxValue;
    DistanceOrPercentage m_MaxValueDistanceOrPercentage;
    double m_MaxValuePercent;
    long m_MaxValueLimit;
    int m_Steps;
    int m_Cycles;																														/**< Amount of cycles. */
    BehaviorAfterStop m_BehaviorAfterStop;																	/**< Defines the behavior after the experiment stops. */

    State m_CurrentState;																										/**< The current experiment state. */
    long m_CurrentLimit;																										/**< Value of the current limit. */
    int m_CurrentStep;
    int m_CurrentCycle;																											/**< The current cycle. */

    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */
    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

    std::mutex m_StageFrameAccessMutex;																			/**< Mutex to protect the access to the stage frame. */
    bool m_CheckDistanceFlag;																								/**< Indicates if the distance needs to be updated. */

    std::shared_ptr<ContinuousEventValues> m_ExperimentValues;							/**< Pointer to the experiment values */

};

#endif // CONTINUOUSEVENT_H
