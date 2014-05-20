
#ifndef EXPERIMENT_H
#define EXPERIMENT_H

// Includes
#include <vector>
#include <mathplot.h>
#include "../hardware/stageframe.h"
#include "../hardware/forcesensormessagehandler.h"
#include "experimentdefinitions.h"
#include "experimentvalues.h"

/**
 * @brief Base class for all the experiments.
 */
class Experiment
{
  public:

    /**
     * @enum Event
     * @brief Defines the events which can occur for the AutoStretch FSM.
     */
    enum class Event{evStart,         /**< Experiment should start */
                     evUpdate,        /**< New measured value */
                     evStop,					/**< Experiment should stop */
                     evDistanceUpdate,/**< New distance value */
                     evForceUpdate};	/**< New force value */

    /**
     * @enum DistanceOrPercentage
     * @brief Defines if the distance is given as distance or as percentage of the preload.
     */
    enum class DistanceOrPercentage{Distance = 0,
                                    Percentage = 1};

    /**
     * @brief Indicates the behavior after the experiment stops.
     */
    enum BehaviorAfterStop{HoldADistance = 0,
                           Repeat = 1,
                           GoToL0 = 2};

    /**
     * @brief The PreviewValue struct
     */
    struct PreviewValue{
        PreviewValue(int itimepoint, DistanceOrStressForce idistanceOrForce, int ivalue){
          timepoint = itimepoint;
          distanceOrForce = idistanceOrForce;
          value = ivalue;
        }
        int getTimepoint(void){
          return(timepoint);
        }

       int timepoint;
       DistanceOrStressForce distanceOrForce;
       int value;
    };

  protected:
    /**
     * @enum Direction
     * @brief Defines Forwards, Backwards and Stop
     */
    enum Direction{Forwards,
                   Backwards,
                   Stop};

  public:
    /**
     * @brief Initializes the experiment type and if experiment is force or stress based.
     * @param type Type of experiment.
     * @param forceOrStress Force or stress.
     */
    Experiment(ExperimentType type,
               DistanceOrStressForce distanceOrStressForce,
               StressOrForce stressOrForce,
               DistanceOrPercentage distanceOrPercent,
               StageFrame* stageframe,
               ForceSensorMessageHandler* forcesensormessagehandler,
               mpFXYVector *vector,
               std::mutex *vectoraccessmutex,
               MyFrame *myframe,
               std::string path,
               Experiment::Direction direction,
               double forcesStressThreshold, double distanceThreshold,
               double area, long currentdistance = 0);

    /**
     * @brief Sets the preload distance.
     * @param preloaddistance Preload distance
     */
    virtual void setPreloadDistance(long preloaddistance) = 0;

    /**
     * @brief Destructor
     */
    virtual ~Experiment();

    /**
     * @brief Returns a vector containing the points required to cread a preview graph.
     * @return Vector containing the preview points.
     */
    virtual void getPreview(std::vector<PreviewValue>& previewvalues) = 0;

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event e) = 0;

    /**
     * @brief Do all the required thing to stop the experiment during process.
     */
    virtual void resetExperiment(void) = 0;

    /**
     * @brief Returns the type of experiment
     * @return The type of experiment
     */
    ExperimentType getExperimentType(){
      return(m_ExperimentType);
    }

    /**
     * @brief Returns a pointer to the experiment values.
     * @return A pointer to the experiment values.
     * @todo throw exception if pointer is NULL.
     */
    virtual std::shared_ptr<ExperimentValues> getExperimentValues(void) = 0;

    /**
     * @brief Defines if experiment is force or stress based.
     * @param forceOrStress
     */
    void setStressOrForce(StressOrForce stressOrForce){
      m_StressOrForce = stressOrForce;
    }

    /**
     * @brief Sets the exit flag, that the experiment stops.
     * @param flag flag value.
     */
    void setExitFlag(bool flag){
      m_ExitFlag = flag;
      process(Experiment::Event::evStop);
    }

  protected:

    StageFrame *m_StageFrame;																								/**< Pointer to the stage frame object */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler;									/**< Pointer to the message handler object */

    double m_ForceStressThreshold;							/**< Threshold for the comparison */
    double m_DistanceThreshold;									/**< Threshold for the coparison of distances */
    Direction m_CurrentDirection;								/**< The current direction */
    ExperimentType m_ExperimentType;						/**< Type of the experiment */
    DistanceOrStressForce m_DistanceOrStressForce; /**< Defines if the experiment is distance of stress/force based. */
    StressOrForce m_StressOrForce;							/**< Defines if the experiment is force or stress based. */
    Experiment::DistanceOrPercentage m_DistanceOrPercentage;								/**< Indicates if the distance limit is calculated by value or by percentage of preload length. */

    long m_PreloadDistance;											/**< Preload distance of the stage frame */
    long m_CurrentForce;												/**< Current force */
    std::vector<long> m_CurrentPositions;				/**< Vector with the current stage positions */
    long m_CurrentDistance;											/**< Current distance of the stage frame */
    bool m_ExitFlag;														/**< Flag indicating that the experiment should stop imediatly */
    double m_Area;																													/**< Area size of the sample. */

};

#endif // EXPERIMENT_H
