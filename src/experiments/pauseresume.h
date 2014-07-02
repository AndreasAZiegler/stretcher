#ifndef PAUSERESUME_H
#define PAUSERESUME_H

#include "experiment.h"
#include "pauseresumevalues.h"

class PauseResume : public Experiment, virtual public UpdatedValuesReceiver
{
  public:
    PauseResume(std::shared_ptr<StageFrame> stageframe,
                std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                mpFXYVector *forceStressDistanceGraph, mpFXYVector *forceStressDisplacementGraph,
                std::mutex *vectoraccessmutex,
                mpFXYVector *maxforcelimitvector,
                mpFXYVector *minforcelimitvector, mpFXYVector *maxdistancelimitvector, mpFXYVector *mindistancelimitvector,
                MyFrame *myframe,
                std::string path,
                long maxforcelimit,
                long minforcelimit,
                long maxdistancelimit,
                long mindistancelimit,
                std::condition_variable *wait
                , std::mutex *mutex,
                ExperimentType type,
                DistanceOrStressOrForce distanceOrStressOrForce,
                long gagelength,
                long mountinglength,
                long zerodistance,
                long currentdistance,
                double area);

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

    MyFrame *m_MyFrame;																			/**< Pointer to the main frame object. */

    std::condition_variable *m_Wait;
    std::mutex *m_WaitMutex;

    State m_CurrentState;																		/**< Current state of the preload FSM */
    std::shared_ptr<PauseResumeValues> m_ExperimentValues;			/**< Pointer to the experiment values */
};

#endif // PAUSERESUME_H
