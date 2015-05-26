/**
 * @file myframe.cpp
 * @brief The main frame.
 * @author Andreas Ziegler
 */

// Includes
#include <mutex>
#include <functional>
#include <iostream>
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/image.h>
#include "../../include/ctb-0.13/serport.h"
#include "mybutton.h"
#include "myframe.h"
#include "myhomestages.h"
#include "mysamplingfrequency_base.h"
#include "myports.h"
#include "myfileoutput.h"
#include "mystartupdialog.h"
#include "../experiments/preload.h"
#include "../experiments/onestepevent.h"
#include "../experiments/continuousevent.h"
#include "../experiments/pause.h"
#include "../experiments/pauseresume.h"
#include "../protocols.h"

// A deleter which doesn't do anything, required for passing shared_ptr.
template<typename T>
void do_nothing_deleter(T *ptr){return;}

//-----------------------------------------------------------------------------
// Regular resources (the non-XRC kind).
//-----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

// Event table connect events to the according methods.
wxBEGIN_EVENT_TABLE(MyFrame, MyFrame_Base)
  EVT_MENU(wxID_EXIT,	MyFrame::OnExit)
  EVT_MENU(XRCID("m_PortsMenuMenuItem"), MyFrame::OnPortsSettings)
  EVT_MENU(XRCID("m_FileOutputMenuItem"), MyFrame::OnFileOutputSettings)
  EVT_MENU(XRCID("m_StartUpDialogMenuItem"), MyFrame::OnOpenStartUpDialog)
  EVT_MENU(XRCID("m_LoadStoredPositionsMenuItem"), MyFrame::OnLoadStoredPositions)
  EVT_MENU(XRCID("m_HomeStagesMenuItem"), MyFrame::OnHomeLinearStages)
  EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookTabChanging)
  EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, MyFrame::OnNotebookTabChanging)
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  //EVT_BUTTON(ID_MotorStop,	MyFrame::OnStopProtocol)
  EVT_FILEPICKER_CHANGED(ID_LoadPreset, MyFrame::OnLoadPreset)
  EVT_BUTTON(ID_ApplyPreset, MyFrame::OnApplyPreset)
  EVT_BUTTON(ID_SavePreset, MyFrame::OnSavePreset)
  EVT_BUTTON(ID_SetMountingLength, MyFrame::OnLengthsSetMountingLength)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_SPINCTRLDOUBLE(ID_CrossSectionArea, MyFrame::OnCrossSectionAreaChange)
  EVT_BUTTON(ID_LoadLimitSet1, MyFrame::OnLimitsLoadSet1)
  EVT_BUTTON(ID_LoadLimitSet2, MyFrame::OnLimitsLoadSet2)
  EVT_BUTTON(ID_LoadLimitSet3, MyFrame::OnLimitsLoadSet3)
  EVT_BUTTON(ID_LoadLimitSet4, MyFrame::OnLimitsLoadSet4)
  EVT_BUTTON(ID_LengthsDistanceGoTo, MyFrame::OnLengthsGoTo)
  EVT_BUTTON(ID_SetLimits, MyFrame::OnLimitsSetLimits)
  EVT_BUTTON(ID_SetSensitivities, MyFrame::OnSetSensitivities)
  EVT_BUTTON(ID_SetZeroDistance, MyFrame::OnLengthsZeroDistance)
  EVT_BUTTON(ID_SetZeroForceStress, MyFrame::OnLengthsZeroForceStress)
  EVT_BUTTON(ID_PreloadCancel, MyFrame::OnPreloadCancel)
  EVT_BUTTON(ID_PreloadSendToProtocol, MyFrame::OnPreloadSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedPercent, MyFrame::OnPreloadSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedMm, MyFrame::OnPreloadSpeedMmChanged)
  EVT_RADIOBUTTON(ID_OneStepStressForce, MyFrame::OnOneStepStressForce)
  EVT_RADIOBUTTON(ID_OneStepDistance, MyFrame::OnOneStepDistance)
  EVT_BUTTON(ID_OneStepCancel, MyFrame::OnOneStepCancel)
  EVT_BUTTON(ID_OneStepSendToProtocol, MyFrame::OnOneStepSendToProtocol)
  EVT_RADIOBUTTON(ID_ContinuousStressForce, MyFrame::OnContinuousStressForce)
  EVT_RADIOBUTTON(ID_ContinuousDistance, MyFrame::OnContinuousDistance)
  EVT_RADIOBUTTON(ID_ContinuousMaxValue, MyFrame::OnContinuousMaxValue)
  EVT_RADIOBUTTON(ID_ContinuousSteps, MyFrame::OnContinuousSteps)
  EVT_BUTTON(ID_ContinuousCancel, MyFrame::OnContinuousCancel)
  EVT_BUTTON(ID_ContinuousSendToProtocol, MyFrame::OnContinuousSendToProtocol)
  EVT_BUTTON(ID_ClearLog, MyFrame::OnClearLog)
  EVT_BUTTON(ID_SaveLog, MyFrame::OnSaveLog)
  EVT_COMBOBOX(ID_GraphChangeType, MyFrame::OnGraphChangeType)
  EVT_CHECKBOX(ID_GraphShowLimits, MyFrame::OnGraphShowLimits)
  EVT_CHECKBOX(ID_GraphAcDeactivate, MyFrame::OnGraphAcDeactivate)
  EVT_BUTTON(ID_ClearGraph, MyFrame::OnClearGraph)
  EVT_BUTTON(ID_ExportCSV, MyFrame::OnExportCSV)
  EVT_BUTTON(ID_ExportPNG, MyFrame::OnExportPNG)
  EVT_BUTTON(ID_DeleteExperiment, MyFrame::OnDeleteExperiment)
  EVT_BUTTON(ID_MoveUpExperiment, MyFrame::OnMoveUpExperiment)
  EVT_BUTTON(ID_MoveDownExperiment, MyFrame::OnMoveDownExperiment)
  EVT_BUTTON(ID_PauseExperiment, MyFrame::OnPauseExperiment)
  EVT_BUTTON(ID_PauseResumeExperiment, MyFrame::OnPauseResumeExperiment)
  EVT_BUTTON(ID_TriggerMessage, MyFrame::OnMakePhoto)
  EVT_BUTTON(ID_Preview, MyFrame::OnPreviewProtocol)
  EVT_BUTTON(ID_RunProtocol, MyFrame::OnRunProtocol)
  EVT_CHECKBOX(ID_LoopProtocol, MyFrame::OnLoopProtocol)
  EVT_SPINCTRL(ID_LoopNumber, MyFrame::OnLoopNumber)
  EVT_BUTTON(ID_StopProtocol, MyFrame::OnStopProtocol)
  EVT_BUTTON(ID_SaveProtocol, MyFrame::OnSaveProtocol)
  EVT_BUTTON(ID_LoadProtocol, MyFrame::OnLoadProtocol)
