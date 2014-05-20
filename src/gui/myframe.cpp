// Includes
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/image.h>
#include "mybutton.h"
#include <mutex>
#include <functional>
#include "../../include/ctb-0.13/serport.h"
#include "myframe.h"
#include "myhomestages.h"
#include "mysamplingfrequency_base.h"
#include "myports.h"
#include "myfileoutput.h"
#include "protocols.h"
#include "../experiments/preload.h"
#include "../experiments/onestepevent.h"
#include "../experiments/conditioning.h"
#include "../experiments/ramp2failure.h"
#include "../experiments/relaxation.h"
#include "../experiments/creep.h"
#include "../experiments/fatiguetesting.h"

#include <iostream>

// An deleter which doesn't do anything, required for passing shared_ptr.
void do_nothing_deleter_conditional_variable(std::condition_variable *){return;}
void do_nothing_deleter_mutex(std::mutex *){return;}
void do_nothing_deleter_int(int *){return;}

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
  EVT_MENU(XRCID("m_SamplingFrequencyMenuItem"), MyFrame::OnSamplingFrequencySettings)
  EVT_MENU(XRCID("m_PortsMenuMenuItem"), MyFrame::OnPortsSettings)
  EVT_MENU(XRCID("m_FileOutputMenuItem"), MyFrame::OnFileOutputSettings)
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  EVT_BUTTON(ID_LoadStoredPosition, MyFrame::OnInitializeLoadStoredPosition)
  EVT_BUTTON(ID_HomeStages, MyFrame::OnInitializeHomeLinearStages)
  EVT_BUTTON(ID_SetMountingLength, MyFrame::OnInitializeSetMountingLength)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_RADIOBOX(ID_GoTo, MyFrame::OnGoTo)
  //EVT_BUTTON(ID_MotorDecreaseDistance,	MyFrame::OnMotorDecreaseDistance)
  //EVT_BUTTON(ID_MotorIncreaseDistance,	MyFrame::OnMotorIncreaseDistance)
  EVT_BUTTON(ID_LoadLimitSet1, MyFrame::OnLimitsLoadSet1)
  EVT_BUTTON(ID_LoadLimitSet2, MyFrame::OnLimitsLoadSet2)
  EVT_BUTTON(ID_LoadLimitSet3, MyFrame::OnLimitsLoadSet3)
  EVT_BUTTON(ID_LoadLimitSet4, MyFrame::OnLimitsLoadSet4)
  //EVT_SPINCTRLDOUBLE(ID_MountingLength, MyFrame::OnMountingLengthChanged)
  EVT_BUTTON(ID_LimitsDistanceGoTo, MyFrame::OnLimitsGoTo)
  EVT_BUTTON(ID_SetLimits, MyFrame::OnLimitsSetLimits)
  EVT_RADIOBUTTON(ID_OneStepStressForce, MyFrame::OnOneStepStressForce)
  EVT_RADIOBUTTON(ID_OneStepDistance, MyFrame::OnOneStepDistance)
  EVT_BUTTON(ID_OneStepSendToProtocol, MyFrame::OnOneStepSendToProtocol)
  EVT_RADIOBUTTON(ID_ContinuousStressForce, MyFrame::OnContinuousStressForce)
  EVT_RADIOBUTTON(ID_ContinuousDistance, MyFrame::OnContinuousDistance)
  EVT_BUTTON(ID_ContinuousSendToProtocol, MyFrame::OnContinuousSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedPercent, MyFrame::OnPreloadSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedMm, MyFrame::OnPreloadSpeedMmChanged)
  EVT_BUTTON(ID_PreloadSendToProtocol, MyFrame::OnPreloadSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedPercent, MyFrame::OnConditioningSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedMm, MyFrame::OnConditioningSpeedMmChanged)
  EVT_BUTTON(ID_ConditioningSendToProtocol, MyFrame::OnConditioningSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedPercent, MyFrame::OnRamp2FailureSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedMm, MyFrame::OnRamp2FailureSpeedMmChanged)
  EVT_BUTTON(ID_Ramp2FailureSendToProtocol, MyFrame::OnRamp2FailureSendToProtocol)
  EVT_BUTTON(ID_ClearGraph, MyFrame::OnClearGraph)
  EVT_BUTTON(ID_ExportCSV, MyFrame::OnExportCSV)
  EVT_BUTTON(ID_DeleteExperiment, MyFrame::OnDeleteExperiment)
  EVT_BUTTON(ID_MoveUpExperiment, MyFrame::OnMoveUpExperiment)
  EVT_BUTTON(ID_MoveDownExperiment, MyFrame::OnMoveDownExperiment)
  EVT_BUTTON(ID_Preview, MyFrame::OnPreviewProtocol)
  EVT_BUTTON(ID_RunProtocol, MyFrame::OnRunProtocol)
  EVT_BUTTON(ID_StopProtocol, MyFrame::OnStopProtocol)
wxEND_EVENT_TABLE()

// Costum event definitions
wxDEFINE_EVENT(EVT_MYBUTTON_DOWN, wxCommandEvent);
wxDEFINE_EVENT(EVT_MYBUTTON_UP, wxCommandEvent);

/**
 * @brief Constructor of the main frame. Sets the icon.
 * @param Title of the software.
 * @param Pointer to the parent object.
 */
