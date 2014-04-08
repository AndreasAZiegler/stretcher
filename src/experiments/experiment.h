
#ifndef EXPERIMENT_H
#define EXPERIMENT_H

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
    enum class ForceOrStress{
      Force = 0,
      Stress = 1
    };

    /**
     * @enum Event
     * @brief Defines the events which can occur for the AutoStretch FSM.
     */
    enum Event{evStart,         /**< AutoStretch should start */
               evUpdate,        /**< New measured value */
               evStop};         /**< AutoStretch should stop */

    /**
     * @brief Initializes the experiment type and if experiment is force or stress based.
     * @param type Type of experiment.
     * @param forceOrStress Force or stress.
     */
    Experiment(Experiment::ExperimentType type, Experiment::ForceOrStress forceOrStress);

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
    void setForceOrStress(ForceOrStress forceOrStress){
      m_ForceOrStress = forceOrStress;
    }

	private:

    ExperimentType m_ExperimentType;						/**< Type of the experiment */
    ForceOrStress m_ForceOrStress;							/**< Defines if the experiment is force or stress based. */

};

#endif // EXPERIMENT_H