wxEND_EVENT_TABLE()

// Costum event definitions
wxDEFINE_EVENT(EVT_MYBUTTON_DOWN, wxCommandEvent);
wxDEFINE_EVENT(EVT_MYBUTTON_UP, wxCommandEvent);

/**
 * @brief Constructor of the main frame. Sets the icon.
 * @param title Title of the software.
 * @param settings Pointer to the settings object.
 * @param parent Pointer to the parent object.
 */
MyFrame::MyFrame(const wxString &title, Settings *settings, wxWindow *parent)
  : MyFrame_Base(title, parent),
    m_BlockNotebookTabFlag(false),
    m_Settings(settings),
    m_DistanceWActuatorCollisionSetFlag(false),
    m_CurrentPositions{0,0},
    m_CurrentDistance(150),
    m_CurrentForce(0),
    m_ForceUnit(wxT(" N")),
    m_MountingLength(150),
    m_DistanceOrForceOrStress(DistanceOrForceOrStress::Force),
    m_CurrentProtocol(nullptr),
    m_MaxDistanceLimit(80 / 0.00009921875/*mm per micro step*/),
    m_TempMaxDistanceLimit(0),
    m_MinDistanceLimit(0),
    m_TempMinDistanceLimit(0),
    m_MaxForceLimit(50000.0),
    m_TempMaxForceLimit(0),
    m_MinForceLimit(-50000.0),
    m_TempMinForceLimit(0),
    m_DistanceLimitExceededFlag(false),
    m_ForceLimitExceededFlag(false),
    m_DisableDecreaseDistanceFlag(false),
    m_DisableIncreaseDistanceFlag(false),
    m_TempForceStressSensitivity(0),
    m_ForceStressSensitivity(0.001 * 10000.0),
    m_TempDistanceSensitivity(0),
    m_DistanceSensitivity(0.01 / 0.00009921875/*mm per micro step*//*distance threshold*/),
    m_GageLength(0),
    m_TempGageLength(0),
    m_MaxPosDistance(0),
    m_TempMaxPosDistance(0),
    m_Area(0),
    m_PreloadDoneFlag(true),
    m_CurrentForceUpdateDelay(0),
    m_ShowGraphFlag(false),
    m_ForceStressDistanceGraph(_("Stress/Force / Distance")),
    m_ForceStressDisplacementGraph(_("Stress/Force / Displacement")),
    m_XAxis(nullptr),
    m_Y1Axis(nullptr),
    m_Y2Axis(nullptr),
    m_MessageHandlersFinishedNumber(0),
    m_HighVelocityAbort(false)
{

  // Sets the program icon.
  SetIcon(wxICON(sample));

  // Set the required ID's
  m_InitializePresetFilePicker->SetId(ID_LoadPreset);
  m_InitializeApplyPresetButton->SetId(ID_ApplyPreset);
  m_InitializeSavePresetButton->SetId(ID_SavePreset);
  m_DecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_IncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_InitializeUnitRadioBox->SetId(ID_Unit);
  m_InitializeCrossSectionSpinCtrl->SetId(ID_CrossSectionArea);
  m_LimitsLimitSet1Button->SetId(ID_LoadLimitSet1);
  m_LimitsLimitSet2Button->SetId(ID_LoadLimitSet2);
  m_LimitsLimitSet3Button->SetId(ID_LoadLimitSet3);
  m_LimitsLimitSet4Button->SetId(ID_LoadLimitSet4);
  m_LimitsLimitSetButton->SetId(ID_SetLimits);
  m_LengthsGoToButton->SetId((ID_LengthsDistanceGoTo));
  m_LengthsMountingLengthButton->SetId(ID_SetMountingLength);
  m_LengthsSensitivityButton->SetId(ID_SetSensitivities);
  m_LengthsSetDistanceZeroButton->SetId(ID_SetZeroDistance);
  m_LengthsSetForceZeroButton->SetId(ID_SetZeroForceStress);
  m_PreloadSpeedPreloadSpinCtrl->SetId(ID_PreloadSpeedPercent);
  m_PreloadSpeedMmSpinCtrl->SetId(ID_PreloadSpeedMm);
  m_PreloadCancelButton->SetId(ID_PreloadCancel);
  m_PreloadSendButton->SetId(ID_PreloadSendToProtocol);
  m_OneStepStressForceRadioBtn->SetId(ID_OneStepStressForce);
  m_OneStepDistanceRadioBtn->SetId(ID_OneStepDistance);
  m_OneStepCancelButton->SetId(ID_OneStepCancel);
  m_OneStepSendButton->SetId(ID_OneStepSendToProtocol);
  m_ContinuousStressForceRadioBtn->SetId(ID_ContinuousStressForce);
  m_ContinuousDistanceRadioBtn->SetId(ID_ContinuousDistance);
  m_ContinuousStressForceMaxValueRadioBtn->SetId(ID_ContinuousMaxValue);
  m_ContinuousDistanceMaxValueRadioBtn->SetId(ID_ContinuousMaxValue);
  m_ContinuousStressForceStepsRadioBtn->SetId(ID_ContinuousSteps);
  m_ContinuousDistanceStepsRadioBtn->SetId(ID_ContinuousSteps);
  m_ContinuousCancelButton->SetId(ID_ContinuousCancel);
  m_ContinuousSendButton->SetId(ID_ContinuousSendToProtocol);
  m_StopButton->SetId(ID_MotorStop);
  m_LogClearButton->SetId(ID_ClearLog);
  m_LogSaveButton->SetId(ID_SaveLog);
  m_GraphTypeComboBox->SetId(ID_GraphChangeType);
  m_GraphLimitsCheckBox->SetId(ID_GraphShowLimits);
  m_GraphAcDeacCheckBox->SetId(ID_GraphAcDeactivate);
  m_GraphClearButton->SetId(ID_ClearGraph);
  m_GraphExportCSVButton->SetId(ID_ExportCSV);
  m_GraphExportPNGButton->SetId(ID_ExportPNG);
  m_ProtocolsXButton->SetId(ID_DeleteExperiment);
  m_ProtocolsUpButton->SetId(ID_MoveUpExperiment);
  m_ProtocolsDownButton->SetId(ID_MoveDownExperiment);
  m_ProtocolsPauseButton->SetId(ID_PauseExperiment);
  m_ProtocolsPauseResumeButton->SetId(ID_PauseResumeExperiment);
  m_ProtocolsTriggerButton->SetId(ID_TriggerMessage);
  m_ProtocolsLoopCheckBox->SetId(ID_LoopProtocol);
  m_ProtocolsLoopSpinCtrl->SetId(ID_LoopNumber);
  m_ProtocolsPreviewButton->SetId(ID_Preview);
  m_ProtocolsRunButton->SetId(ID_RunProtocol);
  m_ProtocolsStopButton->SetId(ID_StopProtocol);
  m_ProtocolsSaveButton->SetId(ID_SaveProtocol);
  m_ProtocolsLoadButton->SetId(ID_LoadProtocol);

  wxString str;
  str << m_LengthsGoToSpinCtrl->GetValue();
  m_LengthsGoToSpinCtrl->SetValue(str + " mm");

  // Register the main frame at the two custom buttons
  m_DecreaseDistanceButton->registerMyFrame(this);
  m_IncreaseDistanceButton->registerMyFrame(this);

  // Bind custom buttons events
  Bind(EVT_MYBUTTON_DOWN, &MyFrame::OnMotorDecreaseDistanceStart, this, ID_MotorDecreaseDistance);
  //m_DecreaseDistanceButton->Bind(EVT_MYBUTTON_DOWN, &MyFrame::OnMotorDecreaseDistanceStart, this, ID_MotorDecreaseDistance);
  Bind(EVT_MYBUTTON_UP, &MyFrame::OnMotorDecreaseDistanceStop, this, ID_MotorDecreaseDistance);
  //m_DecreaseDistanceButton->Bind(EVT_MYBUTTON_UP, &MyFrame::OnMotorDecreaseDistanceStop, this, ID_MotorDecreaseDistance);
  Bind(EVT_MYBUTTON_DOWN, &MyFrame::OnMotorIncreaseDistanceStart, this, ID_MotorIncreaseDistance);
  Bind(EVT_MYBUTTON_UP, &MyFrame::OnMotorIncreaseDistanceStop, this, ID_MotorIncreaseDistance);

  // Bind right clicks in the list box
  m_ProtocolsListBox->Connect(m_ProtocolsListBox->GetId(), wxEVT_CONTEXT_MENU, wxMouseEventHandler(MyFrame::OnProtocolsListRightClick), NULL, this);

  // Create graph
  m_Graph = std::unique_ptr<mpWindow>(new mpWindow(m_GraphPanel, wxID_ANY));

  // Define layer for the graph
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_ForceStressDistanceGraph.SetContinuity(true);
    m_ForceStressDisplacementGraph.SetContinuity(true);
    m_StressForcePreviewGraph.SetContinuity(true);
    m_DistancePreviewGraph.SetContinuity(true);
    wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
    wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);
    m_ForceStressDistanceGraph.SetPen(vectorpenStressForce);
    m_ForceStressDisplacementGraph.SetPen(vectorpenStressForce);
    m_StressForcePreviewGraph.SetPen(vectorpenStressForce);
    m_DistancePreviewGraph.SetPen(vectorpenDistance);
    m_ForceStressDistanceGraph.SetDrawOutsideMargins(false);
    m_ForceStressDisplacementGraph.SetDrawOutsideMargins(false);
    m_StressForcePreviewGraph.SetDrawOutsideMargins(false);
    m_DistancePreviewGraph.SetDrawOutsideMargins(false);

    m_MaxStressForceLimitGraph.SetPen(vectorpenStressForce);
    m_MaxStressForceLimitGraph.SetContinuity(true);
    m_MaxStressForceLimitGraph.SetName("Maximal stress/force limit");
    m_MinStressForceLimitGraph.SetPen(vectorpenStressForce);
    m_MinStressForceLimitGraph.SetContinuity(true);
    m_MinStressForceLimitGraph.SetName("Minimal stress/force limit");
    m_MaxDistanceLimitGraph.SetPen(vectorpenDistance);
    m_MaxDistanceLimitGraph.SetContinuity(true);
    m_MaxDistanceLimitGraph.SetName("Maximal distance limit");
    m_MinDistanceLimitGraph.SetPen(vectorpenDistance);
    m_MinDistanceLimitGraph.SetContinuity(true);
    m_MinDistanceLimitGraph.SetName("Minimal distance limit");
  }

  // Add graph to window
  m_Graph->Fit();
  m_GraphSizer1->Insert(0, m_Graph.get(), 0, wxEXPAND);
  m_GraphPanel->Layout();

  // Load file path
  m_StoragePath = m_Settings->getStoragePath();

  // Set m_LogTextCtrl as the log output.
  wxLog::SetActiveTarget(new wxLogTextCtrl(m_LogTextCtrl));
}

