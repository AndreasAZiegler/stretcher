
#ifndef EXPERIMENT_H
#define EXPERIMENT_H

// Includes
#include <vector>

/**
 * @brief Base class for all the experiments.
 */
class Experiment
{
  public:

    /**
     * @brief Defines the experiment types.
     */
    enum class ExperimentType{
      Preload = 0,
      Conditioning = 1,
      Ramp2Failure = 2,
      Relaxation = 3,
      Creep = 4,
      FatigueTesting = 4
    };

    /**
     * @brief Force or stress
     */
    enum class StressOrForce{
      Stress = 0,
      Force = 1
    };

    /**
     * @enum Event
     * @brief Defines the events which can occur for the AutoStretch FSM.
     */
    enum Event{evStart,         /**< Experiment should start */
               evUpdate,        /**< New measured value */
               evStop,					/**< Experiment should stop */
               evDistanceUpdate,/**< New distance value */
               evForceUpdate};	/**< New force value */

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
    Experiment(Experiment::ExperimentType type,
               Experiment::StressOrForce forceOrStress,
               Experiment::Direction direction,
               double forcesStressThreshold, double distanceThreshold,
               long currentdistance = 0);

    /**
     * @brief FSM of the experiment
     * @param e Occuring event
     */
    virtual void process(Event e) = 0;

    /**
     * @brief Returns the type of experiment
     * @return The type of experiment
     */
    ExperimentType getExperimentType(){
      return(m_ExperimentType);
    }

    /**
     * @brief Defines if experiment is force or stress based.
     * @param forceOrStress
     */
    void setForceOrStress(StressOrForce forceOrStress){
      m_StressOrForce = forceOrStress;
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

    double m_ForceStressThreshold;							/**< Threshold for the comparison */
    double m_DistanceThreshold;									/**< Threshold for the coparison of distances */
    Direction m_CurrentDirection;								/**< The current direction */
    ExperimentType m_ExperimentType;						/**< Type of the experiment */
    StressOrForce m_StressOrForce;							/**< Defines if the experiment is force or stress based. */

    long m_CurrentForce;												/**< Current force */
    std::vector<long> m_CurrentPositions;				/**< Vector with the current stage positions */
    long m_CurrentDistance;											/**< Current distance of the stage frame */
    bool m_ExitFlag;														/**< Flag indicating that the experiment should stop imediatly */

};

#endif // EXPERIMENT_H