MyFrame::MyFrame(const wxString &title, Settings *settings, wxWindow *parent)
  : MyFrame_Base(title, parent),
    m_Settings(settings),
    m_CurrentPositions{0,0},
    m_CurrentDistance(150),
    m_CurrentForce(0),
    m_ForceUnit(wxT(" N")),
    m_MountingLength(150),
    //m_PreloadDistance(0),
    m_StressOrForce(StressOrForce::Force),
    m_CurrentProtocol(nullptr),
    //m_CurrentExperiment(NULL),
    //m_CurrentExperimentValues(NULL),
    m_StageMaxLimit(0),
    m_StageMinLimit(0),
    m_ForceMaxLimit(0),
    m_ForceMinLimit(0),
    m_Area(0),
    //m_ExperimentRunningFlag(false),
    m_PreloadDoneFlag(true),
    //m_MeasurementValuesRecordingFlag(false),
    m_CurrentForceUpdateDelay(0),
    m_VectorLayer(_("Vector")),
    m_XAxis(NULL),
    m_Y1Axis(NULL),
    m_Y2Axis(NULL),
    m_MessageHandlersFinishedNumber(0)
{

  SetIcon(wxICON(sample));

  // Set the required ID's
  m_DecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_IncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_InitializeHomeMotorsButton->SetId(ID_HomeStages);
  m_InitializeMountingLengthButton->SetId(ID_SetMountingLength);
  m_InitializeLoadStoredPositionButton->SetId(ID_LoadStoredPosition);
  m_InitializeUnitRadioBox->SetId(ID_Unit);
  wxString str;
  str << m_LimitsGoToSpinCtrl->GetValue();
  m_LimitsGoToSpinCtrl->SetValue(str + " mm");
  m_LimitsLimitSet1Button->SetId(ID_LoadLimitSet1);
  m_LimitsLimitSet2Button->SetId(ID_LoadLimitSet2);
  m_LimitsLimitSet3Button->SetId(ID_LoadLimitSet3);
  m_LimitsLimitSet4Button->SetId(ID_LoadLimitSet4);
  m_LimitsGoToSpinCtrl->SetId(ID_LimitsDistanceValue);
  m_LimitsLimitSetButton->SetId(ID_SetLimits);
  m_LimitsGoToButton->SetId((ID_LimitsDistanceGoTo));
  m_PreloadSpeedPreloadSpinCtrl->SetId(ID_PreloadSpeedPercent);
  m_PreloadSpeedMmSpinCtrl->SetId(ID_PreloadSpeedMm);
  m_PreloadSendButton->SetId(ID_PreloadSendToProtocol);
  m_OneStepStressForceRadioBtn->SetId(ID_OneStepStressForce);
  m_OneStepDistanceRadioBtn->SetId(ID_OneStepDistance);
  m_OneStepCancelButton->SetId(ID_OneStepCancel);
  m_OneStepSendButton->SetId(ID_OneStepSendToProtocol);
  m_ContinuousStressForceRadioBtn->SetId(ID_ContinuousStressForce);
  m_ContinuousDistanceRadioBtn->SetId(ID_ContinuousDistance);
  m_ContinuousCancelButton->SetId(ID_ContinuousCancel);
  m_ContinuousSendButton->SetId(ID_ContinuousSendToProtocol);
  m_ConditioningStressRadioBtn->SetId(ID_ConditioningStressLimit);
  m_ConditioningDistanceRadioBtn->SetId(ID_ConditioningDistanceLimit);
  m_StopButton->SetId(ID_MotorStop);
  m_ConditioningSpeedPreloadSpinCtrl->SetId(ID_ConditioningSpeedPercent);
  m_ConditioningSpeedMmSpinCtrl->SetId(ID_ConditioningSpeedMm);
  m_ConditioningSendButton->SetId(ID_ConditioningSendToProtocol);
  m_R2FSpeedPreloadSpinCtrl->SetId(ID_Ramp2FailureSpeedPercent);
  m_R2FSpeedMmSpinCtrl->SetId(ID_Ramp2FailureSpeedMm);
  m_R2FSendButton->SetId(ID_Ramp2FailureSendToProtocol);
  m_R2FAfterFailureRadioBox->SetId(ID_GoTo);
  m_GraphClearButton->SetId(ID_ClearGraph);
  m_GraphExportCSVButton->SetId(ID_ExportCSV);
  m_ProtocolsXButton->SetId(ID_DeleteExperiment);
  m_ProtocolsUpButton->SetId(ID_MoveUpExperiment);
  m_ProtocolsDownButton->SetId(ID_MoveDownExperiment);
  m_ProtocolsLoopCheckBox->SetId(ID_LoopProtocol);
  m_ProtocolsPreviewButton->SetId(ID_Preview);
  m_ProtocolsRunButton->SetId(ID_RunProtocol);
  m_ProtocolsStopButton->SetId(ID_StopProtocol);
  m_ProtocolsSaveButton->SetId(ID_SaveProtocol);
  m_ProtocolsLoadButton->SetId(ID_LoadProtocol);

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

  // Create graph
  m_Graph = new mpWindow(m_GraphPanel, wxID_ANY);

  // Define layer for the graph
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_VectorLayer.SetContinuity(true);
    m_StressForcePreviewVector.SetContinuity(true);
    m_DistancePreviewVector.SetContinuity(true);
    wxPen vectorpen(*wxBLUE, 2, wxSOLID);
    m_VectorLayer.SetPen(vectorpen);
    m_StressForcePreviewVector.SetPen(vectorpen);
    m_DistancePreviewVector.SetPen(vectorpen);
    m_VectorLayer.SetDrawOutsideMargins(false);
    m_StressForcePreviewVector.SetDrawOutsideMargins(false);
    m_DistancePreviewVector.SetDrawOutsideMargins(false);
  }

  // Add graph to window
  m_Graph->Fit();
  m_GraphSizer1->Insert(0, m_Graph, 0, wxEXPAND);
  m_GraphPanel->Layout();

  // Load file path
  m_StoragePath = m_Settings->getStoragePath();

  }