/**
 * @brief Changes the limit set buttons names, hides steps in continuous event, sets the digits for the wxSpinCtrlDouble objects and starts up the start up dialog.
 * 				Depending on the user answer, loads start up settings, or open dialog to set the distance at the maximal positions.
 */
void MyFrame::startup(void){
  // Change the limit set button names
  const wxString label1 = "Load " + m_Settings->getSet1Name() + " limits";
  m_LimitsLimitSet1Button->SetLabelText(label1);
  const wxString label2 = "Load " + m_Settings->getSet2Name() + " limits";
  m_LimitsLimitSet2Button->SetLabelText(label2);
  const wxString label3 = "Load " + m_Settings->getSet3Name() + " limits";
  m_LimitsLimitSet3Button->SetLabelText(label3);
  const wxString label4 = "Load " + m_Settings->getSet4Name() + " limits";
  m_LimitsLimitSet4Button->SetLabelText(label4);

  // Set selection of the graph type wxComboBox
  m_GraphTypeComboBox->SetSelection(0);

  // Hide steps in ContinuousEvent
  m_ContinuousStressForceStepsStaticText->Show(false);
  m_ContinuousStressForceStepsSpinCtrl->Show(false);
  m_ContinuousDistanceStepsStaticText->Show(false);
  m_ContinuousDistanceStepsSpinCtrl->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_LengthsGoToSpinCtrl->SetDigits(2);
  m_InitializeCrossSectionSpinCtrl->SetDigits(3);
  m_LimitsLimitMaxDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMaxForceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinForceSpinCtrl->SetDigits(2);
  m_LengthsForceStressSensitivitySpinCtrl->SetDigits(4);
  m_LengthsForceStressSensitivitySpinCtrl->SetValue(0.001);
  m_LengthsDistanceSensitivitySpinCtrl->SetDigits(4);
  m_LengthsDistanceSensitivitySpinCtrl->SetValue(0.001);
  m_PreloadLimitSpinCtrl->SetDigits(3);
  m_PreloadSpeedPreloadSpinCtrl->SetDigits(3);
  m_PreloadSpeedMmSpinCtrl->SetDigits(3);
  m_PreloadSpeedMmSpinCtrl->SetRange(0.01, 11);
  m_OneStepStressForceVelocitySpinCtrl->SetDigits(3);
  m_OneStepStressForceVelocitySpinCtrl->SetRange(0.01, 1000);
  m_OneStepStressForceDelaySpinCtrl->SetDigits(2);
  m_OneStepStressForceLimitSpinCtrl->SetDigits(2);
  m_OneStepStressForceDwellSpinCtrl->SetDigits(2);
  m_OneStepDistanceVelocitySpinCtrl->SetDigits(3);
  m_OneStepDistanceVelocitySpinCtrl->SetRange(0.01, 1000);
  m_OneStepDistanceDelaySpinCtrl->SetDigits(2);
  m_OneStepDistanceLimitSpinCtrl->SetDigits(2);
  m_OneStepDistanceDwellSpinCtrl->SetDigits(2);
  m_OneStepEndOfEventHoldSpinCtrl->SetDigits(2);
  m_ContinuousStressForceVelocitySpinCtrl->SetDigits(3);
  m_ContinuousStressForceVelocitySpinCtrl->SetRange(0.01, 1000);
  m_ContinuousStressForceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousStressForceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousStressForceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousDistanceVelocitySpinCtrl->SetDigits(3);
  m_ContinuousDistanceVelocitySpinCtrl->SetRange(0.01, 1000);
  m_ContinuousDistanceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousDistanceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousDistanceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousEndOfEventHoldDistanceSpinCtrl->SetDigits(2);
  m_ContinuousEndOfEventHoldStressForceSpinCtrl->SetDigits(2);

  std::unique_ptr<wxMessageDialog> dialog = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this,
                                                                                                 "Does the mechanical set up changed since the last use?",
                                                                                                 wxMessageBoxCaptionStr,
                                                                                                 wxYES_NO|wxNO_DEFAULT));
  // Ask if the mechanical set up changed.
  int answer = dialog->ShowModal();
  // If the set up didn't change, load Le, L0, current distance, limits and load stored positions.
  if(wxID_NO == answer){
    loadStoredPositions();
    // Load Le
    m_MaxPosDistance = m_Settings->getMaxPosDistance();
    // set max pos distance in stage frame.
    m_StageFrame->setMaxPosDistance(m_MaxPosDistance);
    m_InitializeMaxPosShowStaticText->SetLabelText(std::to_string(m_MaxPosDistance * 0.00009921875/*mm per micro step*/));
    // Load mounting length.
    m_MountingLength = m_Settings->getMountingLength();
    m_InitializeMountingLengthShowStaticText->SetLabelText(std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/));
    m_LengthsGoToSpinCtrl->SetValue(m_MountingLength * 0.00009921875/*mm per micro step*/);
    // Load L0.
    m_GageLength = m_Settings->getGageLength();

    // Load limits
    m_MinDistanceLimit = m_Settings->getMinDistanceLimit();
    m_MaxDistanceLimit = m_Settings->getMaxDistanceLimit();
    m_MinForceLimit = m_Settings->getMinForceLimit();
    m_MaxForceLimit = m_Settings->getMaxForceLimit();
    // Set limits to the stages
    m_StageFrame->setMinDistanceLimitMS(m_MinDistanceLimit);
    m_StageFrame->setMaxDistanceLimitMS(m_MaxDistanceLimit);
    // Display limits
    m_InitializeMinDistanceShowStaticText->SetLabelText(to_string_wp(m_MinDistanceLimit * 0.00009921875/*mm per micro step*/, 2));
    m_InitializeMaxDistanceShowStaticText->SetLabelText(to_string_wp(m_MaxDistanceLimit * 0.00009921875/*mm per micro step*/, 2));
    m_InitializeMinForceShowStaticText->SetLabelText(to_string_wp(m_MinForceLimit / 10000.0, 2));
    m_InitializeMaxForceShowStaticText->SetLabelText(to_string_wp(m_MaxForceLimit / 10000.0, 2));

    // Load sensitivities
    m_ForceStressSensitivity = m_Settings->getForceStressSensitivity();
    m_DistanceSensitivity = m_Settings->getDistanceSensitivity();
    // Display sensitivities
    m_InitializeForceStressSensitivityShowStaticText->SetLabelText(to_string_wp(m_ForceStressSensitivity / 10000.0, 2));
    m_InitializeDistanceSensitivityShowStaticText->SetLabelText(to_string_wp(m_DistanceSensitivity * 0.00009921875/*mm per micro step*/, 2));

  }else if(wxID_YES == answer){ // If the set up changed, show start up dialog.
    // Reset limits of the stages
    m_MinDistanceLimit = 0;
    m_MaxDistanceLimit = 80 / 0.00009921875/*mm per micro step*/;
    m_StageFrame->setMinDistanceLimitMS(m_MinDistanceLimit);
    m_StageFrame->setMaxDistanceLimitMS(m_MaxDistanceLimit);

    std::unique_ptr<MyStartUpDialog> dialog = std::unique_ptr<MyStartUpDialog>(new MyStartUpDialog(this));
    dialog->ShowModal();
  }
}

