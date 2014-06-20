#ifndef PROTOCOLS_H
#define PROTOCOLS_H

// Includes
#include <vector>
#include <wx/listbox.h>
#include <wx/log.h>
#include <mathplot.h>
#include <thread>
#include "experiments/experiment.h"

// Forward declaration
class MyFrame;

/**
 * @brief Protocols class which handle all the containing experiments and the required methods.
 */
class Protocols
{
  public:
    Protocols(wxListBox *listbox,
              MyFrame *myframe,
              bool *stagesstoppedflag,
              std::mutex *stagesstoppedmutex,
              std::mutex *waitmutex,
              std::condition_variable *wait,
              bool *preloaddoneflag,
              std::mutex *preloaddonemutex,
              bool loopflag,
              double area,
              long maxdistance,
              long mindistance,
              long maxforce,
              long minforce,
              mpFXYVector *forceStressDistanceGraph, mpFXYVector *forceStressDisplacementGraph,
              mpFXYVector *stressForceGraph,
              mpFXYVector *distanceGraph,
              mpFXYVector *maxStressForceLimitGraph,
              mpFXYVector *minStressForceLimitGraph,
              mpFXYVector *maxDistanceLimitGraph,
              mpFXYVector *minDistanceLimitGraph,
              std::string path);

    /**
     * @brief Destructor
     */
    ~Protocols();

    /**
     * @brief Set new limits and forwards them to the experiments.
     * @param mindistancelimit Value for the minimal distance limit.
     * @param maxdistancelimit Value for the maximal distance limit.
     * @param minforcelimit Value for the minimal force limit.
     * @param maxforcelimit Value for the maximal force limit.
     */
    void setLimits(long mindistancelimit, long maxdistancelimit, long minforcelimit, long maxforcelimit);

    /**
     * @brief Sets the cross section area.
     * @param crosssectionarea The cross section area.
     */
    void setCrossSectionArea(double crosssectionarea);

    void loadProtocol(void);

    void saveProtocol(void);

    /**
     * @brief Create the preview vector and display it in the graph.
     */
    void makePreview(void);

    /**
     * @brief Get the preview values.
     */
    void getPreviewValues(void);

    /**
     * @brief Runs the protocol.
     */
    void runProtocol(void);

    /**
     * @brief Checks if protocol exceeds some limits.
     */
    bool checkProtocol(void);

    /**
     * @brief Process the next experiments.
     */
    void process(void);

    /**
     * @brief Stops the protocol.
     */
    void stopProtocol(void);

    /**
     * @brief Moves the experiment from the current position to the one before.
     * @param experimentPosition The current experiment position.
     */
    void moveExperimentUp(int experimentPosition);

    /**
     * @brief Moves the experiment from the current position to the one after.
     * @param experimentPosition The current experiment position.
     */
    void moveExperimentDown(int experimentPosition);

    /**
     * @brief Adds an experiment.
     * @param experiment Pointer to the experiment object.
     */
    void addExperiment(std::unique_ptr<Experiment> &experiment);

    /**
     * @brief Removes the Experiment at the desired position.
     * @param experimentPosition Position of the experiment.
     */
    void removeExperiment(int experimentPosition);

    /**
     * @brief Sets the experiment start time point.
     */
    void setStartPoint(void);

    /**
     * @brief Returns the experiment names as a vector.
     * @return The experiment names as a vector.
     */
    std::vector<std::string> getExperimentNames(void);

    /**
     * @brief Exports the measurement data to a .csv file.
     */
    void exportCSV(std::vector<bool> disableexport, std::string pathname = "");

    /**
     * @brief Returns the current experiment type as a string.
     * @return The current experiment type as a string.
     */
    std::string experimentTypeToString();

    /**
     * @brief Sets the m_ExperimentRunningFlag false if experiment is finished and the stages stopped and record preload distance if a preloading happend.
     */
    void checkFinishedExperiment(void);

    /**
     * @brief Executed by the object main frame when the clear graph button is pressed. Stops the measurement.
     */
    void clearGraphStop(void);

    /**
     * @brief Sets the loop flag.
     * @param flag
     */
    void setLoopFlag(bool flag){
      m_LoopProtocolFlag = flag;
      wxLogMessage(std::string("Protocols: LoopFlag: " + std::to_string(m_LoopProtocolFlag)).c_str());
    }