/**
 * @brief Returns the message handler wait condition variable as shared_ptr.
 * @return The message handler wait condition variable as shared_ptr.
 */
std::shared_ptr<std::condition_variable> MyFrame::getMessageHandlersWait(void){
  std::shared_ptr<std::condition_variable> messagehandlerswait(&m_WaitMessageHandlers, do_nothing_deleter_conditional_variable);
  return(messagehandlerswait);
}

/**
 * @brief Returns the message handler wait mutex as shared_ptr.
 * @return The message handler wait mutex as shared_ptr.
 */
std::shared_ptr<std::mutex> MyFrame::getMessageHandlersWaitMutex(void){
  std::shared_ptr<std::mutex> messagehandlerswaitmutex(&m_WaitMessageHandlersMutex, do_nothing_deleter_mutex);
  return(messagehandlerswaitmutex);
}

/**
 * @brief Returns the number of finished message handler variable as shared_ptr.
 * @return The number of finished message handler variable as shared_ptr.
 */
std::shared_ptr<int> MyFrame::getMessageHandlersFinishedNumber(void){
  std::shared_ptr<int> messagehandlerfinishednr(&m_MessageHandlersFinishedNumber, do_nothing_deleter_int);
  return(messagehandlerfinishednr);
}

/**
 * @brief Register the liner stages and the stage frame, registers the update method at the stage frame and registers the stop wait conditional variable at the stage frame.
 * @param linearstage Pointer to the vector containing the linear motors.
 */
void MyFrame::registerLinearStage(std::vector<LinearStage*> *linearstage, StageFrame *stageframe){
  m_LinearStages = linearstage;
  m_StageFrame = stageframe;

  // Registers update methods at stage frame.
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  // Registers stop wait conditional variable and mutex.
  m_StageFrame->registerStagesStopped(&m_StagesStoppedFlag, &m_StagesStoppedMutex);

  // Reset limit
  m_StageFrame->setMaxDistanceLimit(153);
  m_StageFrame->setMinDistanceLimit(0);
}

/**
 * @brief Registers the message handlers of the linear stages, registers the stage frame at the linear stage message handlers.
 * @param linearstagesmessagehandlers Pointer to the vector containing the message handlers of the linear motors.
 */
void MyFrame::registerLinearStageMessageHandlers(std::vector<LinearStageMessageHandler*> *linearstagesmessagehandlers){
  m_LinearStagesMessageHandlers = linearstagesmessagehandlers;

  // Register stage frame
  (m_LinearStagesMessageHandlers->at(0))->registerStageFrame(m_StageFrame);
  (m_LinearStagesMessageHandlers->at(1))->registerStageFrame(m_StageFrame);
}

/**
 * @brief Register the force sensor.
 * @param forcesensor Pointer to the force sensor.
 */
void MyFrame::registerForceSensor(ForceSensor *forcesensor){
  m_ForceSensor = forcesensor;

  // Registers update method at forcesensormessagehandler.
  m_ForceSensorMessageHandler = m_ForceSensor->getMessageHandler();
  m_ForceId = m_ForceSensorMessageHandler->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);
}

/**
 * @brief Destructor
 */
MyFrame::~MyFrame(){
  // Unregister update methods
  m_ForceSensorMessageHandler->unregisterUpdateMethod(m_ForceId);
  m_StageFrame->unregisterUpdateMethod(m_DistanceId);
  // Stop linear stage receiver threads
  ((m_LinearStages->at(0))->getMessageHandler())->setExitFlag(false);
  ((m_LinearStages->at(1))->getMessageHandler())->setExitFlag(false);

  // Stop force sensor receiver thread
  (m_ForceSensor->getMessageHandler())->setExitFlag(false);

  // Remove vector, and the axis from graph.
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);
  m_Graph->DelLayer(m_XAxis);
  m_Graph->DelLayer(m_Y1Axis);
  m_Graph->DelLayer(m_Y2Axis);

  // Remove all layers and destroy the objects.
  //m_Graph->DelAllLayers(true, false);

  if(NULL != m_Graph){
    delete m_Graph;
  }
  if(NULL != m_XAxis){
    delete m_XAxis;
  }
  if(NULL != m_Y1Axis){
    delete m_Y1Axis;
  }

  {
    // Wait until the message handlers are finished.
    std::unique_lock<std::mutex> lck(m_WaitMessageHandlersMutex);
    m_WaitMessageHandlers.wait(lck, [&]{return(m_MessageHandlersFinishedNumber>=3);});
    std::cout << "MyFrame all 3 handlers should be finished." << std::endl;
  }

  // Delete linear stage objects.
  // Not needed because destructor of main.cpp deletes them.
  /*
  for(auto x : *m_LinearStages){
    delete x;
  }
  delete m_ForceSensor;
  */
}

/**
 * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
 * 				thread. (CallAfter() asynchronously call the updateDistance method)
 * @param value The position of a stage or a force.
 * @param type	Defines the type of the value (position of stage 1, 2 or force)
 */
void MyFrame::updateValues(MeasurementValue measurementValue, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = measurementValue.value;
      CallAfter(&MyFrame::updateDistance);
      break;
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;

      m_CurrentForceUpdateDelay++;
      if(20 <= m_CurrentForceUpdateDelay){
        m_CurrentForceUpdateDelay = 0;
        CallAfter(&MyFrame::updateForce);
      }
      break;
  }
}

/**
 * @brief Hides calculate diameter options, hides cells panel in chamber stretch, hides distance limit options, hides go to options,
 * 				sets digits for the wxSpinCtrlDouble.
 */
