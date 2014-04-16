
#ifndef CREEP_H
#define CREEP_H

#include <thread>
#include "experiment.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "../updatedvaluesreceiver.h"

//#include "ExperimentValues.h"

class Creep : virtual public Experiment, virtual public UpdatedValuesReceiver
{
	public:

    Creep(Experiment::ExperimentType type,
          StressOrForce stressOrForce,
          StageFrame *stageframe,
          std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
          ForceSensorMessageHandler *forcesensormessagehandler,
          std::condition_variable *wait,
          std::mutex *mutex, long holdstressforce, double holdtime, double sensitivity, double speedinmm, double area);

    ~Creep();

    /**
     * @brief Sets the speed in percent of preload.
     * @param percent Speed in percen of preload.
     */
    void setSpeedInPercent(double percent){
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
     * @brief Defines if experiment's hold value is stress or force based.
     * @param stressOrForce Stress or force based.
     */
    void setHoldStressOrForce(Experiment::StressOrForce stressOrForce){
      m_HoldStressOrForce = stressOrForce;
    }

    /**
     * @brief Sets the hold stress/force value.
     * @param force The hold force value.
     */
    void setHoldStressForce(double force){
      m_HoldStressForce = force;
    }

    /**
     * @brief Sets the hold time.
     * @param holdTime The hold time.
     */
    void setHoldTime(double holdTime){
      m_HoldTime = holdTime;
    }

    /**
     * @brief FSM of the Creep experiment
     * @param event Occuring event
     */
    virtual void process(Event e);

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(long value, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Waits the hold time and after stops experiment.
     * @param time Witing time in sec.
     */
    void waitHoldTime(double time);

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

		double m_SpeedInPercent;
		double m_SpeedInMm;
    Experiment::StressOrForce m_HoldStressOrForce;
    long m_HoldStressForce;
    long m_Sensitivity;
    double m_Area;																													/**< Area of the sample in um^2 */

    double m_HoldTime;
    bool m_HoldValueReachedFlag;
    bool m_EndFlag;
    std::mutex m_EndMutex;

    std::thread *m_WaitingThread;

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

		//ExperimentValues * m_expeimentvalues;

};

#endif // CREEP_H
