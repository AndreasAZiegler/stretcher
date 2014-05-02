#ifndef MYFRAME_H
#define MYFRAME_H

/*************** Includes ************/
#include <thread>
#include <wx/window.h>
#include <wx/string.h>
#include <condition_variable>
#include "myframe_base.h"
#include "../updatedvaluesreceiver.h"
#include "../settings.h"
#include "./hardware/linearstage.h"
#include "./hardware/stageframe.h"
#include "./hardware/forcesensor.h"
#include "./experiments/experiment.h"
#include "./experiments/experimentvalues.h"

/**
 * @brief The Main Frame class
 */
class MyFrame : public MyFrame_Base, public UpdatedValuesReceiver
{
  public:
    /**
     * @brief Constructor of the main frame. Sets the icon.
     * @param title Title of the software.
     * @param parent Pointer to the parent object.
     */
    MyFrame(const wxString& title, Settings *settings, wxWindow* parent = (wxWindow *)NULL);

    /**
     * @brief Register the liner stages and the stage frame, registers the update method at the stage frame and registers the stop wait conditional variable at the stage frame.
     * @param linearmotor Pointer to the vector containing the linear motors.
     */
    void registerLinearStage(std::vector<LinearStage *> *linearmotor, StageFrame *stageframe);

    /**
     * @brief Registers the message handlers of the linear stages.
     * @param linearstagesmessagehandlers Pointer to the vector containing the message handlers of the linear motors.
     */
    void registerLinearStageMessageHandlers(std::vector<LinearStageMessageHandler*> *linearstagesmessagehandlers);

    /**
     * @brief Register the force sensor.
     * @param forcesensor Pointer to the force sensor.
     */
    void registerForceSensor(ForceSensor *forcesensor);

    /**
     * @brief Destructor
     */
    ~MyFrame();

    /**
     * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
     * 				thread. (CallAfter() asynchronously call the updateDistance method)
     * @param value The position of a stage or a force.
     * @param type	Defines the type of the value (position of stage 1, 2 or force)
     */
    virtual void updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Executes updateGraph() from the main threa.
     */
    void updateGraphFromExperimentValues(void);

    /**
     * @brief Hides calculate diameter options, hides cells panel in chamber stretch, hides distance limit options, hides go to options,
     * 				sets digits for the wxSpinCtrlDouble.
     */
    void startup(void);

    /**
     * @brief Updates the storage path from the GUI.
     * @param path Path as a std::string
     */
    void updateStoragePath(std::string path);

