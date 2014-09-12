/**
 * @file phototriggervalues.h
 * @brief Photo trigger
 * @author Andreas Ziegler
 */

#ifndef PHOTOTRIGGERVALUES_H
#define PHOTOTRIGGERVALUES_H

// Includes
#include "experimentvalues.h"

class TriggerValues : public ExperimentValues
{
  public:
  /**
   * @brief Initialize all the required parameters.
   * @param stageframe Shared pointer to the stage frame object.
   * @param forcesensormessagehandler Shared pointer to the forcesensormessagehandler object.
   * @param *forceStressDistanceGraph Pointer to the force/stress - distance graph.
   * @param *forceStressDisplacementGraph Pointer to the force/stress - displacement graph.
   * @param *vectoraccessmutex Pointer to the graph access mutex.
   * @param *maxlimitgraph Pointer to the maximum limit graph.
   * @param *minlimitgraph Pointer to the minimum limit graph.
   * @param *myframe Pointer to the main frame object.
   * @param experimentType Experiment type.
   * @param distanceOrForceOrStress Indicates if the experiment is distance-, force- or stress-based.
   * @param area Cross section area.
   * @param gagelength The gage length.
   */
    TriggerValues(std::shared_ptr<StageFrame> stageframe,
                  std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                  mpFXYVector *forceStressDistanceGraph,
                  mpFXYVector *forceStressDisplacementGraph,
                  std::mutex *vectoraccessmutex,
                  mpFXYVector *maxforcelimitvector,
                  mpFXYVector *minforcelimitvector,
                  mpFXYVector *maxdistancelimitvector,
                  mpFXYVector *mindistancelimitvector,
                  MyFrame *myframe,
                  ExperimentType experimentType,
                  DistanceOrForceOrStress distanceOrStressOrForce,
                  double area,
                  long gagelength);

    /**
     * @brief Returns the experiment settings as a std::string.
     * @return Experiment settings as std::string.
     */
    virtual std::string getExperimentSettings(void);

    /**
     * @brief Returns the experiment settings in a short form, usable for the experiment name.
     * @return The experiment settings in a short form.
     */
    virtual std::string experimentSettingsForName(void);
};

#endif // PHOTOTRIGGERVALUES_H
