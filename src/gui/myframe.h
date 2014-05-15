#ifndef MYFRAME_H
#define MYFRAME_H

/*************** Includes ************/
#include <thread>
#include <wx/event.h>
#include <pugixml/pugixml.hpp>
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

// Forward declaration
class Protocols;

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
     * @brief Returns the message handler wait condition variable as shared_ptr.
     * @return The message handler wait condition variable as shared_ptr.
     */
    std::shared_ptr<std::condition_variable> getMessageHandlersWait(void);

    /**
     * @brief Returns the message handler wait mutex as shared_ptr.
     * @return The message handler wait mutex as shared_ptr.
     */
    std::shared_ptr<std::mutex> getMessageHandlersWaitMutex(void);

    /**
     * @brief Returns the number of finished message handler variable as shared_ptr.
     * @return The number of finished message handler variable as shared_ptr.
     */
    std::shared_ptr<int> getMessageHandlersFinishedNumber(void);

    /**
     * @brief Checks if a protocol object is already created, otherwise creates it.
     */
    void checkProtocol(void);

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
     * @brief Prepares the graph to show the experiment values.
     */
    void showValuesGraph(void);

    /**
     * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
     * 				thread. (CallAfter() asynchronously call the updateDistance method)
     * @param value The position of a stage or a force.
     * @param type	Defines the type of the value (position of stage 1, 2 or force)
     */
    virtual void updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type);

    /**
     * @brief Method which will be called from the class ExperimentValues to update the graph. Executes updateGraph() from the main thread.
     */
    void updateGraphFromExperimentValues(void);

    /**
     * @brief Method which will be called from the class Protocols to create the preview graph. Executes createPreviewGraph() from the main thread.
     */
    void showPreviewGraph(void);

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

    /**
     * @brief Method wich will be executed, when the decrease stage distance button is pushed down.
     * @param event Occuring event
     */
    void OnMotorDecreaseDistanceStart(wxCommandEvent &event);

    /**
     * @brief Method wich will be executed, when the decrease stage distance button is released.
     * @param event Occuring event
     */
    void OnMotorDecreaseDistanceStop(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the increase stage distance button is pushed down.
     * @param event Occuring event
     */
    void OnMotorIncreaseDistanceStart(wxCommandEvent &event);

    /**
     * @brief Method wich will be executed, when the increase stage distance button is released.
     * @param event Occuring event
     */
    void OnMotorIncreaseDistanceStop(wxCommandEvent& event);

    /**
     * @brief Returns the current distance.
     * @return The current distance.
     * @todo May move this method to a better place/class.
     */
    long getCurrentDistance(void){
      return(m_CurrentDistance);
    }

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
     * @brief Method wich will be executed, when the user klicks on load stored position button.
     * @param event Occuring event
     */
    void OnInitializeLoadStoredPosition(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the home stage button.
     * @param event Occuring event
     */
    void OnInitializeHomeLinearStages(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the set length button.
     * @param event Occuring event
     */
    void OnInitializeSetZeroLength(wxCommandEvent& event);

    /**
     * @brief Loads limit set 1 from the configuration.
     * @param event Occuring event
     */
    void OnClampingPosLoadSet1(wxCommandEvent& event);

    /**
     * @brief Loads limit set 2 from the configuration.
     * @param event Occuring event
     */
    void OnClampingPosLoadSet2(wxCommandEvent& event);

    /**
     * @brief Loads limit set 3 from the configuration.
     * @param event Occuring event
     */
    void OnClampingPosLoadSet3(wxCommandEvent& event);

    /**
     * @brief Loads limit set 4 from the configuration.
     * @param event Occuring event
     */
    void OnClampingPosLoadSet4(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user sets the limits.
     * @param event Occuring event
     */
    void OnClampingPosSetLimits(wxCommandEvent& event);

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
     * @brief Method wich will be executed, when the user clicks on the delete experiment button.
     * @param event Occuring event
     */
    void OnDeleteExperiment(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the move experiment up button.
     * @param event Occuring event
     */
    void OnMoveUpExperiment(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the move experiment down button.
     * @param event Occuring event
     */
    void OnMoveDownExperiment(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the run protocol button.
     * @param event Occuring event
     */
    void OnRunProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the stop protocol button.
     * @param event Occuring event
     */
    void OnStopProtocol(wxCommandEvent& event);

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
    void updateGraph(void);

    /**
     * @brief Creates the preview graph.
     */
    void createPreviewGraph(void);

    mpWindow *m_Graph;													/**< Pointer to the graph */
    mpFXYVector m_VectorLayer;									/**< Vector layer for the value graph */
    mpFXYVector m_StressForcePreviewVector;			/**< Vector layer for the stress/force preview graph. */
    mpFXYVector m_DistancePreviewVector;				/**< Vector layer for the distance preview graph. */
    std::mutex m_VectorLayerMutex;							/**< Mutex to protect m_VectorLayer */
    mpScaleX *m_XAxis;													/**< Pointer to the X axis */
    mpScaleY *m_Y1Axis;													/**< Pointer to the left Y axis */
    mpScaleY *m_Y2Axis;													/**< Pointer to the right Y axis */
    Settings *m_Settings;												/**< Pointer to the settings object */
    std::vector<LinearStage*> *m_LinearStages;	/**< Vector containing the pointers to the linear stages */
    std::vector<LinearStageMessageHandler*> *m_LinearStagesMessageHandlers; /**< Vector containing the pointer to the message handlers of the liner stages */
    int m_MessageHandlersFinishedNumber;
    std::condition_variable m_WaitMessageHandlers;
    std::mutex m_WaitMessageHandlersMutex;
    StageFrame *m_StageFrame;										/**< Pointer to the stage frame object */
    long m_StageMaxLimit;												/**< The maximal position for the stages */
    long m_StageMinLimit;												/**< The minimal position for the stages */
    long m_ForceMaxLimit;												/**< The maximal allowed force. */
    long m_ForceMinLimit;												/**< The minimal allowed force. */
    ForceSensor *m_ForceSensor;									/**< Pointer to the force sensor */
    ForceSensorMessageHandler *m_ForceSensorMessageHandler; /**< Pointer to the force sensor message handler */
    std::vector<int> m_CurrentPositions;				/**< Vector with the current stage positions */
    long m_CurrentDistance; 										/**< Current distance */
    std::unique_ptr<Protocols> m_CurrentProtocol;								/**< Pointer to the current protocol */
    //Experiment *m_CurrentExperiment;						/**< Pointer to the current experiment */
    //ExperimentValues *m_CurrentExperimentValues;/**< Pointer to the current experiment values */
    std::mutex m_WaitMutex;											/**< Mutex to protect m_Wait */
    std::condition_variable m_Wait;							/**< Wait condition variable to wait for the end of an experiment */
    bool m_StagesStoppedFlag;										/**< Flag indicating if stages stopped or not. */
    std::mutex m_StagesStoppedMutex;						/**< Mutex for m_StagesStoppedFlag */
    bool m_PreloadDoneFlag;											/**< Indicates if preloading is done */
    std::mutex m_PreloadDoneMutex;							/**< Mutex to protect m_PreloadDoneFlag */
    bool m_MeasurementValuesRecordingFlag;			/**< Indicates if the measurement values are recorded or not. */
    std::mutex m_MeasurementValuesRecordingMutex; /**< Mutex to protect m_MeasurementValuesRecordingFlag */
    double m_Area;															/**< Area of the sample */

    StressOrForce m_StressOrForce;							/**< Indicates if experiment is force or stress based */
    long m_CurrentForce;												/**< Current force */
    int m_CurrentForceUpdateDelay;							/**< Counting variable that the force values is not updated always in the GUI. */
    wxString m_ForceUnit;												/**< Current force unit (N or kPa) */
    double m_ClampingDistance;									/**< Clamping distance */
    std::string m_StoragePath;									/**< Path were the measurement values will be saved as a std::string. */

    wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_SamplingFrequency = 1,
	ID_Ports = 2,
	ID_FileOutput = 3,
  ID_Unit = 4,
  ID_DistanceLimit = 5,
  ID_StressLimit = 6,
  ID_GoTo = 7,
  ID_ChamberMeasurement = 8,
  ID_MotorDecreaseDistance = 9,
  ID_MotorIncreaseDistance = 10,
  ID_MotorStop = 11,
  ID_HomeStages = 12,
  ID_SetZeroLength = 13,
  ID_LoadLimitSet1 = 14,
  ID_LoadLimitSet2 = 15,
  ID_LoadLimitSet3 = 16,
  ID_LoadLimitSet4 = 17,
  ID_ClampingPosValue = 18,
  ID_ClampingGoTo = 19,
  ID_PreloadSpeedPercent = 20,
  ID_PreloadSpeedMm = 21,
  ID_PreloadSendToProtocol = 22,
  ID_ConditioningSpeedPercent = 23,
  ID_ConditioningSpeedMm = 24,
  ID_ConditioningSendToProtocol = 25,
  ID_Ramp2FailureSpeedPercent = 26,
  ID_Ramp2FailureSpeedMm = 27,
  ID_Ramp2FailureSendToProtocol = 28,
  ID_RelaxationSendToProtocol = 29,
  ID_CreepSpeedPercent = 30,
  ID_CreepSpeedMm = 31,
  ID_CreepSendToProtocol = 32,
  ID_FatigueSendToProtocol = 33,
  ID_ChamberStretchGelOrCell = 34,
  ID_ChamberStretchSendToProtocol = 35,
  ID_ClearGraph = 36,
  ID_ExportCSV = 37,
  ID_SetLimits = 38,
  ID_LoadStoredPosition = 39,
  ID_DeleteExperiment = 43,
  ID_MoveUpExperiment = 44,
  ID_MoveDownExperiment = 45,
  ID_LoopProtocol = 46,
  ID_RunProtocol = 47,
  ID_StopProtocol = 48,
  ID_SaveProtocol = 49,
  ID_LoadProtocol = 50,
  ID_MakePhoto = 51
};

#endif // MYFRAME_H
