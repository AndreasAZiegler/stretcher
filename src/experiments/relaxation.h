
#ifndef RELAXATION_H
#define RELAXATION_H
#include "experiment.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "../updatedvaluesreceiver.h"
//#include "ExperimentValues.h"

/**
 * @brief The relaxation experiment.
 */
class Relaxation : virtual public Experiment, virtual public UpdatedValuesReceiver
{
	public:

    /**
     * @brief Indicates whether the experiment is distance or stress/force based.
     */
    enum class DistanceOrExpansion{
      Distance = 0,
      Expansion = 1
    };

    /**
     * @brief Initializes all the needed variables.
     * @param type Type of the experiment.
     * @param stressOrForce Indicates if the experiment is stress or force based.
     * @param currentdistance The current distance of the stage frame.
     * @param stageframe Pointer to the stage frame object.
     * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param wait Wait condition.
     * @param mutex Mutex for wait condition.
     * @param distance Distance the stages should move per step.
     * @param pause The pause duration in sec.
     * @param steps The amount of steps.
     * @param preloaddistance Preload distance of the stage frame.
     */
    Relaxation(StageFrame *stageframe,
               std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
               ForceSensorMessageHandler *forcesensormessagehandler,
               mpFXYVector *vector,
               std::mutex *vectoraccessmutex,
               MyFrame *myframe,
               std::string path,

               std::condition_variable *wait,
               std::mutex *mutex,

               ExperimentType type,
               DistanceOrStressOrForce distanceOrStressOrForce,
               long gagelenght,
               long currentdistance,
               double area,

               long distance,
               double pause,
               int steps);

    ~Relaxation();

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<Experiment::PreviewValue>& previewvalue);

    /**
     * @brief Sets the experiment to distance or expansion based.
     * @param distanceOrExpansion distance or expansion based experiment.
     */
    void setDistanceOrExpansion(DistanceOrExpansion distanceOrExpansion){
      m_DistanceOrExpansion = distanceOrExpansion;
    }

    /**
     * @brief Sets the distance.
     * @param distance Distance
     */
    void setDistance(long distance){
      m_Distance = distance;
    }

    /**
     * @brief Sets the expansion.
     * @param expansion Expansion in percent.
     */
    void setExpansion(double expansion){
      m_Expansion = expansion;
    }

    /**
     * @brief Sets the pause in sec.
     * @param pause Pause in sec.
     */
    void setPause(double pause){
      m_Pause = pause;
    }

    /**
     * @brief Sets the amount of steps.
     * @param steps Amount of steps.
     */
    void setSteps(int steps){
      m_Steps = steps;
    }

    /**
     * @brief FSM of the Relaxation experiment
     * @param event Occuring event
     */
    virtual void process(Event e);

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
               pauseState,			/**< Pause state */
               goBackState};		/**< Go back state */

    StageFrame *m_StageFrame;																								/**< Pointer to the stage frame object */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler;									/**< Pointer to the message handler object */
    std::vector<LinearStageMessageHandler*> *m_LinearStageMessageHanders;		/**< Pointer to the vector which contains the pointer to the message handler of the liner stages */

    State m_CurrentState;																										/**< Current state of the preload FSM */

    DistanceOrExpansion m_DistanceOrExpansion;															/**< Indicates if experiment is distance or expansion based. */
    long m_GageLength;																									/**< Preload distance of the stage frame */
    long m_Distance;																												/**< Distance */
    long m_DistanceLimit;																										/**< Distance limit, increases every step by m_Distance */
    double m_Expansion;																											/**< Expansion */
    double m_Pause;																													/**< Pause in sec. */
    double m_SpeedInMm;																											/**< Speed in mm/sec */
    int m_Steps;																														/**< Amount of steps. */
    int m_CurrentStep;																											/**< Number of the current step */

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

		//ExperimentValues * m_expeimentvalues;

};

#endif // RELAXATION_H
