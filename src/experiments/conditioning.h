
#ifndef CONDITIONING_H
#define CONDITIONING_H

// Includes
#include <condition_variable>
#include <mathplot.h>
#include "experiment.h"
#include "../updatedvaluesreceiver.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensor.h"

class Conditioning : virtual public Experiment, virtual public UpdatedValuesReceiver
{
  public:

    /**
     * @brief Indicates whether the experiment is distance or stress/force based.
     */
    enum class DistanceOrStressForce{
      Distance = 0,
      StressForce = 1
    };

   /**
     * @brief Initializes all the needed variables
     * @param type Type of the experiment.
     * @param distanceOrStressForce Indicates if experiment is distance or force/stress based.
     * @param forceOrStress Indicates if experiment is force or stress based.
     * @param stageframe Pointer to the stage frame object.
     * @param linearstagemessagehandlers Pointer to th message handlers of the linear stages.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param graph Pointer to the graph.
     * @param wait Wait condition.
     * @param mutex Mutex for wait condition.
     * @param stressForceLimit Stress or force limit value.
     * @param speedInMM Speed in mm/s.
     * @param area Value of the area.
     * @param preloaddistance Preload distance of the stage frame.
     */
    Conditioning(Experiment::ExperimentType type,
                 Conditioning::DistanceOrStressForce distanceOrStressForce,
                 Experiment::StressOrForce forceOrStress,
                 long currentdistance,
                 StageFrame *stageframe,
                 std::vector<LinearStageMessageHandler*> *linearstagemessagehandlers,
                 ForceSensorMessageHandler *forcesensormessagehandler,
                 mpFXYVector *vector,
                 std::mutex *vectoraccessmutex, MyFrame *myframe,
                 std::condition_variable *wait,
                 std::mutex *mutex,
                 double stressForceLimit, int cycles, long distanceLimit, double speedInMM, double area, long preloaddistance);

    ~Conditioning();

    /**
     * @brief Sets the number of cycles.
     * @param cycles Number of cycles.
     */
    void setCycles(int cycles){
      m_Cycles = cycles;
    }

    /**
     * @brief Sets the speed in %preload/sec.
     * @param percent Speed in %preload/sec.
     */
    void setSpeedInPercent(double percent){
      m_SpeedInPercent = percent;
    }

    /**
     * @brief Sets the speed in mm/sec.
     * @param percent Speed in mm/sec.
     */
    void setSpeedInMm(double mm){
      m_SpeedInMm = mm;
    }

    /**
     * @brief Defines the experiment force or stress based.
     * @param stressOrForceLimit Force or stress based.
     */
    void setStressOrForceLimit(Experiment::StressOrForce stressOrForceLimit){
      m_StressOrForceLimit = stressOrForceLimit;
    }

    /**
     * @brief Sets the force or stress limit value.
     * @param stressForceLimit Stress or force limit value.
     */
    void setStressForceLimit(double stressForceLimit){
      m_StressForceLimit = stressForceLimit;
    }

    /**
     * @brief Sets the distance limit value.
     * @param distanceLimit Distance limit value.
     */
    void setDistanceLimit(long distanceLimit){
      m_DistanceLimit = distanceLimit;
    }

    /**
     * @brief FSM of the conditioning experiment
     * @param event Occuring event
     */
    void process(Experiment::Event event);

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
               goBackState};		/**< Go back state */

    std::vector<LinearStageMessageHandler*> *m_LinearStageMessageHanders;		/**< Pointer to the vector which contains the pointer to the message handler of the liner stages */

    State m_CurrentState;																										/**< Current state of the preload FSM */

    int m_Cycles;																														/**< Number of cycles */
    int m_CurrentCycle;																											/**< Number of the current cycle */
    bool m_DecreaseSpeedFlag;																								/**< Indicates if speed was decreased */
    long m_PreloadDistance;																									/**< Preload distance of the stage frame */
    double m_SpeedInPercent;																								/**< Speed in %preload/sec */
    double m_SpeedInMm;																											/**< Speed in mm/sec */
    DistanceOrStressForce m_DistanceOrStressForceLimit;											/**< Indicates if experiment is distance or stress/force based */
    Experiment::StressOrForce m_StressOrForceLimit;													/**< Indicates if experiment is stress or force based */
    long m_StressForceLimit;																								/**< Stress or force limit value */
    long m_DistanceLimit;																										/**< Distance limit value */
    double m_Area;																													/**< Area of the sample in um^2 */

    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment */
    std::mutex *m_WaitMutex;																								/**< Pointer to the mutex for m_Wait */

};

#endif // CONDITIONING_H
