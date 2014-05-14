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
#include "../experiments/conditioning.h"
#include "../experiments/ramp2failure.h"
#include "../experiments/relaxation.h"
#include "../experiments/creep.h"
#include "../experiments/fatiguetesting.h"

#include <iostream>

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
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_RADIOBOX(ID_ChamberMeasurement, MyFrame::OnChamberMeasurement)
  EVT_RADIOBUTTON(ID_DistanceLimit, MyFrame::OnDistanceLimit)
  EVT_RADIOBUTTON(ID_StressLimit, MyFrame::OnStressLimit)
  EVT_RADIOBOX(ID_GoTo, MyFrame::OnGoTo)
  //EVT_BUTTON(ID_MotorDecreaseDistance,	MyFrame::OnMotorDecreaseDistance)
  //EVT_BUTTON(ID_MotorIncreaseDistance,	MyFrame::OnMotorIncreaseDistance)
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  EVT_BUTTON(ID_HomeStages, MyFrame::OnInitializeHomeLinearStages)
  EVT_BUTTON(ID_SetZeroLength, MyFrame::OnInitializeSetZeroLength)
  EVT_BUTTON(ID_LoadStoredPosition, MyFrame::OnInitializeLoadStoredPosition)
  EVT_BUTTON(ID_LoadLimitSet1, MyFrame::OnClampingPosLoadSet1)
  EVT_BUTTON(ID_LoadLimitSet2, MyFrame::OnClampingPosLoadSet2)
  EVT_BUTTON(ID_LoadLimitSet3, MyFrame::OnClampingPosLoadSet3)
  EVT_BUTTON(ID_LoadLimitSet4, MyFrame::OnClampingPosLoadSet4)
  EVT_SPINCTRLDOUBLE(ID_ClampingPosValue, MyFrame::OnClampingPosValueChanged)
  EVT_BUTTON(ID_ClampingGoTo, MyFrame::OnClampingGoTo)
  EVT_BUTTON(ID_SetLimits, MyFrame::OnClampingPosSetLimits)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedPercent, MyFrame::OnPreloadSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedMm, MyFrame::OnPreloadSpeedMmChanged)
  EVT_BUTTON(ID_PreloadSendToProtocol, MyFrame::OnPreloadSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedPercent, MyFrame::OnConditioningSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedMm, MyFrame::OnConditioningSpeedMmChanged)
  EVT_BUTTON(ID_ConditioningSendToProtocol, MyFrame::OnConditioningSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedPercent, MyFrame::OnRamp2FailureSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedMm, MyFrame::OnRamp2FailureSpeedMmChanged)
  EVT_BUTTON(ID_Ramp2FailureSendToProtocol, MyFrame::OnRamp2FailureSendToProtocol)
  EVT_BUTTON(ID_RelaxationSendToProtocol, MyFrame::OnRelexationSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_CreepSpeedPercent, MyFrame::OnCreepSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_CreepSpeedMm, MyFrame::OnCreepSpeedMmChanged)
  EVT_BUTTON(ID_CreepSendToProtocol, MyFrame::OnCreepSendToProtocol)
  EVT_BUTTON(ID_FatigueSendToProtocol, MyFrame::OnFatigueSendToProtocol)
  EVT_RADIOBOX(ID_ChamberStretchGelOrCell, MyFrame::OnChamberGelCells)
  EVT_BUTTON(ID_ClearGraph, MyFrame::OnClearGraph)
  EVT_BUTTON(ID_ExportCSV, MyFrame::OnExportCSV)
  EVT_BUTTON(ID_DeleteExperiment, MyFrame::OnDeleteExperiment)
  EVT_BUTTON(ID_MoveUpExperiment, MyFrame::OnMoveUpExperiment)
  EVT_BUTTON(ID_MoveDownExperiment, MyFrame::OnMoveDownExperiment)
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
    m_ForceUnit(wxT(" kPa")),
    m_ClampingDistance(150),
    //m_PreloadDistance(0),
    m_StressOrForce(StressOrForce::Force),
    m_CurrentProtocol(NULL),
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
    m_Y2Axis(NULL)
{

  SetIcon(wxICON(sample));

  // Set the required ID's
  wxString str;
  str << m_ClampingPositionSpinCtrl->GetValue();
  m_ClampingPositionSpinCtrl->SetValue(str + " mm");
  m_PreloadUnitRadioBox->SetId(ID_Unit);
  m_ConditioningStressRadioBtn->SetId(ID_StressLimit);
  m_ConditioningDistanceRadioBtn->SetId(ID_DistanceLimit);
  m_R2FAfterFailureRadioBox->SetId(ID_GoTo);
  m_ChamberStretchMeasurementRadioBox->SetId(ID_ChamberMeasurement);
  m_DecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_IncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_StopButton->SetId(ID_MotorStop);
  m_InitializeHomeMotorsButton->SetId(ID_HomeStages);
  m_InitializeCalibrationLengthButton->SetId(ID_SetZeroLength);
  m_InitializeLoadStoredPositionButton->SetId(ID_LoadStoredPosition);
  m_ClampingPositionLimitSet1Button->SetId(ID_LoadLimitSet1);
  m_ClampingPositionLimitSet2Button->SetId(ID_LoadLimitSet2);
  m_ClampingPositionLimitSet3Button->SetId(ID_LoadLimitSet3);
  m_ClampingPositionLimitSet4Button->SetId(ID_LoadLimitSet4);
  m_ClampingPositionSpinCtrl->SetId(ID_ClampingPosValue);
  m_ClampingPositionLimitSetButton->SetId(ID_SetLimits);
  m_ClampingPositionButton->SetId((ID_ClampingGoTo));
  m_PreloadSpeedPreloadSpinCtrl->SetId(ID_PreloadSpeedPercent);
  m_PreloadSpeedMmSpinCtrl->SetId(ID_PreloadSpeedMm);
  m_PreloadSendButton->SetId(ID_PreloadSendToProtocol);
  m_ConditioningSpeedPreloadSpinCtrl->SetId(ID_ConditioningSpeedPercent);
  m_ConditioningSpeedMmSpinCtrl->SetId(ID_ConditioningSpeedMm);
  m_ConditioningSendButton->SetId(ID_ConditioningSendToProtocol);
  m_R2FSpeedPreloadSpinCtrl->SetId(ID_Ramp2FailureSpeedPercent);
  m_R2FSpeedMmSpinCtrl->SetId(ID_Ramp2FailureSpeedMm);
  m_R2FSendButton->SetId(ID_Ramp2FailureSendToProtocol);
  m_RelaxationSendButton->SetId(ID_RelaxationSendToProtocol);
  m_CreepSpeedPreloadSpinCtrl->SetId(ID_CreepSpeedPercent);
  m_CreepSpeedMmSpinCtrl->SetId(ID_CreepSpeedMm);
  m_CreepSendButton->SetId(ID_CreepSendToProtocol);
  m_FatigueSendButton->SetId(ID_FatigueSendToProtocol);
  m_ChamberStretchMeasurementRadioBox->SetId(ID_ChamberStretchGelOrCell);
  m_GraphClearButton->SetId(ID_ClearGraph);
  m_GraphExportCSVButton->SetId(ID_ExportCSV);
  m_ProtocolsXButton->SetId(ID_DeleteExperiment);
  m_ProtocolsUpButton->SetId(ID_MoveUpExperiment);
  m_ProtocolsDownButton->SetId(ID_MoveDownExperiment);
  m_ProtocolsLoopCheckBox->SetId(ID_LoopProtocol);
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
  // Stop linear stage receiver threads
  ((m_LinearStages->at(0))->getMessageHandler())->setExitFlag(false);
  ((m_LinearStages->at(1))->getMessageHandler())->setExitFlag(false);

  // Stop force sensor receiver thread
  (m_ForceSensor->getMessageHandler())->setExitFlag(false);

  // Remove vector, and the axis from graph.
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(m_XAxis);
  m_Graph->DelLayer(m_Y1Axis);

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

  /*
  if(NULL != m_CurrentExperiment){
    delete m_CurrentExperiment;
  }
  if(NULL != m_CurrentExperimentValues){
    delete m_CurrentExperimentValues;
  }
  */

  for(auto x : *m_LinearStages){
    delete x;
  }

  delete m_ForceSensor;
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
  // Hide cells panel in Chamber stretch
  m_ChamberStretchCellsPanel->Show(false);

  // Hide distance limit options
  /*
  m_ConditioningDistanceLimitStaticText->Show(false);
  m_ConditioningDistanceLimitSpinCtrl->Show(false);
  m_ConditioningDisctanceLimitRadioBox->Show(false);
  */
  // Change the limit set butten names
  const wxString label1 = "Load " + m_Settings->getSet1Name() + " limits";
  m_ClampingPositionLimitSet1Button->SetLabelText(label1);
  const wxString label2 = "Load " + m_Settings->getSet2Name() + " limits";
  m_ClampingPositionLimitSet2Button->SetLabelText(label2);
  const wxString label3 = "Load " + m_Settings->getSet3Name() + " limits";
  m_ClampingPositionLimitSet3Button->SetLabelText(label3);
  const wxString label4 = "Load " + m_Settings->getSet4Name() + " limits";
  m_ClampingPositionLimitSet4Button->SetLabelText(label4);

  // Hide Go to options
  m_R2FGoToSpinCtrl->Show(false);
  m_R2FGoToRadioBox->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_ClampingPositionSpinCtrl->SetDigits(2);
  m_PreloadCrossSectionSpinCtrl->SetDigits(2);
  m_PreloadLimitSpinCtrl->SetDigits(2);
  m_PreloadSpeedPreloadSpinCtrl->SetDigits(2);
  m_PreloadSpeedMmSpinCtrl->SetDigits(2);
  m_ConditioningSpeedMmSpinCtrl->SetDigits(2);
  m_ConditioningSpeedPreloadSpinCtrl->SetDigits(2);
  m_ConditioningStressForceLimitSpinCtrl->SetDigits(2);
  m_ConditioningDistanceLimitSpinCtrl->SetDigits(2);
  m_R2FSpeedPreloadSpinCtrl->SetDigits(2);
  m_R2FSpeedMmSpinCtrl->SetDigits(2);
  m_R2FDropBStopSpinCtrl->SetDigits(2);
  m_R2FGoToSpinCtrl->SetDigits(2);
  m_RelaxationRampSpinCtrl->SetDigits(2);
  m_RelaxationPauseSpinCtrl->SetDigits(2);
  m_CreepSpeedPreloadSpinCtrl->SetDigits(2);
  m_CreepSpeedMmSpinCtrl->SetDigits(2);
  m_CreepHoldForceStressSpinCtrl->SetDigits(2);
  m_CreepHoldTimeSpinCtrl->SetDigits(2);
  m_FatigueTotalTimeSpinCtrl->SetDigits(2);
  m_FatigueAmplitudeSpinCtrl->SetDigits(2);
  m_FatigueRestTimeSpinCtrl->SetDigits(2);
  m_FatigueFrequencySpinCtrl->SetDigits(2);

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
  if(0 == m_PreloadUnitRadioBox->GetSelection()){
    m_PreloadLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_CreepHoldForceStressStaticText->SetLabelText("Hold Stress [kPa]");
    m_CreepSensitivityStaticText->SetLabelText("Sensitivity [kPa]");
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
    m_CreepHoldForceStressStaticText->SetLabelText("Hold Force [N]");
    m_CreepSensitivityStaticText->SetLabelText("Sensitivity [N]");
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
 * @brief Method wich will be executed, when the user chooses a measurement in Chamber stretch.
 * @param event Occuring event
 */
void MyFrame::OnChamberMeasurement(wxCommandEvent& event){
  if(0 == m_ChamberStretchMeasurementRadioBox->GetSelection()){
    m_ChamberStretchCellsPanel->Show(false);
    m_ChamberStretchGelPanel->Show(true);
    m_ChamberStretchSizer1->Layout();
  } else{
    m_ChamberStretchGelPanel->Show(false);
    m_ChamberStretchCellsPanel->Show(true);
    m_ChamberStretchSizer1->Layout();
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
void MyFrame::OnInitializeSetZeroLength(wxCommandEvent& event){
  m_StageFrame->setZeroDistance();
  m_StageFrame->setMaxDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
}

/**
 * @brief Loads limit set 1 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosLoadSet1(wxCommandEvent& event){
  m_ClampingPositionLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet1MaxDistance());
  m_ClampingPositionLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet1MinDistance());
  m_ClampingPositionLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet1MaxForce());
  m_ClampingPositionLimitMinForceSpinCtrl->SetValue(m_Settings->getSet1MinForce());
}

/**
 * @brief Loads limit set 2 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosLoadSet2(wxCommandEvent& event){
  m_ClampingPositionLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet2MaxDistance());
  m_ClampingPositionLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet2MinDistance());
  m_ClampingPositionLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet2MaxForce());
  m_ClampingPositionLimitMinForceSpinCtrl->SetValue(m_Settings->getSet2MinForce());
}

/**
 * @brief Loads limit set 3 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosLoadSet3(wxCommandEvent& event){
  m_ClampingPositionLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet3MaxDistance());
  m_ClampingPositionLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet3MinDistance());
  m_ClampingPositionLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet3MaxForce());
  m_ClampingPositionLimitMinForceSpinCtrl->SetValue(m_Settings->getSet3MinForce());
}

/**
 * @brief Loads limit set 4 from the configuration.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosLoadSet4(wxCommandEvent& event){
  m_ClampingPositionLimitMaxDistanceSpinCtrl->SetValue(m_Settings->getSet4MaxDistance());
  m_ClampingPositionLimitMinDistanceSpinCtrl->SetValue(m_Settings->getSet4MinDistance());
  m_ClampingPositionLimitMaxForceSpinCtrl->SetValue(m_Settings->getSet4MaxForce());
  m_ClampingPositionLimitMinForceSpinCtrl->SetValue(m_Settings->getSet4MinForce());
}

/**
 * @brief Method wich will be executed, when the user changes the clamping position value.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosValueChanged(wxSpinDoubleEvent& event){
  m_ClampingDistance = m_ClampingPositionSpinCtrl->GetValue();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Go to" button in clamping position.
 * @param event Occuring event
 */
void MyFrame::OnClampingGoTo(wxCommandEvent& event){
  m_StageFrame->gotoMMDistance(m_ClampingDistance);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedPercentChanged(wxSpinDoubleEvent& event){
 double speedmm = m_ClampingDistance * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 m_PreloadSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_PreloadSpeedMmSpinCtrl->GetValue() / m_ClampingDistance * 100/*%*/;
  m_PreloadSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSendToProtocol(wxCommandEvent& event){
  checkProtocol();

  m_Area = m_PreloadCrossSectionSpinCtrl->GetValue();

  std::unique_ptr<Experiment> experiment(new Preload(ExperimentType::Preload,
                                                     m_StressOrForce,
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

  m_CurrentProtocol->addExperiment(experiment);

  return;
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

  Conditioning::DistanceOrStressForce distanceOrStressForce;
  if(true == m_ConditioningStressRadioBtn->GetValue()){
    distanceOrStressForce = Conditioning::DistanceOrStressForce::StressForce;
  }else if(true == m_ConditioningDistanceRadioBtn->GetValue()){
    distanceOrStressForce = Conditioning::DistanceOrStressForce::Distance;
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
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in relexation.
 * @param event Occuring event
 */
void MyFrame::OnRelexationSendToProtocol(wxCommandEvent& event){

  long distance = 0;
  switch(m_RelaxationRampRadioBox->GetSelection()){
    case 0:
      distance = m_RelaxationRampSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
      break;

    case 1:
   //   distance = (m_RelaxationRampSpinCtrl->GetValue() / 100) * m_PreloadDistance;
      break;
  }

  /*
  m_CurrentExperiment = new Relaxation(ExperimentType::Relaxation,
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
                                       distance,
                                       m_RelaxationPauseSpinCtrl->GetValue(),
                                       m_RelaxationStepsSpinCtrl->GetValue(),
                                       m_Area,
                                       m_PreloadDistance);

  m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
  */

  return;
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in creep.
 * @param event Occuring event
 */
void MyFrame::OnCreepSpeedPercentChanged(wxSpinDoubleEvent& event){
  //double speedmm = m_PreloadDistance * 0.00009921875/*mm per micro step*/ * (m_CreepSpeedPreloadSpinCtrl->GetValue() / 100.0);
  //m_CreepSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in creep.
 * @param event Occuring event
 */
void MyFrame::OnCreepSpeedMmChanged(wxSpinDoubleEvent& event){
  //double speedpercent = m_CreepSpeedMmSpinCtrl->GetValue() / (m_PreloadDistance * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  //m_CreepSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in creep.
 * @param event Occuring event
 */
void MyFrame::OnCreepSendToProtocol(wxCommandEvent& event){

  /*
  m_CurrentExperiment = new Creep(ExperimentType::Creep,
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
                                  m_CreepHoldForceStressSpinCtrl->GetValue(),
                                  m_CreepHoldTimeSpinCtrl->GetValue(),
                                  m_CreepSensitivitySpinCtrl->GetValue(),
                                  m_CreepSpeedMmSpinCtrl->GetValue(),
                                  m_Area);

  m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
  */

  return;
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in fatigue.
 * @param event Occuring event
 */
void MyFrame::OnFatigueSendToProtocol(wxCommandEvent& event){

  long amplitude = 0;
  switch(m_FatigueAmplitudeRadioBox->GetSelection()){
    case 0:
      amplitude = m_FatigueAmplitudeSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
      break;

    case 1:
      //amplitude = (m_FatigueAmplitudeSpinCtrl->GetValue() / 100.0) * m_PreloadDistance;
      break;
  }

  /*
  m_CurrentExperiment = new FatigueTesting(ExperimentType::FatigueTesting,
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
                                           m_FatigueCyclesSpinCtrl->GetValue(),
                                           m_FatigueTotalTimeSpinCtrl->GetValue(),
                                           amplitude,
                                           m_FatigueRestTimeSpinCtrl->GetValue(),
                                           m_FatigueFrequencySpinCtrl->GetValue(),
                                           m_Area,
                                           m_PreloadDistance,
                                           m_CurrentDistance);

  m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
  */

  return;
}

/**
 * @brief Method wich will be executed, when the user changes from gel to cells or vica versa in chamber stretch.
 * @param event Occuring event
 */
void MyFrame::OnChamberGelCells(wxCommandEvent& event){
  if(0 == m_ChamberStretchMeasurementRadioBox->GetSelection()){
    // Hide cells panel in Chamber stretch
    m_ChamberStretchCellsPanel->Show(false);

    // Show gel panel in Chamber stretech
    m_ChamberStretchGelPanel->Show(true);
  }else if(1 == m_ChamberStretchMeasurementRadioBox->GetSelection()){
    // Hide gel panel in Chamber stretch
    m_ChamberStretchGelPanel->Show(false);

    // Show cells panel in Chamber stretech
    m_ChamberStretchCellsPanel->Show(true);
  }
  m_ChamberStretchPanel->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in chamber stretch.
 * @param event Occuring event
 */
void MyFrame::OnChamberStretchSendToProtocol(wxCommandEvent& event){

  // If gel is active
  if(0 == m_ChamberStretchMeasurementRadioBox->GetSelection()){

  }else if(1 == m_ChamberStretchMeasurementRadioBox->GetSelection()){ // If cells is active

    long amplitude = 0;
    switch(m_FatigueAmplitudeRadioBox->GetSelection()){
      case 0:
        amplitude = m_ChamberStretchAmplitudeSpinCtrl1->GetValue() / 0.00009921875/*mm per micro step*/;
        break;

      case 1:
        //amplitude = (m_ChamberStretchAmplitudeSpinCtrl1->GetValue() / 100.0) * m_PreloadDistance;
        break;
    }
    /*
    m_CurrentExperiment = new FatigueTesting(ExperimentType::ChamberStretchGel,
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
                                             m_ChamberStretchCyclesSpinCtrl1->GetValue(),
                                             m_ChamberStretchTotalTimeSpinCtrl1->GetValue(),
                                             amplitude,
                                             m_ChamberStretchRestTimeSpinCtrl1->GetValue()*3600,
                                             m_ChamberStretchFrequencySpinCtrl1->GetValue(),
                                             m_Area,
                                             m_PreloadDistance,
                                             m_CurrentDistance);

    m_CurrentExperimentValues = m_CurrentExperiment->getExperimentValues();
    */

    return;
  }
}

/**
 * @brief Method wich will be executed, when the user sets the limits.
 * @param event Occuring event
 */
void MyFrame::OnClampingPosSetLimits(wxCommandEvent& event){
  m_StageMaxLimit = m_ClampingPositionLimitMaxDistanceSpinCtrl->GetValue();
  m_StageMinLimit = m_ClampingPositionLimitMinDistanceSpinCtrl->GetValue();
  m_ForceMaxLimit = m_ClampingPositionLimitMaxForceSpinCtrl->GetValue();
  m_ForceMinLimit = m_ClampingPositionLimitMinForceSpinCtrl->GetValue();

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
      m_ClampingDistance -= (2 * 0.25);
      m_ClampingPositionSpinCtrl->SetValue(m_ClampingDistance);
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
    m_ClampingDistance += (2 * 0.25);
    m_ClampingPositionSpinCtrl->SetValue(m_ClampingDistance);
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
  m_CurrentProtocol->stopProtocol();
  OnMotorStop(event);
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
void MyFrame::showValuesGraph(void){
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
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);

  m_Graph->AddLayer(&m_StressForcePreviewVector);
  m_Graph->AddLayer(&m_DistancePreviewVector);

  // Add axis.
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  delete m_XAxis;
  m_XAxis = NULL;
  delete m_Y1Axis;
  m_Y1Axis = NULL;
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
  if(NULL == m_CurrentProtocol){
  m_CurrentProtocol = new Protocols(m_ProtocolsListBox,
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
                                    m_StoragePath);
  }
}
