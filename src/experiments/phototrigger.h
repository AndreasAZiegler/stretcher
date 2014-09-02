/**
 * @file phototrigger.h
 * @brief Photo trigger.
 * @author Andreas Ziegler
 */

#ifndef PHOTOTRIGGER_H
#define PHOTOTRIGGER_H

// Includes
#include "experiment.h"
#include "phototriggervalues.h"

/**
 * @class PhotoTrigger phototrigger.h "experiments/phototrigger.h"
 * @brief The photo trigger.
 */
class PhotoTrigger : public Experiment
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
    PhotoTrigger(ExperimentParameters experimentparameters,
                 mpFXYVector *forceStressDistanceGraph,
                 mpFXYVector *forceStressDisplacementGraph,
                 std::mutex *vectoraccessmutex,
                 mpFXYVector *maxforcelimitvector,
                 mpFXYVector *minforcelimitvector,
                 mpFXYVector *maxdistancelimitvector,
                 mpFXYVector *mindistancelimitvector,
                 std::condition_variable *wait,
                 std::mutex *mutex);

    /**
     * @brief Sets the preload distance.
     */
    virtual void setPreloadDistance(void){}

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
     * @brief Do all the required things to stop the experiment during process.
     */
    virtual void resetExperiment(void){}

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void);

  private:

    std::shared_ptr<PhotoTriggerValues> m_ExperimentValues;		/**< Pointer to the experiment values. */
};

#endif // PHOTOTRIGGER_H
