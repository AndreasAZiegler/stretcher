/**
 * @file pausevalues.h
 * @brief Pause values
 * @author Andreas Ziegler
 */

#ifndef PAUSEVALUES_H
#define PAUSEVALUES_H

// Includes
#include "experimentvalues.h"

/**
 * @class PauseValues pausevalues.h "experiments/pausevalues.h"
 * @brief The Pause values.
 */
class PauseValues : public ExperimentValues
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
   * @param pausetime The pause time in s.
   */
    PauseValues(std::shared_ptr<StageFrame> stageframe,
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
                long gagelength,
                int pausetime = 0);

    /**
     * @brief Sets pause time.
     * @param pausetime Pause time in sec.
     */
    void setPauseTime(double pausetime){
      m_PauseTime = pausetime;
    }

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
    double m_PauseTime;																				/**< The amount of seconds, the pause shoudl be. */
};

#endif // PAUSEVALUES_H
