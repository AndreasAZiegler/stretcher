
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

    /**
     * @brief Initializes all the needed variables.
     * @param type Type of the experiment.
     * @param stressOrForce Indicates if the experiment is stress or force based.
     * @param stageframe Pointer to the stage frame object.
     * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param wait Wait condition.
     * @param mutex Mutex for wait condition.
     * @param cycles The number of cycles.
     * @param totaltime The total time for the experiment.
     * @param amplitude The amplitude in mm or %preload distance.
     * @param resttime The rest time after every cycle.
     * @param frequency The frequency in [cycles/sec].
     * @param preloaddistance The preload distance.
     * @param currentdistance The current distance.
     */
    FatigueTesting(Experiment::ExperimentType type,
                   StressOrForce stressOrForce,
                   StageFrame *stageframe,
                   std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                   ForceSensorMessageHandler *forcesensormessagehandler,
                   mpFXYVector *vector, std::mutex *vectoraccessmutex, MyFrame *myframe,
                   std::condition_variable *wait,
                   std::mutex *mutex,
                   int cycles, double totaltime, long amplitude, double resttime, double frequency, double area, long preloaddistance, long currentdistance);

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

    int m_Cycles;																														/**< Number of cycles */
    int m_CurrentCycle;																											/**< Number of the current cycle */
    double m_TotalTime;																											/**< Total time of the experiment */
    double m_AmplitudeInDistance;																						/**< Amplitude in distance */
    double m_RestTime;																											/**< Rest time after each cycle in sec. */
    double m_Frequency;																											/**< Frequency in cycles/sec. */
    long m_PreloadDistance;																									/**< Preload distance of the stage frame */
    double m_SpeedInMm;																											/**< Speed in mm/sec */
    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

		//ExperimentValues * m_expeimentvalues;

};

#endif // FATIGUETESTING_H