/**
 * @brief Returns the message handler wait condition variable as shared_ptr.
 * @return The message handler wait condition variable as shared_ptr.
 */
std::shared_ptr<std::condition_variable> MyFrame::getMessageHandlersWait(void){
  std::shared_ptr<std::condition_variable> messagehandlerswait(&m_WaitMessageHandlers, do_nothing_deleter<std::condition_variable>);
  return(messagehandlerswait);
}

/**
 * @brief Returns the message handler wait mutex as shared_ptr.
 * @return The message handler wait mutex as shared_ptr.
 */
std::shared_ptr<std::mutex> MyFrame::getMessageHandlersWaitMutex(void){
  std::shared_ptr<std::mutex> messagehandlerswaitmutex(&m_WaitMessageHandlersMutex, do_nothing_deleter<std::mutex>);
  return(messagehandlerswaitmutex);
}

/**
 * @brief Returns the number of finished message handler variable as shared_ptr.
 * @return The number of finished message handler variable as shared_ptr.
 */
std::shared_ptr<int> MyFrame::getMessageHandlersFinishedNumber(void){
  std::shared_ptr<int> messagehandlerfinishednr(&m_MessageHandlersFinishedNumber, do_nothing_deleter<int>);
  return(messagehandlerfinishednr);
}

/**
 * @brief Destructor. Unregister the update method from the message handlers, stops the receiver threads, removes vectors and axis from the graph. Waits until the message
 *        handlers are finished and saves the start up settings in the configuration file.
 */

