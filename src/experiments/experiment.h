
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
        PreviewValue(int itimepoint, DistanceOrStressOrForce idistanceOrForce, int ivalue){
          timepoint = itimepoint;
          distanceOrForce = idistanceOrForce;
          value = ivalue;
        }
        int getTimepoint(void){
          return(timepoint);
        }

       int timepoint;
       DistanceOrStressOrForce distanceOrForce;
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
    Experiment(std::shared_ptr<StageFrame> stageframe,
               std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
               mpFXYVector *vector,
               std::mutex *vectoraccessmutex,
               MyFrame *myframe,
               std::string path,

               ExperimentType type,
               DistanceOrStressOrForce distanceOrStressForce,
               Experiment::Direction direction,
               long gagelength, long zerodistance,
               long currentdistance,
               double area,
               double forcesStressThreshold,
               double distanceThreshold);

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
    void setDistanceOrStressOrForce(DistanceOrStressOrForce distanceOrStressOrForce){
      m_DistanceOrStressOrForce = distanceOrStressOrForce;
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

    std::shared_ptr<StageFrame> m_StageFrame;																					/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;									/**< Pointer to the message handler object */

    double m_ForceStressThreshold;							/**< Threshold for the comparison */
    double m_DistanceThreshold;									/**< Threshold for the coparison of distances */
    Direction m_CurrentDirection;								/**< The current direction */
    ExperimentType m_ExperimentType;						/**< Type of the experiment */
    DistanceOrStressOrForce m_DistanceOrStressOrForce; /**< Defines if the experiment is distance of stress/force based. */

    long m_GageLength;													/**< Preload distance of the stage frame */
    long m_ZeroDistance;												/**< Zero distance */
    long m_StartLength;													/**< Distance where the experiment starts. */
    long m_CurrentDistance;											/**< Current distance of the stage frame */
    double m_CurrentForce;											/**< Current force */
    std::vector<long> m_CurrentPositions;				/**< Vector with the current stage positions */
    bool m_ExitFlag;														/**< Flag indicating that the experiment should stop imediatly */
    double m_Area;																													/**< Area size of the sample. */

};

#endif // EXPERIMENT_H
