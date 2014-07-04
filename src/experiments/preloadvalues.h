/**
 * @file preloadvalues.h
 * @brief Preload values.
 * @author Andreas Ziegler
 */

#ifndef PRELOADVALUES_H
#define PRELOADVALUES_H

// Includes
#include "experimentvalues.h"

/**
 * @class PreloadValues preloadvalues.h "experiments/preloadvalues.h"
 * @brief The Preload values.
 */
class PreloadValues : public ExperimentValues
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
     * @param stressForceLimit Force or stress limit.
     * @param velocity The velocity in mm/s.
     */
    PreloadValues(std::shared_ptr<StageFrame> stageframe,
                  std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
                  mpFXYVector *forceStressDistanceGraph,
                  mpFXYVector *forceStressDisplacementGraph,
                  std::mutex *vectoraccessmutex,
                  mpFXYVector *maxlimitvector,
                  mpFXYVector *minlimitvector,
                  MyFrame *myframe,

                  ExperimentType experimentType,
                  DistanceOrStressOrForce distanceOrStressOrForce,
                  double area,
                  long gagelength,

                  double forceStressLimit,
                  double velocity);

    /**
     * @brief Sets the parameters given by the passed struct.
     * @param parameters The parameters as a struct.
     */
    void setParameters(PreloadParameters parameters);

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

  private:
    double m_ForceStressLimit;		/**< The force or stress limit. */
    double m_Velocity;						/**< The velocity in mm/s. */
};

#endif // PRELOADVALUES_H
