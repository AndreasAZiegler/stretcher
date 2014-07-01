
#ifndef EXPERIMENT_H
#define EXPERIMENT_H

// Includes
#include <vector>
#include <mathplot.h>
#include "pugixml/pugixml.hpp"
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
    enum class Direction{Forwards,
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
               MyFrame *myframe,
               long maxforcelimit,
               long minforcelimit,
               long maxdistancelimit,
               long mindistancelimit,

               ExperimentType type,
               DistanceOrStressOrForce distanceOrStressForce,
               Experiment::Direction direction,
               long gagelength,
               long mountinglength,
               long maxposdistance,
               long currentdistance,
               double area,
               double forcesStressThreshold,
               double distanceThreshold);

    /**
     * @brief Set new limits and forwards them to the experiments.
     * @param mindistancelimit Value for the minimal distance limit.
     * @param maxdistancelimit Value for the maximal distance limit.
     * @param minforcelimit Value for the minimal force limit.
     * @param maxforcelimit Value for the maximal force limit.
     */
    void setLimits(long mindistancelimit, long maxdistancelimit, long minforcelimit, long maxforcelimit);

    /**
     * @brief Sets the preload distance.
     */
    virtual void setPreloadDistance(void) = 0;

    /**
     * @brief Sets the start length.
     */
    virtual void setStartLength(void);

    /**
     * @brief Destructor
     */
    virtual ~Experiment();

    /**
     * @brief Saves the experiment settings in the xml_docuement.
     * @param xml Pointer to the xml_document.
     */
    virtual void getXML(pugi::xml_document &xml) = 0;

    /**
     * @brief Saves the points required to cread a preview graph in the vector..
     * @param previewvalues Vector containing the preview values.
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
    MyFrame *m_MyFrame;													/**< Pointer to the main frame. */

    double m_Area;															/**< Cross section area. */
    bool m_CheckLimitsFlag;											/**< Indicates if the limits should be checked. */
    long m_MaxForceLimit;												/**< Maximal force limit. */
    long m_MinForceLimit;												/**< Minimal force limit. */
    long m_MaxDistanceLimit;										/**< Maximal distance limit. */
    long m_MinDistanceLimit;										/**< Minimal distance limit. */
    double m_ForceStressThreshold;							/**< Threshold for the comparison */
    double m_DistanceThreshold;									/**< Threshold for the coparison of distances */
    Direction m_CurrentDirection;								/**< The current direction */
    ExperimentType m_ExperimentType;						/**< Type of the experiment */
    DistanceOrStressOrForce m_DistanceOrStressOrForce; /**< Defines if the experiment is distance of stress/force based. */

    long m_GageLength;													/**< Preload distance of the stage frame */
    long m_MountingLength;											/**< Mountinglength of the stage frame */
    long m_DefaultGageLength;										/**< Default preload distance of the stage frame. */
    long m_MaxPosDistance;												/**< Zero distance */
    long m_StartLength;													/**< Distance where the experiment starts. */
    long m_CurrentDistance;											/**< Current distance of the stage frame */
    double m_CurrentForce;											/**< Current force */
    std::vector<long> m_CurrentPositions;				/**< Vector with the current stage positions */
    bool m_ExitFlag;														/**< Flag indicating that the experiment should stop imediatly */

};

#endif // EXPERIMENT_H