MyFrame::~MyFrame(void){
  // Unregister update methods
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  // Stop linear stage receiver threads
  ((m_LinearStages.at(0))->getMessageHandler())->setExitFlag(false);
  ((m_LinearStages.at(1))->getMessageHandler())->setExitFlag(false);

  // Stop force sensor receiver thread
  (m_ForceSensor->getMessageHandler())->setExitFlag(false);

  // Remove vector, and the axis from graph.
  m_Graph->DelLayer(&m_ForceStressDistanceGraph);
  m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
  m_Graph->DelLayer(&m_StressForcePreviewGraph);
  m_Graph->DelLayer(&m_DistancePreviewGraph);
  m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
  m_Graph->DelLayer(&m_MinStressForceLimitGraph);
  m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
  m_Graph->DelLayer(&m_MinDistanceLimitGraph);
  m_Graph->DelLayer(m_XAxis.get());
  m_Graph->DelLayer(m_Y1Axis.get());
  m_Graph->DelLayer(m_Y2Axis.get());

  {
    // Wait until the message handlers are finished.
    std::unique_lock<std::mutex> lck(m_WaitMessageHandlersMutex);
    m_WaitMessageHandlers.wait(lck, [&]{return(m_MessageHandlersFinishedNumber>=3);});
    std::cout << "MyFrame all 3 handlers should be finished." << std::endl;
  }

  // Save start up settings.
  m_Settings->setMaxPosDistance(m_MaxPosDistance);
  m_Settings->setMountingLength(m_MountingLength);
  m_Settings->setGageLength(m_GageLength);
  m_Settings->setMinDistanceLimit(m_MinDistanceLimit);
  m_Settings->setMaxDistanceLimit(m_MaxDistanceLimit);
  m_Settings->setMinForceLimit(m_MinForceLimit);
  m_Settings->setMaxForceLimit(m_MaxForceLimit);

  // Delete linear stage objects.
  /*
  // Not needed because destructor of main.cpp deletes them.
  for(auto x : *m_LinearStages){
    delete x;
  }
  delete m_ForceSensor;
  */
}

/**
 * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
 * 				thread. (CallAfter() asynchronously call the updateDistance method). Updates the distance and the force and also performs the limit checks.
 * @param value The position of a stage or a force.
 * @param type	Defines the type of the value (position of stage 1, 2 or force)
 */
void MyFrame::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;

      {
        std::lock_guard<std::mutex> lck{m_DistanceLimitExceededMutex};
        // Check if no limits exceeded yet.
        if(false == m_DistanceLimitExceededFlag){
          // Stop stages and protocol if a distance limit exceeded and indicate, that a distance limit exceeded.
          if(((m_MaxDistanceLimit - 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) <= m_CurrentDistance) ||
             ((m_MinDistanceLimit + 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) >= m_CurrentDistance)){
            m_DistanceLimitExceededFlag = true;
            m_StageFrame->stop();
            if(nullptr != m_CurrentProtocol){
              m_CurrentProtocol->stopProtocol();
            }
            // Disable increasing/decreasing of the distance according to the limit which exceeded.
            if((m_MaxDistanceLimit - 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) <= m_CurrentDistance){
              m_DisableIncreaseDistanceFlag = true;
            }else if((m_MinDistanceLimit + 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) >= m_CurrentDistance){
              m_DisableDecreaseDistanceFlag = true;
            }
            wxLogWarning(std::string("Distance limit exceeded, current distance: " + std::to_string(m_CurrentDistance * 0.00009921875/*mm per micro step*/) + "mm").c_str());
          }
        }else{ // Check if limits exceeded yet.

          // Enable increasing/decreasing of the distance according to the limit which exceeded.
          if((m_MaxDistanceLimit - 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) >= m_CurrentDistance){
            m_DisableIncreaseDistanceFlag = false;
          }if((m_MinDistanceLimit + 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) <= m_CurrentDistance){
            m_DisableDecreaseDistanceFlag = false;
          }
          // Indicate that distance is within the limits if it is.
          if(((m_MaxDistanceLimit - 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) >= m_CurrentDistance) &&
             ((m_MinDistanceLimit + 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) <= m_CurrentDistance)){
            m_DistanceLimitExceededFlag = false;
            //wxLogMessage("MyFrame: Distance limit exceeded flag disabled.");
            wxLogMessage("Distance within the limits.");
          }
        }
      }
      // Update distance in the GUI.
      CallAfter(&MyFrame::updateDistance);

      break;
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;

      {
        std::lock_guard<std::mutex> lck{m_ForceLimitExceededMutex};
        if(false == m_ForceLimitExceededFlag){ // Check if no limits exceeded yet.
          // Stop stages and protocol if a force limit exceeded and indicate, that a force limit exceeded.
          if((m_MaxForceLimit < m_CurrentForce) || (m_MinForceLimit > m_CurrentForce)){
            m_ForceLimitExceededFlag = true;
            m_StageFrame->stop();
            if(nullptr != m_CurrentProtocol){
              m_CurrentProtocol->stopProtocol();
            }
            // Disable increasing/decreasing of the distance according to the limit which exceeded.
            if(m_MaxForceLimit < m_CurrentForce){
              m_DisableIncreaseDistanceFlag = true;
            }else if(m_MinForceLimit > m_CurrentForce){
              m_DisableDecreaseDistanceFlag = true;
            }
            wxLogWarning(std::string("Force limit exceeded, current force: " + std::to_string(m_CurrentForce/10000.0) + " N").c_str());
            /*                         " m_MinForceLimit: " + std::to_string(m_MinForceLimit) +
                                     " m_MaxForceLimit: " + std::to_string(m_MaxForceLimit)).c_str());
            */
          }
        }else{ // Check if limits exceeded yet.

          // Enable increasing/decreasing of the distance according to the limit which exceeded.
          if(m_MaxForceLimit > m_CurrentForce){
            m_DisableIncreaseDistanceFlag = false;
          }if(m_MinForceLimit < m_CurrentForce){
            m_DisableDecreaseDistanceFlag = false;
          }
          // Indicate that distance is within the limits if it is.
          if((m_MaxForceLimit > m_CurrentForce) && (m_MinForceLimit < m_CurrentForce)){
            m_ForceLimitExceededFlag = false;
            //wxLogMessage("MyFrame: Force limit exceeded flag disabled.");
            wxLogMessage("Force within the limits.");
          }
        }
      }

      // Update force in the GUI every 20th update.
      m_CurrentForceUpdateDelay++;
      if(20 <= m_CurrentForceUpdateDelay){
        m_CurrentForceUpdateDelay = 0;
        CallAfter(&MyFrame::updateForce);
      }
      break;
  }
}

/**
 * @brief Updates the storage path from the GUI.
 * @param path Path as a std::string
 */
void MyFrame::updateStoragePath(std::string path){
  m_StoragePath = path;
}

