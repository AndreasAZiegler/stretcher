
#ifndef PRELOAD_H
#define PRELOAD_H

#include <condition_variable>
#include "experiment.h"
#include "preloadvalues.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "../updatedvaluesreceiver.h"

/**
 * @brief Class which represents the preload process
 */
class Preload : virtual public Experiment, virtual public UpdatedValuesReceiver
{
  public:

    /**
     * @brief Initializes all the needed variables
     * @param type Type of the experiment.
     * @param forceOrStress Indicates if experiment is force or stress based.
     * @param forcesensormessagehandler Pointer to the force sensor message handler.
     * @param wait Wait condition.
     * @param mutex Mutex for wait condition.
     * @param stressForceLimit Stress or force limit value.
     * @param speedInMM Speed in mm/s.
     * @param area Value of the area.
     */
    Preload(std::shared_ptr<StageFrame> stageframe,
            std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
            mpFXYVector *vector,
            std::mutex *vectoraccessmutex,
            mpFXYVector *maxlimitvector,
            mpFXYVector *minlimitvector,
            MyFrame *myframe,
            std::string path, long maxforcelimit, long minforcelimit, long maxdistancelimit, long mindistancelimit,

            std::condition_variable *wait,
            std::mutex *mutex,
            bool *stagesstopped,
            std::mutex *stagesstoppedmutex,

            ExperimentType type,
            DistanceOrStressOrForce distanceOrStressOrForce,
            long gagelength, long zerodistance,
            long currentdistance,
            double area,

            double stressForceLimit,
            double speedInMM);

    /**
     * @brief Sets the preload distance.
     * @param preloaddistance Preload distance
     */
    virtual void setPreloadDistance(long preloaddistance);

    ~Preload();

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<Experiment::PreviewValue>& previewvalue);

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event e);

    /**
     * @brief Do all the required thing to stop the experiment during process.
     */
    virtual void resetExperiment(void);

    /**
     * @brief Sets speed.
     * @param mm Speed in mm/s
     */
    void setSpeedInMM(double mm){
      m_SpeedInMM = mm;
    }

    /**
     * @brief Sets speed.
     * @param percent Speed in percent of the clamping distance / second.
     */
    void setSpeedInPercent(double percent){
      m_SpeedInPercent = percent;
    }


    /**
     * @brief Sets the area.
     * @param x Length in x direction.
     * @param y Length in y direction.
     */
    void setArea (double x, double y);

    /**
     * @brief Sets the force or stress limit.
     * @param forceStress Force or stress limit.
     */
    void setForceStressLimit (double forceStress){
      m_StressForceLimit = forceStress;
    }

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     * @todo throw exception if pointer is NULL.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void);

  private:
    /**
     * @enum State
     * @brief Defines the states of the AutoStretch FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState};       /**< Run state */

    std::shared_ptr<StageFrame> m_StageFrame;								/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;	/**< Pointer to the message handler object */

    State m_CurrentState;																		/**< Current state of the preload FSM */

    double m_StressForceLimit;															/**< Stress or force limit value */
    double m_SpeedInMM;																			/**< Speed in mm/sec */
    double m_SpeedInPercent;																/**< Speed in percent of clamping distance / sec */


    std::condition_variable *m_Wait;
    std::mutex *m_WaitMutex;

    bool *m_StagesStoppedFlag;
    std::mutex *m_StagesStoppedMutex;

    //PreloadValues *m_ExperimentValues;				/**< Pointer to the experiment values */
    std::shared_ptr<PreloadValues> m_ExperimentValues;				/**< Pointer to the experiment values */
};

#endif // PRELOAD_H