void MyFrame::startup(void){
  // Hide distance limit options
  /*
  m_ConditioningDistanceLimitStaticText->Show(false);
  m_ConditioningDistanceLimitSpinCtrl->Show(false);
  m_ConditioningDisctanceLimitRadioBox->Show(false);
  */
  // Change the limit set butten names
  const wxString label1 = "Load " + m_Settings->getSet1Name() + " limits";
  m_LimitsLimitSet1Button->SetLabelText(label1);
  const wxString label2 = "Load " + m_Settings->getSet2Name() + " limits";
  m_LimitsLimitSet2Button->SetLabelText(label2);
  const wxString label3 = "Load " + m_Settings->getSet3Name() + " limits";
  m_LimitsLimitSet3Button->SetLabelText(label3);
  const wxString label4 = "Load " + m_Settings->getSet4Name() + " limits";
  m_LimitsLimitSet4Button->SetLabelText(label4);

  // Hide Go to options
  m_R2FGoToSpinCtrl->Show(false);
  m_R2FGoToRadioBox->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_LimitsGoToSpinCtrl->SetDigits(2);
  m_InitializeCrossSectionSpinCtrl->SetDigits(2);
  m_LimitsLimitMaxDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMaxForceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinForceSpinCtrl->SetDigits(2);
  m_PreloadLimitSpinCtrl->SetDigits(2);
  m_PreloadSpeedPreloadSpinCtrl->SetDigits(2);
  m_PreloadSpeedMmSpinCtrl->SetDigits(2);
  m_OneStepStressForceVelocitySpinCtrl->SetDigits(2);
  m_OneStepStressForceHoldTime1SpinCtrl->SetDigits(2);
  m_OneStepStressForceUpperLimitSpinCtrl->SetDigits(2);
  m_OneStepStressForceHoldTime2SpinCtrl->SetDigits(2);
  m_OneStepStressForceLowerLimitSpinCtrl->SetDigits(2);
  m_OneStepDistanceVelocitySpinCtrl->SetDigits(2);
  m_OneStepDistanceHoldTime1SpinCtrl->SetDigits(2);
  m_OneStepDistanceUpperLimitSpinCtrl->SetDigits(2);
  m_OneStepDistanceHoldTime2SpinCtrl->SetDigits(2);
  m_OneStepDistanceLowerLimitSpinCtrl->SetDigits(2);
  m_OneStepEndOfEventHoldSpinCtrl->SetDigits(2);
  m_ContinuousStressForceVelocitySpinCtrl->SetDigits(2);
  m_ContinuousStressForceHoldTime1SpinCtrl->SetDigits(2);
  m_ContinuousStressForceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousStressForceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousDistanceVelocitySpinCtrl->SetDigits(2);
  m_ContinuousDistanceHoldTime1SpinCtrl->SetDigits(2);
  m_ContinuousDistanceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousDistanceMaxValueSpinCtrl->SetDigits(2);
  m_ConditioningSpeedMmSpinCtrl->SetDigits(2);
  m_ConditioningSpeedPreloadSpinCtrl->SetDigits(2);
  m_ConditioningStressForceLimitSpinCtrl->SetDigits(2);
  m_ConditioningDistanceLimitSpinCtrl->SetDigits(2);
  m_R2FSpeedPreloadSpinCtrl->SetDigits(2);
  m_R2FSpeedMmSpinCtrl->SetDigits(2);
  m_R2FDropBStopSpinCtrl->SetDigits(2);
  m_R2FGoToSpinCtrl->SetDigits(2);
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
 * @brief Method wich will be executed, when the user goes to the frequency settings.
 * @param event Occuring event
 */
void MyFrame::OnSamplingFrequencySettings(wxCommandEvent& event){
  MySamplingFrequency_Base *samplingFrequency = new MySamplingFrequency_Base(this);
  samplingFrequency->Show();
}

/**
 * @brief Method wich will be executed, when the user goes to the port settings.
 * @param event Occuring event
 */
void MyFrame::OnPortsSettings(wxCommandEvent& event){
  MyPorts *ports = new MyPorts(m_Settings, m_LinearStages, m_ForceSensor, this);

	ports->Show();
}

/**
 * @brief Method wich will be executed, when the user goes to the file output settings.
 * @param event Occuring event
 */
void MyFrame::OnFileOutputSettings(wxCommandEvent& event){
  MyFileOutput *fileOutput = new MyFileOutput(this, m_Settings, m_StoragePath, this);
	fileOutput->Show();
}

/**
 * @brief Method wich will be executed, when the user chooses stress as unit.
 * @param event Occuring event
 */
void MyFrame::OnUnit(wxCommandEvent& event){
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_PreloadLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_ForceUnit = wxT(" kPa");

    m_Graph->DelLayer(m_Y1Axis);
    if(NULL != m_Y1Axis){
      delete m_Y1Axis;
      m_Y1Axis = NULL;
    }
    m_Y1Axis = new mpScaleY(wxT("Stress [kPa]"), mpALIGN_LEFT, true);
    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_Y1Axis->SetFont(graphFont);
    m_Graph->AddLayer(m_Y1Axis);
    m_Graph->Fit();

    m_StressOrForce = StressOrForce::Stress;
  }else{
    m_PreloadLimitStaticText->SetLabelText("Force Limit [N]");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Force Limit [N]");
    m_ForceUnit = wxT(" N");

    m_Graph->DelLayer(m_Y1Axis);
    if(NULL != m_Y1Axis){
      delete m_Y1Axis;
      m_Y1Axis = NULL;
    }
    m_Y1Axis = new mpScaleY(wxT("Force [N]"), mpALIGN_LEFT, true);
    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_Y1Axis->SetFont(graphFont);
    m_Graph->AddLayer(m_Y1Axis);
    m_Graph->Fit();

    m_StressOrForce = StressOrForce::Force;
  }
}

