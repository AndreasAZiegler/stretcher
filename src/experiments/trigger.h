/**
 * @file phototrigger.h
 * @brief Photo trigger.
 * @author Andreas Ziegler
 */

#ifndef PHOTOTRIGGER_H
#define PHOTOTRIGGER_H

// Includes
#include "../hardware/serialtrigger.h"
#include "experiment.h"
#include "triggervalues.h"

/**
 * @class PhotoTrigger phototrigger.h "experiments/phototrigger.h"
 * @brief The photo trigger.
 */
class Trigger : public Experiment
{
  public:
    /**
     * @brief Initialize all the required parameters.
     * @param experimentparameters Common experiment parameters.
     * @param path Path to the folder for exports.
     * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
     * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
     * @param *vectoraccessmutex Pointer to the graph access mutex.
     * @param *maxlimitgraph Pointer to the maximum limit graph.
     * @param *minlimitgraph Pointer to the minimum limit graph.
     * @param *wait Pointer to the wait condition variable.
     * @param *mutex Pointer to the mutex.
     */
    Trigger(ExperimentParameters experimentparameters,
            mpFXYVector *forceStressDistanceGraph,
            mpFXYVector *forceStressDisplacementGraph,
            std::mutex *vectoraccessmutex,
            mpFXYVector *maxforcelimitvector,
            mpFXYVector *minforcelimitvector,
            mpFXYVector *maxdistancelimitvector,
            mpFXYVector *mindistancelimitvector,
            std::condition_variable *wait,
            std::mutex *mutex,
            std::shared_ptr<SerialTrigger> serialtrigger);

    /**
     * @brief Sets the preload distance.
     */
    virtual void setPreloadDistance(void){}

    /**
     * @brief Saves the experiment settings in the xml_docuement.
     * @param xml Pointer to the xml_document.
     */
    virtual void getXML(pugi::xml_document &xml);

    /**
     * @brief Saves the points required to creat a preview graph in the vector..
     * @param previewvalues Vector containing the preview values.
     */
    virtual void getPreview(std::vector<PreviewValue>& previewvalues){}

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event event);

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void);

    /**
     * @brief Do all the required things to stop the experiment during process.
     */
    virtual void resetExperiment(void){}

  private:
    /**
     * @enum State
     * @brief Defines the states of the AutoStretch FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState};       /**< Run state */

    std::shared_ptr<SerialTrigger> m_SerialTrigger;						/**< Pointer to the serial trigger object */
    std::shared_ptr<TriggerValues> m_ExperimentValues;				/**< Pointer to the experiment values. */

    std::condition_variable *m_Wait;													/**< Pointer to the conditioning variable to indicate the end of the experiment. */
    std::mutex *m_WaitMutex;																	/**< Mutex to protect m_WaitActive. */

    State m_CurrentState;																			/**< Current state of the preload FSM. */
};

#endif // PHOTOTRIGGER_H
