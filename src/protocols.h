#ifndef PROTOCOLS_H
#define PROTOCOLS_H

// Includes
#include <vector>
#include <wx/listbox.h>
#include <mathplot.h>
#include <thread>
#include "experiments/experiment.h"

// Forward declaration
class MyFrame;

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
              mpFXYVector *valuesvector,
              mpFXYVector *stressforcevector,
              mpFXYVector *distancevector,
              std::string path);

    /**
     * @brief Destructor
     */
    ~Protocols();

    void loadProtocol(void);

    void saveProtocol(void);

    /**
     * @brief Create the preview vector and display it in the graph.
     */
    void makePreview(void);

    /**
     * @brief Runs the protocol.
     */
    void runProtocol(void);

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
     * @brief Exports the measurement data to a .csv file.
     */
    void exportCSV(std::vector<bool> *disableexport);

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

  private:
    MyFrame *m_MyFrame;																											/**< Pointer to the main frame object. */
    mpFXYVector *m_ValuesVector;																						/**< Pointer to the vector containing the values. */
    mpFXYVector *m_StressForcePreviewVector;																/**< Pointer to the vector containing the stress/force preview values. */
    mpFXYVector *m_DistancePreviewVector;																		/**< Pointer to the vector containing the distance preview values. */
    wxListBox *m_ListBox;																										/**< Pointer to the listbox object. */
    std::vector<std::shared_ptr<Experiment>> m_Experiments;									/**< Vector containing the pointers to the experiments. */
    Experiment *m_CurrentExperiment;																				/**< Pointer to the current experiment */
    int m_CurrentExperimentNr;																							/**< Number of the current experiment. */
    std::thread *m_ExperimentRunningThread;																	/**< Pointer to the experiment running check thread */
    std::vector<std::shared_ptr<ExperimentValues>> m_ExperimentValues;											/**< Vector containing the pointers to the experiment values. */
    std::vector<Experiment::PreviewValue> m_PreviewValues;									/**< Vector containing the preview values. */
    std::vector<double> m_StressForcePreviewValues;													/**< Vector containing the stress/force preview values. */
    std::vector<double> m_DistancePreviewValues;														/**< Vector containing the distance preview values. */
    std::vector<double> m_StressForceTimePreviewValues;											/**< Vector containing the time points for the stress/force values. */
    std::vector<double> m_DistanceTimePreviewValues;												/**< Vector containing the time points for the distance values. */
    std::vector<double> m_GraphStressForceValues;														/**< Vector containing only the stress/force values */
    std::vector<double> m_GraphDistanceValues;															/**< Vector containing only the distance values */
    std::string m_StoragePath;																							/**< Storage path as a std::string */
    std::chrono::high_resolution_clock::time_point m_StartTimePoint;				/**< Start time point of the experiment. */

    std::mutex *m_WaitMutex;																									/**< Mutex to protect m_Wait */
    std::condition_variable *m_Wait;																					/**< Wait condition variable to wait for the end of an experiment */
    bool m_ExperimentRunningFlag;								/**< Flag which indicates if an experiment is running */
    std::mutex m_ExperimentRunningMutex;				/**< Mutex to protect m_ExperimentRunningFlag */
    bool *m_PreloadDoneFlag;											/**< Indicates if preloading is done */
    std::mutex *m_PreloadDoneMutex;							/**< Mutex to protect m_PreloadDoneFlag */
    bool *m_StagesStoppedFlag;										/**< Flag indicating if stages stopped or not. */
    std::mutex *m_StagesStoppedMutex;						/**< Mutex for m_StagesStoppedFlag */
    long m_PreloadDistance;											/**< Preload distance */
    bool m_MeasurementValuesRecordingFlag;			/**< Indicates if the measurement values are recorded or not. */
    std::mutex m_MeasurementValuesRecordingMutex; /**< Mutex to protect m_MeasurementValuesRecordingFlag */
    ExperimentValues *m_CurrentExperimentValues;/**< Pointer to the current experiment values */
};

#endif // PROTOCOLS_H