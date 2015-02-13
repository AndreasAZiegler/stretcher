/**
 * @file main.h
 * @brief The protocol.
 * @author Andreas Ziegler
 */

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
 * @class Protocols protocols.h "protocols.h"
 * @brief Protocols class which handle all the containing experiments and the required methods.
 */
class Protocols
{
  public:
    /**
     * @brief Initializes all the needed variables.
     * @param listbox Pointer to the wxListBox object.
     * @param myframe Pointer to the main frame object.
     * @param stageframe Pointer to the stage frame object.
     * @param forcesensormessagehandler Pointer to the force sensor message hander object.
     * @param vectoraccessmutex Pointer to the mutex to protect the vectors.
     * @param gagelength The gage length.
     * @param mountinglength The mounting length.
     * @param maxposdistance The distance at maximal positions.
     * @param currentdistance The current distance.
     * @param stagesstoppedflag Pointer to the flag that indicates that the stages stopped
     * @param stagesstoppedmutex Pointer to the mutex to protect stagesstoppedflag.
     * @param waitmutex Pointer to the mutex for the condition variable wait.
     * @param wait Pointer to the condition variable to wait for finished experiments.
     * @param preloaddoneflag Pointer to the flag that indicates that preloading is done.
     * @param preloaddonemutex Pointer to the mutex to protect preloaddoneflag.
     * @param loopflag Flag to indicate that the protocol should be looped.
     * @param area Cross section area.
     * @param maxdistance Maximal distance.
     * @param mindistance Minimal distance.
     * @param maxforce Maximal force.
     * @param minforce Minimal force.
     * @param forceStressDistanceGraph Pointer to the force/stress distance graph.
     * @param forceStressDisplacementGraph Pointer to the force/stress displacement graph.
     * @param stressForceGraph Pointer to the stress/force graph.
     * @param distanceGraph Pointer to the distance graph.
     * @param maxStressForceLimitGraph Pointer to the maximal stress/force limit graph.
     * @param minStressForceLimitGraph Pointer to the minimal stress/force limit graph.
     * @param maxDistanceLimitGraph Pointer to the maximal distance limit graph.
     * @param minDistanceLimitGraph Pointer to the minimal distance limit graph.
     * @param path Path.
     */
    Protocols(wxListBox *listbox,
              MyFrame *myframe,
              std::shared_ptr<StageFrame> stageframe,
              std::shared_ptr<ForceSensorMessageHandler> forcesensormessagehandler,
              std::mutex *vectoraccessmutex,
              long gagelength,
              long mountinglength,
              long maxposdistance,
              long currentdistance,
              bool *stagesstoppedflag,
              std::mutex *stagesstoppedmutex,
              std::mutex *waitmutex,
              std::condition_variable *wait,
              bool *preloaddoneflag,
              std::mutex *preloaddonemutex,
              bool loopflag, int loopnumber,
              double area,
              long maxdistance,
              long mindistance,
              long maxforce,
              long minforce,
              long forcestresssensitivity,
              long distancesensitivity,
              mpFXYVector *forceStressDistanceGraph,
              mpFXYVector *forceStressDisplacementGraph,
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

    /**
     * @brief Sets the new sensitivities.
     * @param forcestresssensitivity The force/stress sensitivity.
     * @param distancesensitivity The distance sensitivity.
     */
    void setSensitivities(long forcestresssensitivity, long distancesensitivity);

    /**
     * @brief Load a protocol from a file.
     * @param path Path of the protocol file.
     * @param gagelength Gage length
     * @param mountinglength Mounting length
     * @param maxposdistance Maximal position distance
     * @param currentdistance Current distance
     */
    void loadProtocol(std::string path, long gagelength, long mountinglength, long maxposdistance, long currentdistance);

    /**
     * @brief Saves the current protocol to the desired place as an .xml file.
     */
    void saveProtocol(std::string path);

    /**
     * @brief Remembers, which experiment will be changed and returns the experiment type.
     * @return The experiment type.
     */
    ExperimentType getEditExperimentType(void);

    /**
     * @brief Returns a reference to the experiment which will be edited.
     * @return The reference to the experiment which will be edited.
     */
    std::shared_ptr<Experiment> getEditExperiment(void){
      return(m_Experiments[m_EditedExperiment]);
    }

    /**
     * @brief Updates the parameters of the edited experiment in the wxListBox.
     */
    void updateEditedExperimentParameters(void);

    /**
     * @brief Create the preview vector and display it in the graph.
     */
    void makePreview(void);

    /**
     * @brief Get the preview values.
     */
    void getPreviewValues(void);

    /**
     * @brief Get the values for the limit check.
     */
    void getLimitValues(void);

    /**
     * @brief Get the values from the experiments.
     */
    void getValues(void);

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
     * @brief Activates & deactivated the graph in the experiment values class.
     * @param flag true = activated / false = deactivated.
     */
    void acDeactivateGraph(bool flag);

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
     * @brief Checks if there is any data to export.
     * @return True if there is some data / False otherwise.
     */
    bool hasData(void);

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
    void setLoopFlag(bool flag, int loopnumber = 0){
      m_LoopProtocolFlag = flag;
      m_LoopNumber = loopnumber;
      wxLogMessage(std::string("Protocols: LoopFlag: " + std::to_string(m_LoopProtocolFlag)).c_str());
    }

  private:

    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    std::shared_ptr<StageFrame> m_StageFrame;																/**< Pointer to the stage frame object */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler;	/**< Pointer to the message handler object */
    std::mutex *m_VectorLayerMutex;																					/**< Pointer to the mutex to protect m_VectorLayer */
    mpFXYVector *m_ForceStressDistanceGraph;																/**< Pointer to the vector containing the force/stress - distance values. */
    mpFXYVector *m_ForceStressDisplacementGraph;														/**< Pointer to the vector containing the force/stress - displacement values. */
    mpFXYVector *m_StressForcePreviewGraph;																	/**< Pointer to the vector containing the stress/force preview values. */
    mpFXYVector *m_DistancePreviewGraph;																		/**< Pointer to the vector containing the distance preview values. */
    mpFXYVector *m_MaxStressForceLimitGraph;																/**< Pointer to the vector containing the max. stress/force limits. */
    mpFXYVector *m_MinStressForceLimitGraph;																/**< Pointer to the vector containing the min. stress/force limits. */
    mpFXYVector *m_MaxDistanceLimitGraph;																		/**< Pointer to the vector containing the max. distance limits. */
    mpFXYVector *m_MinDistanceLimitGraph;																		/**< Pointer to the vector containing the min. distance limits. */
    std::vector<double> m_PreviewStressForceLimitTimePoints;								/**< Vector for the preview and stress/force limits timepoints. */
    std::vector<double> m_DistanceLimitTimePoints;													/**< Vector for the distance limits timepoints. */
    std::vector<double> m_MaxStressForceLimits;															/**< Vector for the max. stress/force limits. */
    std::vector<double> m_MinStressForceLimits;															/**< Vector for the min. stress/force limits. */
    std::vector<double> m_MaxDistanceLimits;																/**< Vector for the max. distance limits. */
    std::vector<double> m_MinDistanceLimits;																/**< Vector for the min. distance limits. */
    wxListBox *m_ListBox;																										/**< Pointer to the listbox object. */
    std::vector<std::shared_ptr<Experiment>> m_Experiments;									/**< Vector containing the pointers to the experiments. */
    int m_CurrentExperimentNr;																							/**< Number of the current experiment. */
    int m_EditedExperiment;																									/**< Number of the experiment which will be edited. */
    std::unique_ptr<std::thread> m_ExperimentRunningThread;									/**< Pointer to the experiment running check thread */
    double m_Area;																													/**< Area size of the sample. */
    long m_MaxDistanceLimit;																								/**< The maximal position for the stages */
    long m_MinDistanceLimit;																								/**< The minimal position for the stages */
    long m_MaxForceLimit;																										/**< The maximal allowed force. */
    long m_MinForceLimit;																										/**< The minimal allowed force. */
    long m_ForceStressSensitivity;																					/**< The force/stress sensitivity. */
    long m_DistanceSensitivity;																							/**< The distance sensitivity. */
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
    int m_LoopNumber;																															/**< The amount of times the protocol should be looped. */
    int m_CurrentLoopNumber;																								/**< The current loop number. */
    bool m_StopProtocolFlag;																								/**< Indicates if the protocol should be stopped. */
    std::mutex *m_WaitMutex;																								/**< Mutex to protect m_Wait */
    std::condition_variable *m_Wait;																				/**< Wait condition variable to wait for the end of an experiment */
    bool m_ExperimentRunningFlag;																						/**< Flag which indicates if an experiment is running */
    std::mutex m_ExperimentRunningMutex;																		/**< Mutex to protect m_ExperimentRunningFlag */
    bool *m_PreloadDoneFlag;																								/**< Indicates if preloading is done */
    std::mutex *m_PreloadDoneMutex;																					/**< Mutex to protect m_PreloadDoneFlag */
    bool *m_StagesStoppedFlag;																							/**< Flag indicating if stages stopped or not. */
    std::mutex *m_StagesStoppedMutex;																				/**< Mutex for m_StagesStoppedFlag */
    long m_GageLength;																											/**< Preload distance of the stage frame */
    long m_MountingLength;																									/**< Mountinglength of the stage frame */
    long m_MaxPosDistance;																									/**< Zero distance */
    long m_CurrentDistance;																									/**< Current distance of the stage frame */
    long m_PreloadDistance;																									/**< Preload distance */
    bool m_MeasurementValuesRecordingFlag;																	/**< Indicates if the measurement values are recorded or not. */
    std::mutex m_MeasurementValuesRecordingMutex; 													/**< Mutex to protect m_MeasurementValuesRecordingFlag */
};
#endif // PROTOCOLS_H
