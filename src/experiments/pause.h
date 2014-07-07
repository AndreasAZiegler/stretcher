/**
 * @file pause.h
 * @brief Pause experiment.
 * @author Andreas Ziegler
 */

#ifndef PAUSE_H
#define PAUSE_H

// Includes
#include "experiment.h"
#include "pausevalues.h"

/**
 * @class Pause pause.h "experiments/pause.h"
 * @brief The Pause experiment.
 */
class Pause : public Experiment, virtual public UpdatedValuesReceiver
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
     * @param pausetime The pause time in s.
     */
    Pause(ExperimentParameters experimentparameters,
          std::string path,
          mpFXYVector *forceStressDistanceGraph,
          mpFXYVector *forceStressDisplacementGraph,
          std::mutex *vectoraccessmutex,
          mpFXYVector *maxforcelimitvector,
          mpFXYVector *minforcelimitvector,
          mpFXYVector *maxdistancelimitvector,
          mpFXYVector *mindistancelimitvector,
          std::condition_variable *wait,
          std::mutex *mutex,
          int pausetime = 0);

    /**
     * @brief Sets pause time.
     * @param pausetime Pause time in sec.
     */
    void setPauseTime(double pausetime){
      m_PauseTime = pausetime;
      m_ExperimentValues->setPauseTime(m_PauseTime);
    }

    /**
     * @brief Sets the preload distance.
     */
    virtual void setPreloadDistance(void){}

    /**
     * @brief Returns struct with the parameters for the GUI.
     * @return The parameters for the GUI.
     */
    double getParametersForGUI(void){
      return(m_PauseTime);
    }

    /**
     * @brief Saves the experiment settings in the xml_docuement.
     * @param xml Pointer to the xml_document.
     */
    virtual void getXML(pugi::xml_document &xml);

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<PreviewValue>& previewvalues);

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event event);

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     * @todo throw exception if pointer is NULL.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void);

    /**
     * @brief Do all the required thing to stop the experiment during process.
     */
    virtual void resetExperiment(void){}

    /**
     * @brief Abstract method which will be calles by the message handlers to update the values
     * @param value Position of linear stage 1 or 2 or the force
     * @param type Type of value.
     */
    virtual void updateValues(MeasurementValue value, UpdatedValuesReceiver::ValueType type){}

  private:
    /**
     * @enum State
     * @brief Defines the states of the AutoStretch FSM.
     */
    enum State{stopState,       /**< Stop state */
               runState};       /**< Run state */

    /**
     * @brief Sleep for the amount of milliseconds.
     * @param milliseconds
     */
    void sleepForMilliseconds(double seconds);

    std::condition_variable *m_Wait;										/**< Pointer to the conditioning variable to indicate the end of the experiment. */
    std::mutex *m_WaitMutex;														/**< Mutex to protect m_WaitActive. */

    State m_CurrentState;																/**< Current state of the preload FSM. */
    double m_PauseTime;																	/**< The amount of seconds, the pause shoudl be. */
    std::shared_ptr<PauseValues> m_ExperimentValues;		/**< Pointer to the experiment values. */
};

#endif // PAUSE_H