/**
 * @brief Method wich will be executed, when the software will be closed by the user.
 * @param event Occuring event
 */
void MyFrame::OnExit(wxCommandEvent& event){
  Close(true);
}

/**
 * @brief Method wich will be executed, when the user goes to the port settings.
 * @param event Occuring event
 */
void MyFrame::OnPortsSettings(wxCommandEvent& event){
  std::unique_ptr<MyPorts> ports = std::unique_ptr<MyPorts>(new MyPorts(m_Settings, m_LinearStages, m_ForceSensor, this));

  ports->ShowModal();
}

/**
 * @brief Method wich will be executed, when the user goes to the file output settings.
 * @param event Occuring event
 */
void MyFrame::OnFileOutputSettings(wxCommandEvent& event){
  std::unique_ptr<MyFileOutput> fileOutput = std::unique_ptr<MyFileOutput>(new MyFileOutput(this, m_Settings, m_StoragePath, this));
  fileOutput->ShowModal();
}

/**
 * @brief Method wich will be executed, when the user changes a tab in the wxNotebook (Needed to disable tab changes during experiment parameters changes).
 * @param event Occuring event
 */
void MyFrame::OnNotebookTabChanging(wxBookCtrlEvent& event){
  // Skip the event if the tab is not blocked.
  if(false == m_BlockNotebookTabFlag){
    event.Skip(true);
  }else{ // Otherwise veto the event.
    event.Veto();
  }
}

/**
 * @brief Method wich will be executed, when the user opens the start up dialog.
 * @param event Occuring event
 */
void MyFrame::OnOpenStartUpDialog(wxCommandEvent& event){
  // Reset limits of the stages
  m_MinDistanceLimit = 0;
  m_MaxDistanceLimit = 80 / 0.00009921875/*mm per micro step*/;
  m_StageFrame->setMinDistanceLimitMS(m_MinDistanceLimit);
  m_StageFrame->setMaxDistanceLimitMS(m_MaxDistanceLimit);

  std::unique_ptr<MyStartUpDialog> dialog = std::unique_ptr<MyStartUpDialog>(new MyStartUpDialog(this));
  dialog->ShowModal();
}

/**
 * @brief Method wich will be executed, when the user chooses stress as unit.
 * @param event Occuring event
 */
void MyFrame::OnUnit(wxCommandEvent& event){
  // Changes to stress.
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_InitializeMinForceStaticText->SetLabelText("Min. stress [MPa]:");
    m_InitializeMaxForceStaticText->SetLabelText("Max. stress [MPa]:");
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal stress [MPa]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal stress [MPa]:");
    m_LengthsForceStressSensitivityStaticText->SetLabelText("Stress [MPa]");
    m_LengthsSetForceZeroButton->SetLabelText("Zero stress");
    m_PreloadLimitStaticText->SetLabelText("Stress Limit [MPa]");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [MPa]:");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [MPa]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText("Incrementd [dMPa]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("MPa");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Smax.");
    m_ContinuousEndOfEventHoldStressForceRadioBtn->SetLabelText("Stop at [MPa]:");
    m_ForceUnit = wxT(" MPa");

    // If the graph is active.
    if(true == m_ShowGraphFlag){
      wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
      m_Graph->DelLayer(m_Y1Axis.get());
      m_Y1Axis.reset(new mpScaleY(wxT("Stress [MPa]"), mpALIGN_LEFT, true));
      m_Y1Axis->SetPen(vectorpenStressForce);
      wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
      m_Y1Axis->SetFont(graphFont);
      m_Graph->AddLayer(m_Y1Axis.get());
      m_Graph->Fit();
    }

    m_DistanceOrForceOrStress = DistanceOrForceOrStress::Stress;
  }else{ // Changes to force.
    m_InitializeMinForceStaticText->SetLabelText("Min. force [N]:");
    m_InitializeMaxForceStaticText->SetLabelText("Max. force [N]:");
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal force [N]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal force [N]:");
    m_LengthsForceStressSensitivityStaticText->SetLabelText("Force [N]");
    m_LengthsSetForceZeroButton->SetLabelText("Zero force");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [N]:");
    m_PreloadLimitStaticText->SetLabelText("Force Limit [N]");
    m_OneStepStressForceLimitStaticText->SetLabelText(" Limit [N]:");
    m_OneStepStressForceLimitStaticText->SetLabelText(" Limit [N]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText(" Incrementd [dN]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("N");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Fmax.");
    m_ContinuousEndOfEventHoldStressForceRadioBtn->SetLabelText("Stop at [N]:");
    m_ForceUnit = wxT(" N");

    // If the graph is active.
    if(true == m_ShowGraphFlag){
      wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
      m_Graph->DelLayer(m_Y1Axis.get());
      m_Y1Axis.reset(new mpScaleY(wxT("Force [N]"), mpALIGN_LEFT, true));
      m_Y1Axis->SetPen(vectorpenStressForce);
      wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
      m_Y1Axis->SetFont(graphFont);
      m_Graph->AddLayer(m_Y1Axis.get());
      m_Graph->Fit();
    }

    m_DistanceOrForceOrStress = DistanceOrForceOrStress::Force;
  }
}

/**
 * @brief Method wich will be executed, when the user changes the cross section area. Updates the cross section area in the protocol.
 * @param event Occuring event
 */
void MyFrame::OnCrossSectionAreaChange(wxSpinDoubleEvent& event){
  m_Area = m_InitializeCrossSectionSpinCtrl->GetValue();
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->setCrossSectionArea(m_Area);
  }
}

/**
 * @brief Method wich will be executed when the user clicks on load stored positions.
 * @param event Occuring event
 */
void MyFrame::OnLoadStoredPositions(wxCommandEvent& event){
  loadStoredPositions();
}

/**
 * @brief Method wich will be executed at start up when the set up didn't change. Loads stored positions from the linear stages.
 */
void MyFrame::loadStoredPositions(void){
  (m_LinearStages.at(0))->loadStoredPosition();
  (m_LinearStages.at(1))->loadStoredPosition();
}

/**
 * @brief Method wich will be executed, when the user klicks on the home stage button.
 * @param event Occuring event
 */
void MyFrame::OnHomeLinearStages(wxCommandEvent& event){

  MyHomeStages *homestages = new MyHomeStages(m_StageFrame, this, this);
  homestages->Show();
}

/**
 * @brief Method wich will be executed, when the user clicks on the set Le button. It sets the current distance to the limit to force the user
 *        to set the limits. Sets the distance at the maximum positions and displays it in the GUI.
 */
