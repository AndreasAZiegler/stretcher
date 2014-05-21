#ifndef ONESTEPEVENT_H
#define ONESTEPEVENT_H

// Includes
#include "experiment.h"
#include "onestepeventvalues.h"
#include "../updatedvaluesreceiver.h"

class OneStepEvent : public Experiment, virtual public UpdatedValuesReceiver
{
  private:

  public:
    OneStepEvent(std::shared_ptr<StageFrame> stageframe,
                 std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                 mpFXYVector *vector,
                 std::mutex *vectoraccessmutex,
                 MyFrame *myframe,
                 std::string path,


                 std::condition_variable *wait,
                 std::mutex *mutex,
                 bool *stagesstopped,
                 std::mutex *stagesstoppedmutex,

                 ExperimentType type,
                 DistanceOrStressOrForce distanceOrStressForce,
                 long gagelength,
                 long currentdistance,
                 double area,

                 DistanceOrPercentage velocityDistanceOrPercentage,
                 double velocity,
                 double holdtime1,
                 long upperlimit,
                 double holdtime2,
                 long lowerlimit,
                 DistanceOrPercentage holdDistanceOrPercentage,
                 long holddistance,
                 int cycles,
                 BehaviorAfterStop behaviorAfterStop);

    /**
     * @brief Sets the preload distance.
     * @param preloaddistance Preload distance
     */
    virtual void setPreloadDistance(long preloaddistance);

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

    std::shared_ptr<ExperimentValues> getExperimentValues(void){
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
    void updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type);

  private:

    /**
     * @enum State
     * @brief Defines the states of the Conditioning FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState,       	/**< Run state */
               goStartState,		/**< Go to start state */
               goBackState};		/**< Go back state */

    DistanceOrPercentage m_VelocityDistanceOrPercentage;
    double m_Velocity;
    double m_HoldTime1;
    long m_UpperLimit;
    double m_HoldTime2;
    long m_LowerLimit;
    DistanceOrPercentage m_HoldDistanceOrPercentage;
    long m_HoldDistance;
    int m_Cycles;
    BehaviorAfterStop m_BehaviorAfterStop;

    State m_CurrentState;
    long m_CurrentLimit;
    int m_CurrentCycle;

    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */
    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

    std::shared_ptr<OneStepEventValues> m_ExperimentValues;				/**< Pointer to the experiment values */
};

#endif // ONESTEPEVENT_H