/**
 * @brief Method wich will be executed, when the user chooses distance as limit.
 * @param event Occuring event
 */
void MyFrame::OnDistanceLimit(wxCommandEvent& event){
  /*
  m_ConditioningDistanceLimitStaticText->Show(true);
  m_ConditioningDistanceLimitSpinCtrl->Show(true);
  m_ConditioningDisctanceLimitRadioBox->Show(true);

  m_ConditioningStressForceLimitStaticText->Show(false);
  m_ConditioningStressForceLimitSpinCtrl->Show(false);
  */
}

/**
 * @brief Method wich will be executed, when the user chooses stress as limit.
 * @param event Occuring event
 */
void MyFrame::OnStressLimit(wxCommandEvent& event){
  /*
  m_ConditioningStressForceLimitStaticText->Show(true);
  m_ConditioningStressForceLimitSpinCtrl->Show(true);

  m_ConditioningDistanceLimitStaticText->Show(false);
  m_ConditioningDistanceLimitSpinCtrl->Show(false);
  m_ConditioningDisctanceLimitRadioBox->Show(false);
  */
}

/**
 * @brief Method wich will be executed, when the user chooses "Go to" after failure in R2F.
 * @param event Occuring event
 */
void MyFrame::OnGoTo(wxCommandEvent& event){
  if(2 == m_R2FAfterFailureRadioBox->GetSelection()){
    m_R2FGoToSpinCtrl->Show(true);
    m_R2FGoToRadioBox->Show(true);
  }else{
    m_R2FGoToSpinCtrl->Show(false);
    m_R2FGoToRadioBox->Show(false);
  }
}

/**
 * @brief Method wich will be executed, when the user klicks on load stored position button.
 * @param event Occuring event
 */
void MyFrame::OnInitializeLoadStoredPosition(wxCommandEvent& event){
  (m_LinearStages->at(0))->loadStoredPosition();
  (m_LinearStages->at(1))->loadStoredPosition();
}

/**
 * @brief Method wich will be executed, when the user klicks on the home stage button.
 * @param event Occuring event
 */
void MyFrame::OnInitializeHomeLinearStages(wxCommandEvent& event){
  /*
  (m_LinearStages->at(0))->home();
  (m_LinearStages->at(1))->home();
  */
  MyHomeStages *homestages = new MyHomeStages(m_LinearStages, this);
  homestages->Show();
}

/**
 * @brief Method wich will be executed, when the user clicks on the set length button.
 * @param event Occuring event
 */
