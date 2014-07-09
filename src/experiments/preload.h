/**
 * @file preload.h
 * @brief Preload experiment.
 * @author Andreas Ziegler
 */

#ifndef PRELOAD_H
#define PRELOAD_H

#include <condition_variable>
#include "experiment.h"
#include "preloadvalues.h"
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "../updatedvaluesreceiver.h"

/**
 * @class Preload preload.h "experiments/preload.h"
 * @brief Class which represents the preload process
 */
class Preload : virtual public Experiment, virtual public UpdatedValuesReceiver
{
  public:
    /**
     * @brief Initializes all the needed variables and registers the update method at the message handelers.
     * @param experimentparameters Common experiment parameters.
     * @param path Path to the folder for exports.
     * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
     * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
     * @param *vectoraccessmutex Pointer to the graph access mutex.
     * @param *maxlimitgraph Pointer to the maximum limit graph.
     * @param *minlimitgraph Pointer to the minimum limit graph.
     * @param *wait Pointer to the wait condition variable.
     * @param *mutex Pointer to the mutex.
     * @param *stagesstopped Pointer to the flag stages stopped.
     * @param *stagesstoppedmutex Pointer to the mutex to protect the stagesstopped flag.
     * @param parameters Parameter struct containing the experiment parameters.
     */
    Preload(ExperimentParameters experimentparameters,

            std::string path,
            mpFXYVector *forceStressDistanceGraph,
            mpFXYVector *forceStressDisplacementGraph,
            std::mutex *vectoraccessmutex,
            mpFXYVector *maxforcelimitvector,
            mpFXYVector *minforcelimitvector,
            mpFXYVector *maxdistancelimitvector,
            mpFXYVector *mindistancelimitvector,
            MyFrame *myframe,
            long maxforcelimit,
            long minforcelimit,
            long maxdistancelimit,
            long mindistancelimit,
            long forcestressthreshold,
            long distancethreshold,

            std::condition_variable *wait,
            std::mutex *mutex,
            bool *stagesstopped,
            std::mutex *stagesstoppedmutex,

            PreloadParameters parameters);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(PreloadParameters parameters);

    /**
     * @brief Sets the preload distance.
     * @param preloaddistance Preload distance
     */
    virtual void setPreloadDistance();

    /**
     * @brief Destructor
     */
    ~Preload();

    /**
     * @brief Returns struct with the parameters for the GUI.
     * @return The parameters for the GUI.
     */
    PreloadParameters getParametersForGUI(void);

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
      m_Velocity = mm;
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

    std::shared_ptr<StageFrame> m_StageFrame;																/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;	/**< Pointer to the message handler object */

    State m_CurrentState;																										/**< Current state of the preload FSM */

    double m_InitStressForceLimit;																					/**< Initialize stress of force limit value. */
    long m_StressForceLimit;																								/**< Stress or force limit value */
    double m_Velocity;																											/**< Speed in mm/sec */


    std::condition_variable *m_Wait;																				/**< Pointer to the conditioning variable to indicate the end of the experiment. */
    std::mutex *m_WaitMutex;																								/**< Mutex to protect m_WaitActive. */

    bool *m_StagesStoppedFlag;																							/**< Flag indicating if stages stopped or not. */
    std::mutex *m_StagesStoppedMutex;																				/**< Mutex for m_StagesStoppedFlag */

    std::shared_ptr<PreloadValues> m_ExperimentValues;											/**< Pointer to the experiment values */
};

#endif // PRELOAD_H
