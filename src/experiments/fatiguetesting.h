
#ifndef FATIGUETESTING_H
#define FATIGUETESTING_H

#include <mutex>
#include <condition_variable>
#include "experiment.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "../updatedvaluesreceiver.h"
//#include "ExperimentValues.h"

class FatigueTesting : virtual public Experiment, virtual public UpdatedValuesReceiver
{
	public:

    FatigueTesting(Experiment::ExperimentType type,
                   StressOrForce stressOrForce,
                   StageFrame *stageframe,
                   std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                   ForceSensorMessageHandler *forcesensormessagehandler,
                   std::condition_variable *wait,
                   std::mutex *mutex,
                   int cycles, double totaltime, long amplitude, double resttime, double frequency, long preloaddistance, long currentdistance);

    /**
     * @brief Destructor
     */
    ~FatigueTesting();

    /**
     * @brief Sets the number of cycles.
     * @param cycles The number of cycles.
     */
    void setCycles(int cycles){
      m_Cycles = cycles;
    }

    /**
     * @brief Sets the total time.
     * @param time The total time.
     */
    void setTotalTime(double time){
      m_TotalTime = time;
    }

    /**
     * @brief Sets the amplitude as a distance.
     * @param distance Distance.
     */
    void setAmplitudeInDistance(double distance = 0){
      m_AmplitudeInDistance = distance;
    }

    /**
     * @brief Sets the rest time.
     * @param time The rest time.
     */
    void setRestTime(double time){
      m_RestTime = time;
    }

    /**
     * @brief Sets the frequency.
     * @param frequency The frequency.
     */
    void setFrequency(double frequency){
      m_Frequency = frequency;
    }

    /**
     * @brief FSM of the fatigue experiment
     * @param event Occuring event
     */
    virtual void process(Event event);

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(long value, UpdatedValuesReceiver::ValueType type);

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

		int m_Cycles;
    int m_CurrentCycle;
		double m_TotalTime;
		double m_AmplitudeInDistance;
		double m_RestTime;
		double m_Frequency;
    long m_PreloadDistance;																									/**< Preload distance of the stage frame */
    double m_SpeedInMm;																											/**< Speed in mm/sec */

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

		//ExperimentValues * m_expeimentvalues;

};

#endif // FATIGUETESTING_H