void MyFrame::setDistanceWActuatorCollision(double le){
  //m_MaxPosDistance = m_LengthsLESpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
  m_DistanceWActuatorCollisionSetFlag = true;
  // Set min. and max. distance and the distance at maximum positions.
  //m_StageFrame->setMinDistanceLimitMM(m_CurrentDistance * 0.00009921875/*mm per micro step*/);
  //m_StageFrame->setMaxDistanceLimitMM(m_CurrentDistance * 0.00009921875/*mm per micro step*/);
  m_StageFrame->setMinDistanceLimitMS(m_CurrentDistance);
  m_StageFrame->setMaxDistanceLimitMS(m_CurrentDistance);
  m_MaxPosDistance = m_StageFrame->setDistanceWActuatorCollision(le / 0.00009921875/*mm per micro step*/);
  m_LengthsGoToSpinCtrl->SetValue(m_MaxPosDistance);
}

/**
 * @brief Method wich will be executed, when the user clicks on the set length button. Updates the mounting- and gagelength.
 * @param event Occuring event
 */
void MyFrame::OnLengthsSetMountingLength(wxCommandEvent& event){
  m_MountingLength = m_CurrentDistance;
  m_GageLength = m_CurrentDistance;
  m_InitializeMountingLengthShowStaticText->SetLabelText(std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/));
}

/**
 * @brief Method wich will be executed, when the user clicks on the set sensitivities.
 * @param event Occuring event
 */
void MyFrame::OnSetSensitivities(wxCommandEvent& event){
  if(DistanceOrForceOrStress::Force == m_DistanceOrForceOrStress){
    m_ForceStressSensitivity = m_LengthsForceStressSensitivitySpinCtrl->GetValue() * 10000.0;
  }else if(DistanceOrForceOrStress::Stress == m_DistanceOrForceOrStress){
    m_ForceStressSensitivity = (m_LengthsForceStressSensitivitySpinCtrl->GetValue() * m_Area / 1000) * 10000.0;
  }

  m_DistanceSensitivity = m_LengthsDistanceSensitivitySpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;

  m_Settings->setForceStressSensitivity(m_ForceStressSensitivity);
  m_Settings->setDistanceSensitivity(m_DistanceSensitivity);

  m_InitializeForceStressSensitivityShowStaticText->SetLabelText(to_string_wp(m_LengthsForceStressSensitivitySpinCtrl->GetValue(), 4));
  m_InitializeDistanceSensitivityShowStaticText->SetLabelText(to_string_wp(m_LengthsDistanceSensitivitySpinCtrl->GetValue(), 4));

  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->setSensitivities(m_ForceStressSensitivity, m_DistanceSensitivity);
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the zero distance button.
 * @param event Occuring event
 */
void MyFrame::OnLengthsZeroDistance(wxCommandEvent& event){
  m_StageFrame->setZeroDistanceOffset();
}

/**
 * @brief Method wich will be executed, when the user clicks on the zero force/stress button.
 * @param event Occuring event
 */
void MyFrame::OnLengthsZeroForceStress(wxCommandEvent& event){
  m_ForceSensor->setZeroForce();
}

/**
 * @brief Loads limit set 1 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnLimitsLoadSet1(wxCommandEvent& event){
  m_LimitsLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet1MaxDistance());
  m_LimitsLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet1MinDistance());
  m_LimitsLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet1MaxForce());
  m_LimitsLimitMinForceSpinCtrl->SetValue(m_Settings->getSet1MinForce());
}

/**
 * @brief Loads limit set 2 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnLimitsLoadSet2(wxCommandEvent& event){
  m_LimitsLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet2MaxDistance());
  m_LimitsLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet2MinDistance());
  m_LimitsLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet2MaxForce());
  m_LimitsLimitMinForceSpinCtrl->SetValue(m_Settings->getSet2MinForce());
}

/**
 * @brief Loads limit set 3 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnLimitsLoadSet3(wxCommandEvent& event){
  m_LimitsLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet3MaxDistance());
  m_LimitsLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet3MinDistance());
  m_LimitsLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet3MaxForce());
  m_LimitsLimitMinForceSpinCtrl->SetValue(m_Settings->getSet3MinForce());
}

/**
 * @brief Loads limit set 4 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnLimitsLoadSet4(wxCommandEvent& event){
  m_LimitsLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet4MaxDistance());
  m_LimitsLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet4MinDistance());
  m_LimitsLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet4MaxForce());
  m_LimitsLimitMinForceSpinCtrl->SetValue(m_Settings->getSet4MinForce());
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Go to" button in lengths.
 * @param event Occuring event
 */
void MyFrame::OnLengthsGoTo(wxCommandEvent& event){
  m_StageFrame->gotoMMDistance(m_LengthsGoToSpinCtrl->GetValue());
}

/**
 * @brief Method wich will be executed, when the user sets the limits. Updates the limits and forwards them to the protocol.
 * @param event Occuring event
 */
void MyFrame::OnLimitsSetLimits(wxCommandEvent& event){
  m_MaxDistanceLimit = m_LimitsLimitMaxDistanceSpinCtrl->GetValue();
  m_MinDistanceLimit = m_LimitsLimitMinDistanceSpinCtrl->GetValue();

  m_InitializeMinDistanceShowStaticText->SetLabelText(to_string_wp(m_MinDistanceLimit, 2));
  m_InitializeMaxDistanceShowStaticText->SetLabelText(to_string_wp(m_MaxDistanceLimit, 2));

  if(1 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = m_LimitsLimitMaxForceSpinCtrl->GetValue() * 10000.0;
    m_MinForceLimit = m_LimitsLimitMinForceSpinCtrl->GetValue() * 10000.0;
    m_InitializeMinForceShowStaticText->SetLabelText(to_string_wp(m_LimitsLimitMinForceSpinCtrl->GetValue(), 2));
    m_InitializeMaxForceShowStaticText->SetLabelText(to_string_wp(m_LimitsLimitMaxForceSpinCtrl->GetValue(), 2));
  } else if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = (m_LimitsLimitMaxForceSpinCtrl->GetValue() * m_Area / 1000) * 10000.0;
    m_MinForceLimit = (m_LimitsLimitMinForceSpinCtrl->GetValue() * m_Area / 1000) * 10000.0;
    m_InitializeMinForceShowStaticText->SetLabelText(to_string_wp(m_LimitsLimitMinForceSpinCtrl->GetValue(), 2));
    m_InitializeMaxForceShowStaticText->SetLabelText(to_string_wp(m_LimitsLimitMaxForceSpinCtrl->GetValue(), 2));
  }

  m_StageFrame->setMaxDistanceLimitMM(m_MaxDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_StageFrame->setMinDistanceLimitMM(m_MinDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_MaxDistanceLimit /= 0.00009921875/*mm per micro step*/;
  m_MinDistanceLimit /= 0.00009921875/*mm per micro step*/;

  checkProtocol();
  // Update limits in the protocol.
  m_CurrentProtocol->setLimits(m_MinDistanceLimit, m_MaxDistanceLimit, m_MinForceLimit, m_MaxForceLimit);
}

/**
 * @brief Method wich will be executed, when the decrease stage distance button is pushed down. Starts moving forward.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistanceStart(wxCommandEvent& event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  if(false == m_DisableDecreaseDistanceFlag){
    m_StageFrame->moveForward(1/*mm/s*/);
    // Create IncreaseDecreaseVelocityTimer class object
    m_IncreaseDecreaseVelocityTimer = std::make_shared<IncreaseDecreaseVelocityTimer>(IncreaseDecreaseVelocityTimer(m_StageFrame, 0.1/*mm/s*/));
    // Start increaseTimer in a seperate thread. It will increase the speed periodically.
    m_IncreaseDecreaseVelocityTimerThread = std::thread(&IncreaseDecreaseVelocityTimer::decreaseDistanceTimer, m_IncreaseDecreaseVelocityTimer);
    m_IncreaseDecreaseVelocityTimerThread.detach();
  }else{
    wxLogMessage("MyFrame: Decrease distance disabled");
  }
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the decrease stage distance button is released. Stops the stage frame.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistanceStop(wxCommandEvent& event){
  // Only proceed if the increase velocity thread exists.
  if(m_IncreaseDecreaseVelocityTimer){
    // Only stop the stage if it is not already done by the limit check.
    if(false == m_DisableDecreaseDistanceFlag){
      m_StageFrame->stop();
    }
    // Stop increaseTimer method, which runs in an own thread.
    m_IncreaseDecreaseVelocityTimer->setExitFlag();
    // Delete the IncreaseDecreaseVelocityTimer object for later use.
    m_IncreaseDecreaseVelocityTimer.reset();
  }
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the increase stage distance button is pushed down. Starts moving backwards.
 * @param event Occuring event
 */
void MyFrame::OnMotorIncreaseDistanceStart(wxCommandEvent &event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  if(false == m_DisableIncreaseDistanceFlag){
    m_StageFrame->moveBackward(1/*mm/s*/);
    // Create IncreaseDecreaseVelocityTimer class object
    m_IncreaseDecreaseVelocityTimer = std::make_shared<IncreaseDecreaseVelocityTimer>(IncreaseDecreaseVelocityTimer(m_StageFrame, 0.1/*mm/s*/));
    // Start increaseTimer in a seperate thread. It will increase the speed periodically.
    m_IncreaseDecreaseVelocityTimerThread = std::thread(&IncreaseDecreaseVelocityTimer::increaseDistanceTimer, m_IncreaseDecreaseVelocityTimer);
    m_IncreaseDecreaseVelocityTimerThread.detach();
  }else{
    wxLogMessage("MyFrame: Increase distance disabled");
  }
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the increase stage distance button is released. Stops the stage frame.
 * @param event Occuring event
 */
void MyFrame::OnMotorIncreaseDistanceStop(wxCommandEvent& event){
  // Only proceed if the increase velocity thread exists.
  if(m_IncreaseDecreaseVelocityTimer){
    // Only stop the stage if it is not already done by the limit check.
    if(false == m_DisableIncreaseDistanceFlag){
      m_StageFrame->stop();
    }
    // Stop increaseTimer method, which runs in an own thread.
    m_IncreaseDecreaseVelocityTimer->setExitFlag();
    // Delete the IncreaseDecreaseVelocityTimer object for later use.
    m_IncreaseDecreaseVelocityTimer.reset();
  }
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the user klicks on the motor stop button. Stops the stage frame and notify the stop.
 * @param event Occuring event
 */
void MyFrame::OnMotorStop(wxCommandEvent& event){
  if(nullptr != m_StageFrame){
    m_StageFrame->stop();
  }
  std::lock_guard<std::mutex> lck(m_WaitMutex);
  m_Wait.notify_all();

  /*
  if(NULL != m_CurrentExperiment){
    m_CurrentExperiment = NULL;
  }
  */
}

/**
 * @brief Method wich will be executed, when the user clicks on the clear log button.
 * @param event Occuring event
 */
void MyFrame::OnClearLog(wxCommandEvent& event){
  m_LogTextCtrl->Clear();
}

/**
 * @brief Method wich will be executed, when the user clicks on the save log button. Opens a file dialog and then saves the log.
 * @param event Occuring event
 */
void MyFrame::OnSaveLog(wxCommandEvent& event){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));

  std::string pathAndFilename = m_StoragePath + "/" + "Log_" + std::string(mbstr) + ".txt";

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save png file"), "", "", "LOG files (*.txt)|*.txt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  saveFileDialog.SetPath(pathAndFilename);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }

  m_LogTextCtrl->SaveFile(saveFileDialog.GetPath());

  wxLogMessage(std::string("Log saved in: " + saveFileDialog.GetPath()).c_str());
}