void MyFrame::OnInitializeSetMountingLength(wxCommandEvent& event){
  m_StageFrame->setZeroDistance();
  m_StageFrame->setMaxDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
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
 * @brief Method wich will be executed, when the user changes the clamping position value.
 * @param event Occuring event
 */
void MyFrame::OnMountingLengthChanged(wxSpinDoubleEvent& event){
  m_MountingLength = m_LimitsGoToSpinCtrl->GetValue();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Go to" button in clamping position.
 * @param event Occuring event
 */
void MyFrame::OnLimitsGoTo(wxCommandEvent& event){
  m_StageFrame->gotoMMDistance(m_MountingLength);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedPercentChanged(wxSpinDoubleEvent& event){
 double speedmm = m_MountingLength * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 m_PreloadSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_PreloadSpeedMmSpinCtrl->GetValue() / m_MountingLength * 100/*%*/;
  m_PreloadSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSendToProtocol(wxCommandEvent& event){
  checkProtocol();

  m_Area = m_InitializeCrossSectionSpinCtrl->GetValue();

  //Experiment* experiment = new Preload(ExperimentType::Preload,
  std::unique_ptr<Experiment> experiment(new Preload(ExperimentType::Preload,
                                                     m_StressOrForce,
                                                     Experiment::DistanceOrPercentage::Distance,
                                                     m_StageFrame,
                                                     m_ForceSensorMessageHandler,
                                                     &m_VectorLayer,
                                                     &m_VectorLayerMutex,
                                                     this,
                                                     m_StoragePath,
                                                     &m_Wait,
                                                     &m_WaitMutex,
                                                     &m_StagesStoppedFlag,
                                                     &m_StagesStoppedMutex,
                                                     m_PreloadLimitSpinCtrl->GetValue(),
                                                     m_PreloadSpeedMmSpinCtrl->GetValue(),
                                                     m_Area));
                                                     //m_Area);

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Method wich will be executed, when the user chooses stress/force in the one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepStressForce(wxCommandEvent& event){
  m_OneStepDistancePanel->Show(false);
  m_OneStepStressForcePanel->Show(true);
  m_OneStepPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses distance in the one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepDistance(wxCommandEvent& event){
  m_OneStepStressForcePanel->Show(false);
  m_OneStepDistancePanel->Show(true);
  m_OneStepPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepSendToProtocol(wxCommandEvent& event){
  checkProtocol();

  DistanceOrStressForce distanceOrStressForce;
  double holdtime1;
  long upperlimit;
  double holdtime2;
  long lowerlimit;
  if(true == m_OneStepStressForceRadioBtn->GetValue()){
    distanceOrStressForce = DistanceOrStressForce::StressForce;
    holdtime1 = m_OneStepStressForceHoldTime1SpinCtrl->GetValue();
    upperlimit = m_OneStepStressForceUpperLimitSpinCtrl->GetValue();
    holdtime2 = m_OneStepStressForceHoldTime2SpinCtrl->GetValue();
    lowerlimit = m_OneStepStressForceLowerLimitSpinCtrl->GetValue();
  }else if(true == m_OneStepDistanceRadioBtn->GetValue()){
    distanceOrStressForce = DistanceOrStressForce::Distance;
    holdtime1 = m_OneStepDistanceHoldTime1SpinCtrl->GetValue();
    upperlimit = m_OneStepDistanceUpperLimitSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
    holdtime2 = m_OneStepDistanceHoldTime2SpinCtrl->GetValue();
    lowerlimit = m_OneStepDistanceLowerLimitSpinCtrl->GetValue();
  }

  Experiment::DistanceOrPercentage distanceOrPercentage;
  if(true == m_OneStepStressForceVelocityMmRadioBtn->GetValue()){
    distanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
  } else if(true == m_OneStepStressForceVelocityPercentRadioBtn->GetValue()){
    distanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
  }

  Experiment::DistanceOrPercentage holddistanceOrPercentage;
  if(true == m_OneStepEndOfEventHoldMmRadioBtn->GetValue()){
    holddistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
  } else if(true == m_OneStepEndOfEventHoldPercentRadioBtn->GetValue()){
    holddistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
  }
  long holddistance = m_OneStepEndOfEventHoldSpinCtrl->GetValue();

  int cycles = 1;
  if(true == m_OneStepEndOfEventRepeatCheckBox->GetValue()){
    cycles = m_OneStepEndOfEventRepeatSpinCtrl->GetValue();
  }

  Experiment::BehaviorAfterStop behaviorAfterStop;
  if(true == m_OneStepEndOfEventHoldMmRadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::HoldADistance;
  } else if(true == m_OneStepEndOfEventL0RadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::GoToL0;
  }

  m_Area = m_InitializeCrossSectionSpinCtrl->GetValue();

  std::unique_ptr<Experiment> experiment(new OneStepEvent(ExperimentType::OneStepEvent,
                                                          distanceOrStressForce,
                                                          m_StressOrForce,
                                                          distanceOrPercentage,
                                                          m_StageFrame,
                                                          m_ForceSensorMessageHandler,
                                                          &m_VectorLayer,
                                                          &m_VectorLayerMutex,
                                                          this,
                                                          m_StoragePath,
                                                          &m_Wait,
                                                          &m_WaitMutex,
                                                          &m_StagesStoppedFlag,
                                                          &m_StagesStoppedMutex,
                                                          m_OneStepStressForceVelocitySpinCtrl->GetValue(),
                                                          holdtime1,
                                                          upperlimit,
                                                          holdtime2,
                                                          lowerlimit,
                                                          holddistanceOrPercentage,
                                                          holddistance,
                                                          cycles,
                                                          behaviorAfterStop,
                                                          m_Area,
                                                          m_CurrentDistance));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Method wich will be executed, when the user chooses stress/force in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousStressForce(wxCommandEvent& event){
  m_ContinuousDistancePanel->Show(false);
  m_ContinuousStressForcePanel->Show(true);
  m_ContinuousPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses distance in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousDistance(wxCommandEvent& event){
  m_ContinuousStressForcePanel->Show(false);
  m_ContinuousDistancePanel->Show(true);
  m_ContinuousPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousSendToProtocol(wxCommandEvent& event){

}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSpeedPercentChanged(wxSpinDoubleEvent& event){
 //double speedmm = m_PreloadDistance * 0.00009921875/*mm per micro step*/ * (m_ConditioningSpeedPreloadSpinCtrl->GetValue() / 100);
 //m_ConditioningSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSpeedMmChanged(wxSpinDoubleEvent& event){
  //double speedpercent = m_ConditioningSpeedMmSpinCtrl->GetValue() / (m_PreloadDistance * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  //m_ConditioningSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSendToProtocol(wxCommandEvent& event){

  DistanceOrStressForce distanceOrStressForce;
  if(true == m_ConditioningStressRadioBtn->GetValue()){
    distanceOrStressForce = DistanceOrStressForce::StressForce;
  }else if(true == m_ConditioningDistanceRadioBtn->GetValue()){
    distanceOrStressForce = DistanceOrStressForce::Distance;
  }

  int calculatelimit = m_ConditioningDistanceLimitSpinCtrl->GetValue();
  Experiment::DistanceOrPercentage dp;
  switch(m_ConditioningDisctanceLimitRadioBox->GetSelection()){
    case 0:
      //distancelimit = static_cast<long>(m_PreloadDistance + m_ConditioningDistanceLimitSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/);
      dp = Experiment::DistanceOrPercentage::Distance;
      break;
    case 1:
      //distancelimit = ((m_ConditioningDistanceLimitSpinCtrl->GetValue() / 100) + 1.0) * m_PreloadDistance;
      dp = Experiment::DistanceOrPercentage::Percentage;
      break;
  }

  /*
  std::unique_ptr<Experiment> experiment(new Conditioning(ExperimentType::Conditioning,
                                         distanceOrStressForce,
                                         m_StressOrForce,
                                         m_CurrentDistance,
                                         m_StageFrame,
                                         m_LinearStagesMessageHandlers,
                                         m_ForceSensorMessageHandler,
                                         &m_VectorLayer,
                                         &m_VectorLayerMutex,
                                         this,
                                         m_StoragePath,
                                         &m_Wait,
                                         &m_WaitMutex,
                                         m_ConditioningStressForceLimitSpinCtrl->GetValue(),
                                         m_ConditioningCyclesSpinCtrl->GetValue(),
                                         dp,
                                         calculatelimit,
                                         m_ConditioningSpeedMmSpinCtrl->GetValue(),
                                         m_Area));

  //m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
  m_CurrentProtocol->addExperiment(experiment);
  */

  return;
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSpeedPercentChanged(wxSpinDoubleEvent& event){
  //double speedmm = m_PreloadDistance * 0.00009921875/*mm per micro step*/ * (m_R2FSpeedPreloadSpinCtrl->GetValue() / 100.0);
  //m_R2FSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSpeedMmChanged(wxSpinDoubleEvent& event){
  //double speedpercent = m_R2FSpeedMmSpinCtrl->GetValue() / (m_PreloadDistance * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  //m_R2FSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSendToProtocol(wxCommandEvent& event){

  Ramp2Failure::BehaviorAfterFailure behavior = Ramp2Failure::BehaviorAfterFailure::PreloadPos;
  switch(m_R2FAfterFailureRadioBox->GetSelection()){
    case 0:
      behavior = Ramp2Failure::BehaviorAfterFailure::PreloadPos;
      break;

    case 1:
      behavior = Ramp2Failure::BehaviorAfterFailure::Stop;
      break;

    case 2:
      behavior = Ramp2Failure::BehaviorAfterFailure::GoTo;
      break;
  }

  long distanceafterfailure = 0;
  if(0 == m_R2FGoToRadioBox->GetSelection()){
    //distanceafterfailure = m_R2FGoToSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
  }else if(1 == m_R2FGoToRadioBox->GetSelection()){
    //distanceafterfailure = ((m_R2FGoToSpinCtrl->GetValue() / 100) /*+ 1.0*/) * m_PreloadDistance;
  }

  /*
  m_CurrentExperiment = new Ramp2Failure(ExperimentType::Ramp2Failure,
                                         m_StressOrForce,
                                         m_StageFrame,
                                         m_LinearStagesMessageHandlers,
                                         m_ForceSensorMessageHandler,
                                         &m_VectorLayer,
                                         &m_VectorLayerMutex,
                                         this,
                                         m_StoragePath,
                                         &m_Wait,
                                         &m_WaitMutex,
                                         behavior,
                                         m_R2FSpeedMmSpinCtrl->GetValue(),
                                         m_R2FDropBStopSpinCtrl->GetValue(),
                                         m_Area,
                                         m_PreloadDistance,
                                         distanceafterfailure);

  m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
  */

  return;
}

/**
 * @brief Method wich will be executed, when the user sets the limits.
 * @param event Occuring event
 */
void MyFrame::OnLimitsSetLimits(wxCommandEvent& event){
  m_StageMaxLimit = m_LimitsLimitMaxDistanceSpinCtrl->GetValue();
  m_StageMinLimit = m_LimitsLimitMinDistanceSpinCtrl->GetValue();
  m_ForceMaxLimit = m_LimitsLimitMaxForceSpinCtrl->GetValue();
  m_ForceMinLimit = m_LimitsLimitMinForceSpinCtrl->GetValue();

  m_StageFrame->setMaxDistanceLimit(m_StageMaxLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_StageFrame->setMinDistanceLimit(m_StageMinLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
}

/**
 * @brief Method wich will be executed, when the user klicks on the decrease distance button.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistance(wxCommandEvent& event){
  std::cout << "MyFrame decrease distance" << std::endl;
  if(NULL != m_LinearStages){
    m_StageFrame->moveMM(0.25);

    // If the clamping position tab is active.
    if(1 == m_Experiments->GetSelection()){
      m_MountingLength -= (2 * 0.25);
      m_LimitsGoToSpinCtrl->SetValue(m_MountingLength);
    }
  }
}

/**
 * @brief Method wich will be executed, when the user klicks on the increase distance button.
 * @param event Occuring event
 */
void MyFrame::OnMotorIncreaseDistance(wxCommandEvent& event){
  std::cout << "MyFrame increase distance" << std::endl;
  if(NULL != m_LinearStages){
    m_StageFrame->moveMM(-0.25);
  }

  // If the clamping position tab is active.
  if(1 == m_Experiments->GetSelection()){
    m_MountingLength += (2 * 0.25);
    m_LimitsGoToSpinCtrl->SetValue(m_MountingLength);
  }
}

/**
 * @brief Method wich will be executed, when the decrease stage distance button is pushed down.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistanceStart(wxCommandEvent& event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  m_StageFrame->moveForward(4/*mm/s*/);
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the decrease stage distance button is released.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistanceStop(wxCommandEvent& event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  m_StageFrame->stop();
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the increase stage distance button is pushed down.
 * @param event Occuring event
 */
void MyFrame::OnMotorIncreaseDistanceStart(wxCommandEvent &event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  m_StageFrame->moveBackward(4/*mm/s*/);
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the increase stage distance button is released.
 * @param event Occuring event
 */
void MyFrame::OnMotorIncreaseDistanceStop(wxCommandEvent& event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  m_StageFrame->stop();
  event.Skip();
}

/**
 * @brief Method wich will be executed, when the user klicks on the motor stop button.
 * @param event Occuring event
 */
void MyFrame::OnMotorStop(wxCommandEvent& event){
  if(NULL != m_LinearStages){
    (m_LinearStages->at(0))->stop();
    (m_LinearStages->at(1))->stop();
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
 * @brief Method wich will be executed, when the user clicks on the export csv button.
 * @param event Occuring event
 */
void MyFrame::OnExportCSV(wxCommandEvent& event){
  //m_CurrentExperimentValues->exportCSV();
}

/**
 * @brief Method wich will be executed, when the user clicks on the clear graph button.
 * @param event Occuring event
 */
void MyFrame::OnClearGraph(wxCommandEvent& event){
  m_CurrentProtocol->clearGraphStop();
  /*
  {
    std::unique_lock<std::mutex> lck(m_MeasurementValuesRecordingMutex);
    if(true == m_MeasurementValuesRecordingFlag){
      m_MeasurementValuesRecordingFlag = false;
      m_CurrentExperimentValues->stopMeasurement();
    }
  }
      */
  //m_Graph->DelLayer(&m_VectorLayer);
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_VectorLayer.Clear();
    std::cout << "MyFrame cleared graph." << std::endl;
  }
  m_Graph->Fit();
  //delete m_CurrentExperimentValues;
}

/**
 * @brief Method wich will be executed, when the user clicks on the delete experiment button.
 * @param event Occuring event
 */
void MyFrame::OnDeleteExperiment(wxCommandEvent& event){
  m_CurrentProtocol->removeExperiment(m_ProtocolsListBox->GetSelection());
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment up button.
 * @param event Occuring event
 */
void MyFrame::OnMoveUpExperiment(wxCommandEvent& event){
  m_CurrentProtocol->moveExperimentUp(m_ProtocolsListBox->GetSelection());
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment down button.
 * @param event Occuring event
 */
void MyFrame::OnMoveDownExperiment(wxCommandEvent& event){
  m_CurrentProtocol->moveExperimentDown(m_ProtocolsListBox->GetSelection());
}

/**
 * @brief Method wich will be executed, when the user clicks on the preview protocol button.
 * @param event Occuring event
 */
void MyFrame::OnPreviewProtocol(wxCommandEvent& event){
  m_CurrentProtocol->makePreview();
}

/**
 * @brief Method wich will be executed, when the user clicks on the run protocol button.
 * @param event Occuring event
 */
void MyFrame::OnRunProtocol(wxCommandEvent& event){
  m_CurrentProtocol->runProtocol();
}

/**
 * @brief Method wich will be executed, when the user clicks on the stop protocol button.
 * @param event Occuring event
 */
void MyFrame::OnStopProtocol(wxCommandEvent& event){
  //OnMotorStop(event);
  m_CurrentProtocol->stopProtocol();
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
void MyFrame::updateForce(){
  wxString tmp;
  tmp << (static_cast<double>(m_CurrentForce) / 10000.0) << m_ForceUnit;
  m_ForceStaticText->SetLabel(tmp);
}

/**
 * @brief Prepares the graph to show the experiment values.
 */
void MyFrame::createValuesGraph(void){
  // Remove layers
  m_Graph->DelLayer(m_Y1Axis);
  m_Graph->DelLayer(m_Y2Axis);
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);

  // Clear vectors
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_VectorLayer.Clear();
  }

  // Add axis.
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  if(NULL == m_XAxis){
    delete m_XAxis;
    m_XAxis = NULL;
  }
  if(NULL == m_Y1Axis){
    delete m_Y1Axis;
    m_Y1Axis = NULL;
  }
  if(NULL == m_Y2Axis){
    delete m_Y2Axis;
    m_Y2Axis = NULL;
  }
  m_XAxis = new mpScaleX(wxT("Distance [mm]"), mpALIGN_BOTTOM, true, mpX_NORMAL);
  m_Y1Axis = new mpScaleY(wxT("Force [N]"), mpALIGN_LEFT, true);
  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis);
  m_Graph->AddLayer(m_Y1Axis);
  m_Graph->AddLayer(&m_VectorLayer);

  m_Graph->Fit();
}

/**
 * @brief Method which will be called from the class ExperimentValues to update the graph. Executes updateGraph() from the main thread.
 */
void MyFrame::updateGraphFromExperimentValues(){
  CallAfter(&MyFrame::updateGraph);
}

/**
 * @brief Method which will be called from the class Protocols to create the preview graph. Executes createPreviewGraph() from the main thread.
 */
void MyFrame::showPreviewGraph(){
  CallAfter(&MyFrame::createPreviewGraph);
}

/**
 * @brief Updates the graph in the GUI.
 */
void MyFrame::updateGraph(void){
  m_Graph->Fit();
  //std::cout << "ExperimentVaues graph fitted." << std::endl;
}

/**
 * @brief Creates the preview graph.
 */
void MyFrame::createPreviewGraph(void){
  // Remove layers
  m_Graph->DelLayer(m_Y1Axis);
  m_Graph->DelLayer(m_Y2Axis);
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);

  // Add axis.
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

  if(NULL == m_XAxis){
    delete m_XAxis;
    m_XAxis = NULL;
  }
  if(NULL == m_Y1Axis){
    delete m_Y1Axis;
    m_Y1Axis = NULL;
  }
  if(NULL == m_Y2Axis){
    delete m_Y2Axis;
    m_Y2Axis = NULL;
  }

  m_XAxis = new mpScaleX(wxT("Time"), mpALIGN_BOTTOM, true, mpX_NORMAL);
  m_Y1Axis = new mpScaleY(wxT("Force [N]"), mpALIGN_LEFT, true);
  m_Y2Axis = new mpScaleY(wxT("Distance [mm]"), mpALIGN_RIGHT, true);
  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis);
  m_Graph->AddLayer(m_Y1Axis);
  m_Graph->AddLayer(m_Y2Axis);
  m_Graph->AddLayer(&m_StressForcePreviewVector);
  m_Graph->AddLayer(&m_DistancePreviewVector);

  m_Graph->Fit();
}

/**
 * @brief Checks if a protocol object is already created, otherwise creates it.
 */
void MyFrame::checkProtocol(void){
  if(nullptr == m_CurrentProtocol){
  m_CurrentProtocol = std::unique_ptr<Protocols>(new Protocols(m_ProtocolsListBox,
                                                               this,
                                                               &m_StagesStoppedFlag,
                                                               &m_StagesStoppedMutex,
                                                               &m_WaitMutex,
                                                               &m_Wait,
                                                               &m_PreloadDoneFlag,
                                                               &m_PreloadDoneMutex,
                                                               &m_VectorLayer,
                                                               &m_StressForcePreviewVector,
                                                               &m_DistancePreviewVector,
                                                               m_StoragePath));
  }
}