  private:
    /**
     * @brief Method wich will be executed, when the software will be closed by the user.
     * @param event Occuring event
     */
    void OnExit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the frequency settings.
     * @param event Occuring event
     */
    void OnSamplingFrequencySettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the port settings.
     * @param event Occuring event
     */
    void OnPortsSettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the file output settings.
     * @param event Occuring event
     */
    void OnFileOutputSettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user activates the calculate diameter settings.
     * @param event Occuring event
     */
    void OnCalculateDiameter(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes unit.
     * @param event Occuring event
     */
    void OnUnit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses distance as limit.
     * @param event Occuring event
     */
    void OnDistanceLimit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses stress as limit.
     * @param event Occuring event
     */
    void OnStressLimit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses "Go to" after failure in R2F.
     * @param event Occuring event
     */
    void OnGoTo(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses a measurement in Chamber stretch.
     * @param event Occuring event
     */
    void OnChamberMeasurement(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the home stage button.
     * @param event Occuring event
     */
    void OnHomeLinearStages(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the clamping position value.
     * @param event Occuring event
     */
    void OnClampingPosValueChanged(wxSpinDoubleEvent &event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Go to" button in clamping position.
     * @param event Occuring event
     */
    void OnClampingGoTo(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in percent in preload.
     * @param event Occuring event
     */
    void OnPreloadSpeedPercentChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in mm in preload.
     * @param event Occuring event
     */
    void OnPreloadSpeedMmChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload.
     * @param event Occuring event
     */
    void OnPreloadSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in percent in conditioning.
     * @param event Occuring event
     */
    void OnConditioningSpeedPercentChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in mm in conditioning.
     * @param event Occuring event
     */
    void OnConditioningSpeedMmChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in conditioning.
     * @param event Occuring event
     */
    void OnConditioningSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in percent in ramp 2 failure.
     * @param event Occuring event
     */
    void OnRamp2FailureSpeedPercentChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in mm in ramp 2 failure.
     * @param event Occuring event
     */
    void OnRamp2FailureSpeedMmChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in ramp 2 failure.
     * @param event Occuring event
     */
    void OnRamp2FailureSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in relexation.
     * @param event Occuring event
     */
    void OnRelexationSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in percent in creep.
     * @param event Occuring event
     */
    void OnCreepSpeedPercentChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the speed value in mm in creep.
     * @param event Occuring event
     */
    void OnCreepSpeedMmChanged(wxSpinDoubleEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in creep.
     * @param event Occuring event
     */
    void OnCreepSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in fatigue.
     * @param event Occuring event
     */
    void OnFatigueSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes from gel to cells or vica versa in chamber stretch.
     * @param event Occuring event
     */
    void OnChamberGelCells(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in chamber stretch.
     * @param event Occuring event
     */
    void OnChamberStretchSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user sets the limits.
     * @param event Occuring event
     */
    void OnSetLimits(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the decrease distance button.
     * @param event Occuring event
     */
    void OnMotorDecreaseDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the increase distance button.
     * @param event Occuring event
     */
    void OnMotorIncreaseDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the motor stop button.
     * @param event Occuring event
     */
    void OnMotorStop(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the export csv button.
     * @param event Occuring event
     */
    void OnExportCSV(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the clear graph button.
     * @param event Occuring event
     */
    void OnClearGraph(wxCommandEvent& event);

    /**
     * @brief Calculates the distance and print the value in the GUI.
     */
    void updateDistance(void);

    /**
     * @brief Calculates the distance and print the value in the GUI.
     */
    void updateForce();

    /**
     * @brief Updates the graph in the GUI.
     */
    void updateGraph();

    /**
     * @brief Sets the m_ExperimentRunningFlag false if experiment is finished and the stages stopped and record preload distance if a preloading happend.
     */
    void checkFinishedExperiment();

    mpWindow* m_Graph;													/**< Pointer to the graph */
    mpFXYVector m_VectorLayer;									/**< Vector layer for the graph */
    std::mutex m_VectorLayerMutex;							/**< Mutex to protect m_VectorLayer */
    Settings *m_Settings;												/**< Pointer to the settings object */
    std::vector<LinearStage*> *m_LinearStages;	/**< Vector containing the pointers to the linear stages */
    std::vector<LinearStageMessageHandler*> *m_LinearStagesMessageHandlers; /**< Vector containing the pointer to the message handlers of the liner stages */
    StageFrame *m_StageFrame;										/**< Pointer to the stage frame object */
    long m_StageMaxLimit;												/**< The maximal position for the stages */
    long m_StageMinLimit;												/**< The minimal position for the stages */
    long m_ForceMaxLimit;												/**< The maximal allowed force. */
    long m_ForceMinLimit;												/**< The minimal allowed force. */
    ForceSensor *m_ForceSensor;									/**< Pointer to the force sensor */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler; /**< Pointer to the force sensor message handler */
    std::vector<int> m_CurrentPositions;				/**< Vector with the current stage positions */
    long m_CurrentDistance; 										/**< Current distance */
    Experiment *m_CurrentExperiment;						/**< Pointer to the current experiment */
    ExperimentValues *m_CurrentExperimentValues;/**< Pointer to the current experiment values */
    std::thread *m_ExperimentRunningThread;			/**< Pointer to the experiment running check thread */
    bool m_ExperimentRunningFlag;								/**< Flag which indicates if an experiment is running */
    std::mutex m_ExperimentRunningMutex;				/**< Mutex to protect m_ExperimentRunningFlag */
    bool m_PreloadDoneFlag;											/**< Indicates if preloading is done */
    std::mutex m_PreloadDoneMutex;							/**< Mutex to protect m_PreloadDoneFlag */
    bool m_MeasurementValuesRecordingFlag;			/**< Indicates if the measurement values are recorded or not. */
    std::mutex m_MeasurementValuesRecordingMutex; /**< Mutex to protect m_MeasurementValuesRecordingFlag */
    double m_Area;															/**< Area of the sample */
    std::condition_variable m_Wait;							/**< Wait condition variable to wait for the end of an experiment */
    std::mutex m_WaitMutex;											/**< Mutex to protect m_Wait */
    bool m_StagesStoppedFlag;										/**< Flag indicating if stages stopped or not. */
    std::mutex m_StagesStoppedMutex;						/**< Mutex for m_StagesStoppedFlag */

    StressOrForce m_StressOrForce;							/**< Indicates if experiment is force or stress based */
    long m_CurrentForce;												/**< Current force */
    int m_CurrentForceUpdateDelay;							/**< Counting variable that the force values is not updated always in the GUI. */
    wxString m_ForceUnit;												/**< Current force unit (N or kPa) */
    double m_ClampingDistance;									/**< Clamping distance */
    long m_PreloadDistance;											/**< Preload distance */

    std::string m_StoragePath;									/**< Path were the measurement values will be saved as a std::string. */

    wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_SamplingFrequency = 1,
	ID_Ports = 2,
	ID_FileOutput = 3,
	ID_CalculateDiameter = 4,
	ID_Unit = 5,
	ID_DistanceLimit = 6,
	ID_StressLimit = 7,
  ID_GoTo = 8,
  ID_ChamberMeasurement = 9,
  ID_MotorDecreaseDistance = 10,
  ID_MotorIncreaseDistance = 11,
  ID_MotorStop = 12,
  ID_HomeStages = 13,
  ID_ClampingPosValue = 14,
  ID_ClampingGoTo = 15,
  ID_PreloadSpeedPercent = 16,
  ID_PreloadSpeedMm = 17,
  ID_PreloadSendToProtocol = 18,
  ID_ConditioningSpeedPercent = 19,
  ID_ConditioningSpeedMm = 20,
  ID_ConditioningSendToProtocol = 21,
  ID_Ramp2FailureSpeedPercent = 22,
  ID_Ramp2FailureSpeedMm = 23,
  ID_Ramp2FailureSendToProtocol = 24,
  ID_RelaxationSendToProtocol = 25,
  ID_CreepSpeedPercent = 26,
  ID_CreepSpeedMm = 27,
  ID_CreepSendToProtocol = 28,
  ID_FatigueSendToProtocol = 29,
  ID_ChamberStretchGelOrCell = 30,
  ID_ChamberStretchSendToProtocol = 31,
  ID_ClearGraph = 32,
  ID_ExportCSV = 33,
  ID_SetLimits = 36
};

#endif // MYFRAME_H
