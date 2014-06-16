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
    void registerLinearStage(std::vector<std::shared_ptr<LinearStage> > &linearmotor, std::shared_ptr<StageFrame> &stageframe);

    /**
     * @brief Registers the message handlers of the linear stages.
     * @param linearstagesmessagehandlers Pointer to the vector containing the message handlers of the linear motors.
     */
    void registerLinearStageMessageHandlers(std::vector<std::shared_ptr<LinearStageMessageHandler> > &linearstagesmessagehandlers);

    /**
     * @brief Register the force sensor.
     * @param forcesensor Pointer to the force sensor.
     */
    void registerForceSensor(std::shared_ptr<ForceSensor> forcesensor);

    /**
     * @brief Destructor
     */
    ~MyFrame();

    /**
     * @brief Prepares the graph to show the experiment values.
     */
    void createValuesGraph(void);

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
     * @brief Method wich will be executed, when the user clicks on the set Le button.
     * @param event Occuring event
     */
    void setDistanceWActuatorCollision(double le);

    /**
     * @brief Returns the current distance.
     * @return The current distance.
     * @todo May move this method to a better place/class.
     */
    long getCurrentDistance(void){
      return(m_CurrentDistance);
    }

    /**
     * @brief Shows pause/resume dialog.
     */
    void showPauseResumeDialogFromPauseResume(std::condition_variable *wait, std::mutex *mutex);

  private:

    /**
     * @brief A wrapper of std::to_string to set precission.
     * @param a_value Variable which will be converted.
     * @param n Precision
     * @return Return value.
     */
    template <typename T>
    std::string to_string_wp(const T a_value, const int n = 6){
      std::ostringstream outstr;
      outstr.precision(n);
      outstr << a_value;
      return(outstr.str());
    }

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
     * @brief Method wich will be executed, when the user opens the start up dialog.
     * @param event Occuring event
     */
    void OnOpenStartUpDialog(wxCommandEvent& event);

    /**
     * @brief Method which will be executed, when the user click on the file picker to load a preset.
     * @param event Occuring event
     */
    void OnLoadPreset(wxFileDirPickerEvent &event);

    /**
     * @brief Method which will be executed, when the user applies a preset.
     * @param event Occuring event
     */
    void OnApplyPreset(wxCommandEvent& event);

    /**
     * @brief Method which will be executed, when the user saves a preset.
     * @param event Occuring event
     */
    void OnSavePreset(wxCommandEvent& event);

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
     * @brief Method wich will be executed when the user clicks on load stored positions.
     * @param event Occuring event
     */
    void OnLoadStoredPositions(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on load stored position button.
     * @param event Occuring event
     */
    void loadStoredPositions(void);

    /**
     * @brief Method wich will be executed, when the user klicks on the home stage button.
     * @param event Occuring event
     */
    void OnHomeLinearStages(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the set length button.
     * @param event Occuring event
     */
    void OnLengthsSetMountingLength(wxCommandEvent& event);

    /**
     * @brief Loads limit set 1 from the configuration.
     * @param event Occuring event
     */
    void OnLimitsLoadSet1(wxCommandEvent& event);

    /**
     * @brief Loads limit set 2 from the configuration.
     * @param event Occuring event
     */
    void OnLimitsLoadSet2(wxCommandEvent& event);

    /**
     * @brief Loads limit set 3 from the configuration.
     * @param event Occuring event
     */
    void OnLimitsLoadSet3(wxCommandEvent& event);

    /**
     * @brief Loads limit set 4 from the configuration.
     * @param event Occuring event
     */
    void OnLimitsLoadSet4(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user sets the limits.
     * @param event Occuring event
     */
    void OnLimitsSetLimits(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes the mounting length.
     * @param event Occuring event
     */
    void OnMountingLengthChanged(wxSpinDoubleEvent &event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Go to" button in limits.
     * @param event Occuring event
     */
    void OnLimitsGoTo(wxCommandEvent& event);

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
     * @brief Method wich will be executed, when the user chooses stress/force in the one step event.
     * @param event Occuring event
     */
    void OnOneStepStressForce(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses distance in the one step event.
     * @param event Occuring event
     */
    void OnOneStepDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in the one step event.
     * @param event Occuring event
     */
    void OnOneStepSendToProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses stress/force in the continuous event.
     * @param event Occuring event
     */
    void OnContinuousStressForce(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses distance in the contiunous event.
     * @param event Occuring event
     */
    void OnContinuousDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses max value in the contiunous event.
     * @param event Occuring event
     */
    void OnContinuousMaxValue(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses steps in the contiunous event.
     * @param event Occuring event
     */
    void OnContinuousSteps(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in the continuous event.
     * @param event Occuring event
     */
    void OnContinuousSendToProtocol(wxCommandEvent& event);

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
     * @brief Method wich will be executed, when the user clicks on the export png button.
     * @param event Occuring event
     */
    void OnExportPNG(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the clear log button.
     * @param event Occuring event
     */
    void OnClearLog(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the save log button.
     * @param event Occuring event
     */
    void OnSaveLog(wxCommandEvent& event);

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
     * @brief Method wich will be executed, when the user clicks on the pause experiment down button.
     * @param event Occuring event
     */
    void OnPauseExperiment(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the pause experiment down button.
     * @param event Occuring event
     */
    void OnPauseResumeExperiment(wxCommandEvent& event);

    /**
     * @brief Shows pause/resume dialog.
     */
    void showPauseResumeDialog(std::condition_variable *wait, std::mutex *mutex);

    /**
     * @brief Method wich will be executed, when the user clicks on the preview protocol button.
     * @param event Occuring event
     */
    void OnPreviewProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user clicks on the run protocol button.
     * @param event Occuring event
     */
    void OnRunProtocol(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user activates Loop in protocol.
     * @param event Occuring event
     */
    void OnLoopProtocol(wxCommandEvent& event);

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

    std::unique_ptr<mpWindow> m_Graph;					/**< Pointer to the graph */
    std::mutex m_UpdateGraphMutex;							/**< Mutex to protect the graph update. */
    bool m_ShowGraphFlag;												/**< Indicates if the graph is active or not. */
    mpFXYVector m_VectorLayer;									/**< Vector layer for the value graph */
    mpFXYVector m_StressForcePreviewVector;			/**< Vector layer for the stress/force preview graph. */
    mpFXYVector m_DistancePreviewVector;				/**< Vector layer for the distance preview graph. */
    mpFXYVector m_MaxStressForceLimitVector;		/**< Vector layer to display the max stress/force limits. */
    mpFXYVector m_MinStressForceLimitVector;		/**< Vector layer to display the min stress/force limits. */
    mpFXYVector m_MaxDistanceLimitVector;				/**< Vector layer to display the max distance limits. */
    mpFXYVector m_MinDistanceLimitVector;				/**< Vector layer to display the min distance limits. */
    std::mutex m_VectorLayerMutex;							/**< Mutex to protect m_VectorLayer */
    std::unique_ptr<mpScaleX> m_XAxis;					/**< Pointer to the X axis */
    std::unique_ptr<mpScaleY> m_Y1Axis;					/**< Pointer to the left Y axis */
    std::unique_ptr<mpScaleY> m_Y2Axis;					/**< Pointer to the right Y axis */
    Settings *m_Settings;												/**< Pointer to the settings object */
    std::vector<std::shared_ptr<LinearStage>> m_LinearStages;	/**< Vector containing the pointers to the linear stages */
    std::vector<std::shared_ptr<LinearStageMessageHandler>> m_LinearStagesMessageHandlers; /**< Vector containing the pointer to the message handlers of the liner stages */
    int m_MessageHandlersFinishedNumber;				/**< Number of finished message handlers. */
    std::condition_variable m_WaitMessageHandlers;/**< Wait condition variable to wait for message handlers to stop. */
    std::mutex m_WaitMessageHandlersMutex;			/**< Mutex to protect m_WaitMessageHandlers. */
    std::shared_ptr<StageFrame> m_StageFrame;		/**< Pointer to the stage frame object */
    bool m_DistanceWActuatorCollisionSetFlag;		/**< Indicates if the distance with actuator collision is already defined. */
    double m_MaxDistanceLimit;									/**< The maximal position for the stages */
    double m_MinDistanceLimit;									/**< The minimal position for the stages */
    double m_MaxForceLimit;											/**< The maximal allowed force. */
    double m_MinForceLimit;											/**< The minimal allowed force. */
    double m_TempMinDistanceLimit;
    double m_TempMaxDistanceLimit;
    double m_TempMinForceLimit;
    double m_TempMaxForceLimit;
    bool m_DistanceLimitExceededFlag;						/**< Indicates if a distance limit exceeded. */
    std::mutex m_DistanceLimitExceededMutex;		/**< Mutex to protect m_DistanceLimitExceededFlag. */
    bool m_ForceLimitExceededFlag;							/**< Indicates if a force limit exceeded. */
    std::mutex m_ForceLimitExceededMutex;				/**< Mutex to protect m_ForceLimitExceededFlag. */
    bool m_DisableIncreaseDistanceFlag;					/**< Indicates if increasing of the distance should be disabled. */
    bool m_DisableDecreaseDistanceFlag;					/**< Indicates if decreasing of the distance should be disabled. */
    std::shared_ptr<ForceSensor> m_ForceSensor;	/**< Pointer to the force sensor */
    std::shared_ptr<ForceSensorMessageHandler> m_ForceSensorMessageHandler; /**< Pointer to the force sensor message handler */
    std::vector<int> m_CurrentPositions;				/**< Vector with the current stage positions */
    long m_CurrentDistance; 										/**< Current distance */
    std::shared_ptr<Protocols> m_CurrentProtocol;								/**< Pointer to the current protocol */
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
    long m_MountingLength;											/**< The mounting length. */
    double m_TempMountingLength;
    long m_GageLength;													/**< Current gage length which will be the mounting length, the user defined distance or the preload distance. */
    double m_TempGageLength;
    long m_MaxPosDistance;											/**< Distance when the motors are on max position (resulting in smallest distance) */
    double m_TempMaxPosDistance;
    double m_Area;															/**< Area of the sample */

    DistanceOrStressOrForce m_DistanceOrStressOrForce;	/**< Indicates if experiment is force or stress based */
    long m_CurrentForce;												/**< Current force */
    int m_CurrentForceUpdateDelay;							/**< Counting variable that the force values is not updated always in the GUI. */
    wxString m_ForceUnit;												/**< Current force unit (N or kPa) */
    std::string m_StoragePath;									/**< Path were the measurement values will be saved as a std::string. */

    wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_SamplingFrequency = 1,
	ID_Ports = 2,
	ID_FileOutput = 3,
  ID_LoadStoredPosition = 4,
  ID_HomeStages = 5,
  ID_Unit = 6,
  ID_MotorDecreaseDistance = 7,
  ID_MotorIncreaseDistance = 8,
  ID_MotorStop = 9,
  ID_LoadPreset = 10,
  ID_ApplyPreset = 11,
  ID_SavePreset = 12,
  ID_SetDistanceWActuatorCollision = 13,
  ID_SetMountingLength = 14,
  ID_LoadLimitSet1 = 15,
  ID_LoadLimitSet2 = 16,
  ID_LoadLimitSet3 = 17,
  ID_LoadLimitSet4 = 18,
  ID_LimitsDistanceValue = 19,
  ID_SetLimits = 20,
  ID_LimitsDistanceGoTo = 21,
  ID_PreloadSpeedPercent = 22,
  ID_PreloadSpeedMm = 23,
  ID_PreloadSendToProtocol = 24,

  ID_OneStepStressForce = 25,
  ID_OneStepDistance = 26,
  ID_OneStepCancel = 27,
  ID_OneStepSendToProtocol = 28,

  ID_ContinuousStressForce = 29,
  ID_ContinuousDistance = 30,
  ID_ContinuousMaxValue = 31,
  ID_ContinuousSteps = 32,
  ID_ContinuousCancel = 33,
  ID_ContinuousSendToProtocol = 34,

  ID_ClearLog = 35,
  ID_SaveLog = 36,
  ID_ClearGraph = 37,
  ID_ExportCSV = 38,
  ID_ExportPNG = 39,
  ID_DeleteExperiment = 40,
  ID_MoveUpExperiment = 41,
  ID_MoveDownExperiment = 42,
  ID_PauseExperiment = 43,
  ID_PauseResumeExperiment = 44,
  ID_LoopProtocol = 45,
  ID_Preview = 46,
  ID_RunProtocol = 47,
  ID_StopProtocol = 48,
  ID_SaveProtocol = 49,
  ID_LoadProtocol = 50,
  ID_MakePhoto = 51
};

#endif // MYFRAME_H