/**
 * @brief Opens a velocity warning pop-up dialog.
 */
bool MyFrame::showHighVelocityWarningFromExperiments(void){
  CallAfter(&MyFrame::showHighVelocityWarning);

  // Wait until dialog is closed.
  std::unique_lock<std::mutex> lck(m_WaitHighVelocityMutex);
  m_WaitHighVelocity.wait(lck);
  return(m_HighVelocityAbort);
}

/**
 * @brief Shows velocity warining dialog.
 */
void MyFrame::showHighVelocityWarning(void){
  std::unique_ptr<wxMessageDialog> dialog = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this, "Velocity is a bit high. Set to maximal velocity (11 mm/s)?", wxMessageBoxCaptionStr, wxOK|wxCANCEL));
  if(wxID_CANCEL == dialog->ShowModal()){
    m_HighVelocityAbort = true;
  }else{
    m_HighVelocityAbort = false;
  }
  std::lock_guard<std::mutex> lck(m_WaitHighVelocityMutex);
  m_WaitHighVelocity.notify_all();
}

/**
 * @brief Calculates the distance and print the value in the GUI.
 */
void MyFrame::updateDistance(void){
  wxString tmp;
  tmp << m_CurrentDistance * 0.00009921875/*mm per micro step*/ << wxT(" mm");
  m_DistanceStaticText->SetLabel(tmp);
  //std::cout << "Distance updated." << std::endl;
}

/**
 * @brief Calculates the distance and print the value in the GUI.
 */
void MyFrame::updateForce(void){
  wxString tmp;
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    tmp << (static_cast<double>((m_CurrentForce / 10000.0) / m_Area)) << m_ForceUnit;
  } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
    tmp << (static_cast<double>(m_CurrentForce) / 10000.0) << m_ForceUnit;
  }
  m_ForceStaticText->SetLabel(tmp);
}