  private:
    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    mpFXYVector *m_ForceStressDistanceGraph;																/**< Pointer to the vector containing the force/stress - distance values. */
    mpFXYVector *m_ForceStressDisplacementGraph;														/**< Pointer to the vector containing the force/stress - displacement values. */
    mpFXYVector *m_StressForcePreviewGraph;																	/**< Pointer to the vector containing the stress/force preview values. */
    mpFXYVector *m_DistancePreviewGraph;																		/**< Pointer to the vector containing the distance preview values. */
    mpFXYVector *m_MaxStressForceLimitGraph;																/**< Pointer to the vector containing the max. stress/force limits. */
    mpFXYVector *m_MinStressForceLimitGraph;																/**< Pointer to the vector containing the min. stress/force limits. */
    mpFXYVector *m_MaxDistanceLimitGraph;																		/**< Pointer to the vector containing the max. distance limits. */
    mpFXYVector *m_MinDistanceLimitGraph;																		/**< Pointer to the vector containing the min. distance limits. */
    std::vector<double> m_TimePointLimits;																	/**< Vector for the limits timepoints. */
    std::vector<double> m_MaxStressForceLimits;															/**< Vector for the max. stress/force limits. */
    std::vector<double> m_MinStressForceLimits;															/**< Vector for the min. stress/force limits. */
    std::vector<double> m_MaxDistanceLimits;																/**< Vector for the max. distance limits. */
    std::vector<double> m_MinDistanceLimits;																/**< Vector for the min. distance limits. */
    wxListBox *m_ListBox;																										/**< Pointer to the listbox object. */
    std::vector<std::shared_ptr<Experiment>> m_Experiments;									/**< Vector containing the pointers to the experiments. */
    Experiment *m_CurrentExperiment;																				/**< Pointer to the current experiment */
    int m_CurrentExperimentNr;																							/**< Number of the current experiment. */
    std::unique_ptr<std::thread> m_ExperimentRunningThread;									/**< Pointer to the experiment running check thread */
    double m_Area;																													/**< Area size of the sample. */
    long m_MaxDistanceLimit;																								/**< The maximal position for the stages */
    long m_MinDistanceLimit;																								/**< The minimal position for the stages */
    long m_MaxForceLimit;																										/**< The maximal allowed force. */
    long m_MinForceLimit;																										/**< The minimal allowed force. */
    std::vector<std::shared_ptr<ExperimentValues>> m_ExperimentValues;			/**< Vector containing the pointers to the experiment values. */
    std::vector<Experiment::PreviewValue> m_PreviewValues;									/**< Vector containing the preview values. */
    std::vector<double> m_StressForcePreviewValues;													/**< Vector containing the stress/force preview values. */
    std::vector<double> m_DistancePreviewValues;														/**< Vector containing the distance preview values. */
    std::vector<double> m_StressForceTimePreviewValues;											/**< Vector containing the time points for the stress/force values. */
    std::vector<double> m_DistanceTimePreviewValues;												/**< Vector containing the time points for the distance values. */
    std::vector<double> m_StressForceGraphValues;														/**< Vector containing only the stress/force values */
    std::vector<double> m_DistanceGraphValues;															/**< Vector containing only the distance values */
    std::vector<double> m_DisplacementGraphValues;													/**< Vector containing only the displacment values */
    std::string m_StoragePath;																							/**< Storage path as a std::string */
    std::chrono::high_resolution_clock::time_point m_StartTimePoint;				/**< Start time point of the experiment. */

    bool m_LoopProtocolFlag;																								/**< Indicates if the protocol should be looped. */
    bool m_StopProtocolFlag;																								/**< Indicates if the protocol should be stopped. */
    std::mutex *m_WaitMutex;																								/**< Mutex to protect m_Wait */
    std::condition_variable *m_Wait;																				/**< Wait condition variable to wait for the end of an experiment */
    bool m_ExperimentRunningFlag;																						/**< Flag which indicates if an experiment is running */
    std::mutex m_ExperimentRunningMutex;																		/**< Mutex to protect m_ExperimentRunningFlag */
    bool *m_PreloadDoneFlag;																								/**< Indicates if preloading is done */
    std::mutex *m_PreloadDoneMutex;																					/**< Mutex to protect m_PreloadDoneFlag */
    bool *m_StagesStoppedFlag;																							/**< Flag indicating if stages stopped or not. */
    std::mutex *m_StagesStoppedMutex;																				/**< Mutex for m_StagesStoppedFlag */
    long m_PreloadDistance;																									/**< Preload distance */
    bool m_MeasurementValuesRecordingFlag;																	/**< Indicates if the measurement values are recorded or not. */
    std::mutex m_MeasurementValuesRecordingMutex; 													/**< Mutex to protect m_MeasurementValuesRecordingFlag */
    ExperimentValues *m_CurrentExperimentValues;														/**< Pointer to the current experiment values */
};
#endif // PROTOCOLS_H
