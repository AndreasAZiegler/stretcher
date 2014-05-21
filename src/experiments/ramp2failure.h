
#ifndef RAMP2FAILURE_H
#define RAMP2FAILURE_H
#include "experiment.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"

//#include "experimentvalues.h"

class Ramp2Failure : virtual public Experiment, virtual public UpdatedValuesReceiver
{
  public:

    /**
     * @brief Possible bahoviors after failur (end of the experiment)
     */
    enum class BehaviorAfterFailure{
      PreloadPos = 0,
      Stop = 1,
      GoTo = 2
    };

    /**
     * @brief Initializes all the needed variables.
     * @param type Type of the experiment.
     * @param stageframe Pointer to the stage frame object.
     * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param wait Wait condition.
     * @param mutex Mutex for wait condition.
     * @param speedInMM Speed in mm/s.
     * @param area Value of the area.
     * @param preloaddistance Preload distance of the stage frame.
     */
    Ramp2Failure(std::shared_ptr<StageFrame> stageframe,
                 std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                 std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                 mpFXYVector *vector,
                 std::mutex *vectoraccessmutex,
                 MyFrame *myframe,
                 std::string path,

                 std::condition_variable *wait,
                 std::mutex *mutex,

                 ExperimentType type,
                 DistanceOrStressOrForce distanceOrStressOrForce,
                 long gagelength,
                 long currentdistance,
                 double area,

                 BehaviorAfterFailure behavior,
                 double speedInMM,
                 double dropbeforestop,
                 long preloaddistance,
                 long distanceafterfailure);

    ~Ramp2Failure();

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<Experiment::PreviewValue>& previewvalue);

    /**
     * @brief Sets the speed in percent of preload.
     * @param percent Speed in percen of preload.
     */
    void setSpeedPercent(double percent){
      m_SpeedInPercent = percent;
    }

    /**
     * @brief Sets the speed in mm/s.
     * @param mm Speed in mm/s.
     */
    void setSpeedMm(double mm){
      m_SpeedInMm = mm;
    }

    /**
     * @brief FSM of the Ramp2Failure experiment
     * @param event Occuring event
     */
    virtual void process(Event e);

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Sets the percentage of max force which will stop the experiment when it is reached.
     * @param force Percent of maximal force.
     */
    void setDropBeforeStop(int percent){
      m_DropBeforeStop = percent;
    }

    /**
     * @brief Sets the bahavior for the end of the experiment.
     * @param bahvior Behavior
     */
    void setBehavior(BehaviorAfterFailure behavior){
      m_BehaviorAfterFailure = behavior;
    }

    /**
     * @brief Sets the distance after failure.
     * @param distance Distance after failure.
     */
    void setDistance(double distance){
      m_DistanceAfterFailure = distance;
    }

    /**
     * @brief Sets the distance after failure according to the percentage of the preload.
     * @param percent Percent of preload.
     */
    void setPercentPreload(double percent);

	private:

    /**
     * @enum State
     * @brief Defines the states of the Ramp2Failure FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState,       	/**< Run state */
               goBackState};		/**< Go back state */

    std::shared_ptr<StageFrame> m_StageFrame;																/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;	/**< Pointer to the message handler object */
    std::vector<LinearStageMessageHandler*> *m_LinearStageMessageHanders;		/**< Pointer to the vector which contains the pointer to the message handler of the liner stages */

    double m_SpeedInPercent;																								/**< Speed in %preload/sec */
    double m_SpeedInMm;																											/**< Speed in mm/sec */
    double m_DropBeforeStop;																								/**< Percentage of maximal force */
    BehaviorAfterFailure m_BehaviorAfterFailure;														/**< Indicates what happens after the experiment */
    long m_DistanceAfterFailure;																						/**< Distance after experiment if desired */
    State m_CurrentState;																										/**< Current state of the FSM */
    long m_GageLength;																											/**< Preload distance of the stage frame */
    double m_Area;																													/**< Area of the sample in um^2 */
    long m_MaxForce;																												/**< Maximal measured force */
    std::mutex m_ForceMutex;																								/**< Mutex to protect m_MaxForce and m_CurrentForce */

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

		//ExperimentValues * m_expeimentvalues;

};

#endif // RAMP2FAILURE_H
