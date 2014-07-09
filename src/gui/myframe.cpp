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
#include "mystartupdialog.h"
#include "myexportdialog.h"
#include "mypausedialog.h"
#include "protocols.h"
#include "../experiments/preload.h"
#include "../experiments/onestepevent.h"
#include "../experiments/continuousevent.h"
#include "../experiments/pause.h"
#include "../experiments/pauseresume.h"

#include <iostream>

// An deleter which doesn't do anything, required for passing shared_ptr.
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
  EVT_MENU(XRCID("m_SamplingFrequencyMenuItem"), MyFrame::OnSamplingFrequencySettings)
  EVT_MENU(XRCID("m_PortsMenuMenuItem"), MyFrame::OnPortsSettings)
  EVT_MENU(XRCID("m_FileOutputMenuItem"), MyFrame::OnFileOutputSettings)
  EVT_MENU(XRCID("m_StartUpDialogMenuItem"), MyFrame::OnOpenStartUpDialog)
  EVT_MENU(XRCID("m_LoadStoredPositionsMenuItem"), MyFrame::OnLoadStoredPositions)
  EVT_MENU(XRCID("m_HomeStagesMenuItem"), MyFrame::OnHomeLinearStages)
  EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookTabChanging)
  EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, MyFrame::OnNotebookTabChanging)
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  EVT_FILEPICKER_CHANGED(ID_LoadPreset, MyFrame::OnLoadPreset)
  EVT_BUTTON(ID_ApplyPreset, MyFrame::OnApplyPreset)
  EVT_BUTTON(ID_SavePreset, MyFrame::OnSavePreset)
  //EVT_BUTTON(ID_SetDistanceWActuatorCollision, MyFrame::OnLengthsSetDistanceWActuatorCollision)
  EVT_BUTTON(ID_SetMountingLength, MyFrame::OnLengthsSetMountingLength)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_SPINCTRLDOUBLE(ID_CrossSectionArea, MyFrame::OnCrossSectionAreaChange)
  EVT_BUTTON(ID_LoadLimitSet1, MyFrame::OnLimitsLoadSet1)
  EVT_BUTTON(ID_LoadLimitSet2, MyFrame::OnLimitsLoadSet2)
  EVT_BUTTON(ID_LoadLimitSet3, MyFrame::OnLimitsLoadSet3)
  EVT_BUTTON(ID_LoadLimitSet4, MyFrame::OnLimitsLoadSet4)
  EVT_BUTTON(ID_LengthsDistanceGoTo, MyFrame::OnLimitsGoTo)
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
  EVT_BUTTON(ID_ClearGraph, MyFrame::OnClearGraph)
  EVT_BUTTON(ID_ExportCSV, MyFrame::OnExportCSV)
  EVT_BUTTON(ID_ExportPNG, MyFrame::OnExportPNG)
  EVT_BUTTON(ID_DeleteExperiment, MyFrame::OnDeleteExperiment)
  EVT_BUTTON(ID_MoveUpExperiment, MyFrame::OnMoveUpExperiment)
  EVT_BUTTON(ID_MoveDownExperiment, MyFrame::OnMoveDownExperiment)
  EVT_BUTTON(ID_PauseExperiment, MyFrame::OnPauseExperiment)
  EVT_BUTTON(ID_PauseResumeExperiment, MyFrame::OnPauseResumeExperiment)
  EVT_BUTTON(ID_Preview, MyFrame::OnPreviewProtocol)
  EVT_BUTTON(ID_RunProtocol, MyFrame::OnRunProtocol)
  EVT_CHECKBOX(ID_LoopProtocol, MyFrame::OnLoopProtocol)
  EVT_BUTTON(ID_StopProtocol, MyFrame::OnStopProtocol)
  EVT_BUTTON(ID_SaveProtocol, MyFrame::OnSaveProtocol)
  EVT_BUTTON(ID_LoadProtocol, MyFrame::OnLoadProtocol)
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
    m_BlockNotebookTabFlag(false),
    m_Settings(settings),
    m_DistanceWActuatorCollisionSetFlag(false),
    m_CurrentPositions{0,0},
    m_CurrentDistance(150),
    m_CurrentForce(0),
    m_ForceUnit(wxT(" N")),
    m_MountingLength(150),
    //m_PreloadDistance(0),
    m_DistanceOrStressOrForce(DistanceOrStressOrForce::Force),
    m_CurrentProtocol(nullptr),
    m_MaxDistanceLimit(50 / 0.00009921875/*mm per micro step*/),
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

  SetIcon(wxICON(sample));

  // Set the required ID's
  m_InitializePresetFilePicker->SetId(ID_LoadPreset);
  m_InitializeApplyPresetButton->SetId(ID_ApplyPreset);
  m_InitializeSavePresetButton->SetId(ID_SavePreset);
  m_DecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_IncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_InitializeUnitRadioBox->SetId(ID_Unit);
  m_InitializeCrossSectionSpinCtrl->SetId(ID_CrossSectionArea);
  wxString str;
  str << m_LengthsGoToSpinCtrl->GetValue();
  m_LengthsGoToSpinCtrl->SetValue(str + " mm");
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
  m_GraphClearButton->SetId(ID_ClearGraph);
  m_GraphExportCSVButton->SetId(ID_ExportCSV);
  m_GraphExportPNGButton->SetId(ID_ExportPNG);
  m_ProtocolsXButton->SetId(ID_DeleteExperiment);
  m_ProtocolsUpButton->SetId(ID_MoveUpExperiment);
  m_ProtocolsDownButton->SetId(ID_MoveDownExperiment);
  m_ProtocolsPauseButton->SetId(ID_PauseExperiment);
  m_ProtocolsPauseResumeButton->SetId(ID_PauseResumeExperiment);
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
 * @brief Register the liner stages and the stage frame, registers the update method at the stage frame and registers the stop wait conditional variable at the stage frame.
 * @param linearstage Pointer to the vector containing the linear motors.
 */
void MyFrame::registerLinearStage(std::vector<std::shared_ptr<LinearStage>> &linearstage, std::shared_ptr<StageFrame> &stageframe){
  m_LinearStages.push_back(linearstage.at(0));
  m_LinearStages.push_back(linearstage.at(1));
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
void MyFrame::registerLinearStageMessageHandlers(std::vector<std::shared_ptr<LinearStageMessageHandler>> &linearstagesmessagehandlers){
  m_LinearStagesMessageHandlers.push_back(linearstagesmessagehandlers.at(0));
  m_LinearStagesMessageHandlers.push_back(linearstagesmessagehandlers.at(1));

  // Register stage frame
  (m_LinearStagesMessageHandlers.at(0))->registerStageFrame(m_StageFrame);
  (m_LinearStagesMessageHandlers.at(1))->registerStageFrame(m_StageFrame);
}

/**
 * @brief Register the force sensor.
 * @param forcesensor Pointer to the force sensor.
 */
void MyFrame::registerForceSensor(std::shared_ptr<ForceSensor> forcesensor){
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

  // Remove all layers and destroy the objects.
  //m_Graph->DelAllLayers(true, false);

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
 * 				thread. (CallAfter() asynchronously call the updateDistance method). Also performs the limit checks.
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
          // Stop stages and protocol if limit exceeded and indicate, that a limit exceeded.
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
            wxLogWarning(std::string("MyFrame: Distance limit exceeded, current distance: " + std::to_string(m_CurrentDistance)).c_str());
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
            wxLogMessage("MyFrame: Distance limit exceeded flag disabled.");
          }
        }
      }
      CallAfter(&MyFrame::updateDistance);

      break;
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = measurementValue.value;

      {
        std::lock_guard<std::mutex> lck{m_ForceLimitExceededMutex};
        /*
        if(false == m_DistanceWActuatorCollisionSetFlag){
          if(-50000.0 >= m_CurrentForce){
            m_StageFrame->stop();
            m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/    /*);
          }
        }else */if(false == m_ForceLimitExceededFlag){ // Check if no limits exceeded yet.
          // Stop stages and protocol if limit exceeded and indicate, that a limit exceeded.
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
            wxLogWarning(std::string("MyFrame: Force limit exceeded, current force: " + std::to_string(m_CurrentForce) +
                                     " m_MinForceLimit: " + std::to_string(m_MinForceLimit) +
                                     " m_MaxForceLimit: " + std::to_string(m_MaxForceLimit)).c_str());
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
            wxLogMessage("MyFrame: Force limit exceeded flag disabled.");
          }
        }
      }

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
  // Change the limit set butten names
  const wxString label1 = "Load " + m_Settings->getSet1Name() + " limits";
  m_LimitsLimitSet1Button->SetLabelText(label1);
  const wxString label2 = "Load " + m_Settings->getSet2Name() + " limits";
  m_LimitsLimitSet2Button->SetLabelText(label2);
  const wxString label3 = "Load " + m_Settings->getSet3Name() + " limits";
  m_LimitsLimitSet3Button->SetLabelText(label3);
  const wxString label4 = "Load " + m_Settings->getSet4Name() + " limits";
  m_LimitsLimitSet4Button->SetLabelText(label4);

  // Hide steps in ContinuousEvent
  m_ContinuousStressForceStepsStaticText->Show(false);
  m_ContinuousStressForceStepsSpinCtrl->Show(false);
  m_ContinuousDistanceStepsStaticText->Show(false);
  m_ContinuousDistanceStepsSpinCtrl->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_LengthsGoToSpinCtrl->SetDigits(2);
  m_InitializeCrossSectionSpinCtrl->SetDigits(2);
  m_LimitsLimitMaxDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinDistanceSpinCtrl->SetDigits(2);
  m_LimitsLimitMaxForceSpinCtrl->SetDigits(2);
  m_LimitsLimitMinForceSpinCtrl->SetDigits(2);
  m_LengthsForceStressSensitivitySpinCtrl->SetDigits(4);
  m_LengthsDistanceSensitivitySpinCtrl->SetDigits(4);
  m_PreloadLimitSpinCtrl->SetDigits(2);
  m_PreloadSpeedPreloadSpinCtrl->SetDigits(2);
  m_PreloadSpeedMmSpinCtrl->SetDigits(2);
  m_PreloadSpeedMmSpinCtrl->SetRange(0.05, 11);
  m_OneStepStressForceVelocitySpinCtrl->SetDigits(2);
  m_OneStepStressForceVelocitySpinCtrl->SetRange(0.05, 1000);
  m_OneStepStressForceDelaySpinCtrl->SetDigits(2);
  m_OneStepStressForceLimitSpinCtrl->SetDigits(2);
  m_OneStepStressForceDwellSpinCtrl->SetDigits(2);
  m_OneStepDistanceVelocitySpinCtrl->SetDigits(2);
  m_OneStepDistanceVelocitySpinCtrl->SetRange(0.05, 1000);
  m_OneStepDistanceDelaySpinCtrl->SetDigits(2);
  m_OneStepDistanceLimitSpinCtrl->SetDigits(2);
  m_OneStepDistanceDwellSpinCtrl->SetDigits(2);
  m_OneStepEndOfEventHoldSpinCtrl->SetDigits(2);
  m_ContinuousStressForceVelocitySpinCtrl->SetDigits(2);
  m_ContinuousStressForceVelocitySpinCtrl->SetRange(0.05, 1000);
  m_ContinuousStressForceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousStressForceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousStressForceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousDistanceVelocitySpinCtrl->SetDigits(2);
  m_ContinuousDistanceVelocitySpinCtrl->SetRange(0.05, 1000);
  m_ContinuousDistanceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousDistanceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousDistanceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousEndOfEventHoldSpinCtrl->SetDigits(2);

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
    m_StageFrame->setMinDistanceLimit(m_MinDistanceLimit);
    m_StageFrame->setMaxDistanceLimit(m_MaxDistanceLimit);
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
    std::unique_ptr<MyStartUpDialog> dialog = std::unique_ptr<MyStartUpDialog>(new MyStartUpDialog(this));
    dialog->ShowModal();
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
 * @brief Method wich will be executed, when the user goes to the frequency settings.
 * @param event Occuring event
 */
void MyFrame::OnSamplingFrequencySettings(wxCommandEvent& event){
  std::unique_ptr<MySamplingFrequency_Base> samplingFrequency = std::unique_ptr<MySamplingFrequency_Base>(new MySamplingFrequency_Base(this));
  samplingFrequency->ShowModal();
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
 * @brief Method wich will be executed, when the user changes a tab in the wxNotebook.
 * @param event Occuring event
 */
void MyFrame::OnNotebookTabChanging(wxBookCtrlEvent& event){
  if(false == m_BlockNotebookTabFlag){
    event.Skip(true);
  }else{
    event.Veto();
  }
}

/**
 * @brief Method wich will be executed, when the user opens the start up dialog.
 * @param event Occuring event
 */
void MyFrame::OnOpenStartUpDialog(wxCommandEvent& event){
  std::unique_ptr<MyStartUpDialog> dialog = std::unique_ptr<MyStartUpDialog>(new MyStartUpDialog(this));
  dialog->ShowModal();
}

/**
 * @brief Method wich will be executed, when the user chooses stress as unit.
 * @param event Occuring event
 */
void MyFrame::OnUnit(wxCommandEvent& event){
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_InitializeMinForceStaticText->SetLabelText("Min. stress [kPa]:");
    m_InitializeMaxForceStaticText->SetLabelText("Max. stress [kPa]:");
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal stress [kPa]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal stress [kPa]:");
    m_LengthsForceStressSensitivityStaticText->SetLabelText("Stress [kPa]");
    m_LengthsSetForceZeroButton->SetLabelText("Zero stress");
    m_PreloadLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [kPa]:");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [kPa]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText("Incrementd [dkPa]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("kPa");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Smax.");
    m_ContinuousEndOfEventHoldRadioBtn->SetLabelText("Stop at [kPa]:");
    m_ForceUnit = wxT(" kPa");

    if(true == m_ShowGraphFlag){
      wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
      m_Graph->DelLayer(m_Y1Axis.get());
      m_Y1Axis.reset(new mpScaleY(wxT("Stress [kPa]"), mpALIGN_LEFT, true));
      m_Y1Axis->SetPen(vectorpenStressForce);
      wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
      m_Y1Axis->SetFont(graphFont);
      m_Graph->AddLayer(m_Y1Axis.get());
      m_Graph->Fit();
    }

    m_DistanceOrStressOrForce = DistanceOrStressOrForce::Stress;
  }else{
    m_InitializeMinForceStaticText->SetLabelText("Min. force [N]:");
    m_InitializeMaxForceStaticText->SetLabelText("Max. force [N]:");
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal force [N]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal force [N]:");
    m_LengthsForceStressSensitivityStaticText->SetLabelText("Force [N]");
    m_LengthsSetForceZeroButton->SetLabelText("Zero force");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [N]:");
    m_PreloadLimitStaticText->SetLabelText("Force Limit [N]");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [N]:");
    m_OneStepStressForceLimitStaticText->SetLabelText("Upper limit [N]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText("Incrementd [dN]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("N");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Fmax.");
    m_ContinuousEndOfEventHoldRadioBtn->SetLabelText("Stop at [N]:");
    m_ForceUnit = wxT(" N");

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

    m_DistanceOrStressOrForce = DistanceOrStressOrForce::Force;
  }
}

/**
 * @brief Method wich will be executed, when the user changes the cross section area.
 * @param event Occuring event
 */
void MyFrame::OnCrossSectionAreaChange(wxSpinDoubleEvent& event){
  m_Area = m_InitializeCrossSectionSpinCtrl->GetValue();
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->setCrossSectionArea(m_Area);
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
 * @brief Method wich will be executed when the user clicks on load stored positions.
 * @param event Occuring event
 */
void MyFrame::OnLoadStoredPositions(wxCommandEvent& event){
  loadStoredPositions();
}

/**
 * @brief Method wich will be executed at start up when the set up didn't change.
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
  //m_MaxDistanceLimit = 153 / 0.00009921875/*mm per micro step*/;
  /*
  m_StageFrame->setMaxDistanceLimit(153);
  m_StageFrame->setMinDistanceLimit(0);
  */

  MyHomeStages *homestages = new MyHomeStages(m_StageFrame, this, this);
  homestages->Show();
}

/**
 * @brief Method wich will be executed, when the user clicks on the set Le button.
 */
void MyFrame::setDistanceWActuatorCollision(double le){
  //m_MaxPosDistance = m_LengthsLESpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
  m_DistanceWActuatorCollisionSetFlag = true;
  // Set min distance.
  m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  m_StageFrame->setMaxDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  m_MaxPosDistance = m_StageFrame->setDistanceWActuatorCollision(le / 0.00009921875/*mm per micro step*/);
  m_LengthsGoToSpinCtrl->SetValue(m_MaxPosDistance);
}

/**
 * @brief Method wich will be executed, when the user clicks on the set length button.
 * @param event Occuring event
 */
void MyFrame::OnLengthsSetMountingLength(wxCommandEvent& event){
  m_MountingLength = m_CurrentDistance;
  m_GageLength = m_CurrentDistance;
  m_InitializeMountingLengthShowStaticText->SetLabelText(std::to_string(m_MountingLength * 0.00009921875/*mm per micro step*/));
  //m_StageFrame->setMaxDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  //m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
}

/**
 * @brief Method wich will be executed, when the user clicks on the set sensitivities.
 * @param event Occuring event
 */
void MyFrame::OnSetSensitivities(wxCommandEvent& event){
  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    m_ForceStressSensitivity = m_LengthsForceStressSensitivitySpinCtrl->GetValue() * 10000.0;
  }else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
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
  m_StageFrame->setZeroDistance();
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
 * @brief Method wich will be executed, when the user clicks on the "Go to" button in clamping position.
 * @param event Occuring event
 */
void MyFrame::OnLimitsGoTo(wxCommandEvent& event){
  m_StageFrame->gotoMMDistance(m_LengthsGoToSpinCtrl->GetValue());
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedPercentChanged(wxSpinDoubleEvent& event){
 //double speedmm = m_MountingLength * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 double speedmm = m_GageLength * 0.00009921875/*mm per micro step*/  * (m_PreloadSpeedPreloadSpinCtrl->GetValue() / 100);
 m_PreloadSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_PreloadSpeedMmSpinCtrl->GetValue() / (m_GageLength * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  m_PreloadSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_PreloadSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitGraph;
    minlimitvector = &m_MinStressForceLimitGraph;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitGraph;
    minlimitvector = &m_MinDistanceLimitGraph;
  }

  PreloadParameters parameters;
  parameters.distanceOrStressOrForce = m_DistanceOrStressOrForce;
  parameters.stressForceLimit = m_PreloadLimitSpinCtrl->GetValue();
  parameters.velocity = m_PreloadSpeedMmSpinCtrl->GetValue();

  if(true == m_BlockNotebookTabFlag){
    m_PreloadSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<Preload> preload = std::dynamic_pointer_cast<Preload>(m_CurrentProtocol->getEditExperiment());
    preload->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;

  }else{
    //Experiment* experiment = new Preload(ExperimentType::Preload,
    std::unique_ptr<Experiment> experiment(new Preload(m_StageFrame,
                                                       m_ForceSensorMessageHandler,
                                                       &m_ForceStressDistanceGraph,
                                                       &m_ForceStressDisplacementGraph,
                                                       &m_VectorLayerMutex,
                                                       maxlimitvector,
                                                       minlimitvector,
                                                       this,
                                                       m_StoragePath,
                                                       m_MaxForceLimit,
                                                       m_MinForceLimit,
                                                       m_MaxDistanceLimit,
                                                       m_MinDistanceLimit,
                                                       m_ForceStressSensitivity,
                                                       m_DistanceSensitivity,

                                                       &m_Wait,
                                                       &m_WaitMutex,
                                                       &m_StagesStoppedFlag,
                                                       &m_StagesStoppedMutex,

                                                       ExperimentType::Preload,
                                                       m_DistanceOrStressOrForce,
                                                       m_GageLength,
                                                       m_MountingLength,
                                                       m_MaxPosDistance,
                                                       m_CurrentDistance,
                                                       m_Area,

                                                       parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
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
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in the one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_OneStepSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in one step event.
 * @param event Occuring event
 */
void MyFrame::OnOneStepSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitGraph;
    minlimitvector = &m_MinStressForceLimitGraph;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitGraph;
    minlimitvector = &m_MinDistanceLimitGraph;
  }

  OneStepEventParameters parameters;
  if(true == m_OneStepStressForceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = m_DistanceOrStressOrForce;
    parameters.delay = m_OneStepStressForceDelaySpinCtrl->GetValue();
    parameters.dwell = m_OneStepStressForceDwellSpinCtrl->GetValue();
    parameters.holdLimit = m_OneStepStressForceHoldLimitCheckBox->GetValue();
    parameters.limit = m_OneStepStressForceLimitSpinCtrl->GetValue();

    if(true == m_OneStepStressForceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_OneStepStressForceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.velocity = m_OneStepStressForceVelocitySpinCtrl->GetValue();
  }else if(true == m_OneStepDistanceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = DistanceOrStressOrForce::Distance;
    parameters.delay = m_OneStepDistanceDelaySpinCtrl->GetValue();

    if(true == m_OneStepDistanceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_OneStepDistanceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.velocity = m_OneStepDistanceVelocitySpinCtrl->GetValue();

    if(true == m_OneStepDistanceLimitMmRelRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
    }else if(true == m_OneStepDistanceLimitMmRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::Distance;
    }else if(true == m_OneStepDistanceLimitPercentRadioBtn->GetValue()){
      parameters.limitDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }
    parameters.limit = m_OneStepDistanceLimitSpinCtrl->GetValue();
    parameters.dwell = m_OneStepDistanceDwellSpinCtrl->GetValue();
  }
  wxLogMessage(std::string("MyFrame: limit: " + std::to_string(parameters.limit)).c_str());

  if(true == m_OneStepEndOfEventHoldMmRelRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
  }else if(true == m_OneStepEndOfEventHoldMmRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::Distance;
  }else if(true == m_OneStepEndOfEventHoldPercentRadioBtn->GetValue()){
    parameters.holdDistanceOrPercentage = DistanceOrPercentage::Percentage;
  }
  parameters.holdDistance = m_OneStepEndOfEventHoldSpinCtrl->GetValue();

  if(true == m_OneStepEndOfEventRepeatCheckBox->GetValue()){
    parameters.cycles = m_OneStepEndOfEventRepeatSpinCtrl->GetValue();
  }else{
    parameters.cycles = 1;
  }

  if(true == m_OneStepEndOfEventHoldRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::HoldADistance;
  }else if(true == m_OneStepEndOfEventL0RadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToL0;
  }else if(true == m_OneStepEndOfEventStopRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::Stop;
  }else if(true == m_OneStepEndOfEventMLRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToML;
  }

  if(true == m_BlockNotebookTabFlag){
    m_OneStepSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<OneStepEvent> onestepevent = std::dynamic_pointer_cast<OneStepEvent>(m_CurrentProtocol->getEditExperiment());
    onestepevent->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;

  }else{
    std::unique_ptr<Experiment> experiment(new OneStepEvent(m_StageFrame,
                                                            m_ForceSensorMessageHandler,
                                                            &m_ForceStressDistanceGraph,
                                                            &m_ForceStressDisplacementGraph,
                                                            &m_VectorLayerMutex,
                                                            maxlimitvector,
                                                            minlimitvector,
                                                            this,
                                                            m_StoragePath,
                                                            m_MaxForceLimit,
                                                            m_MinForceLimit,
                                                            m_MaxDistanceLimit,
                                                            m_MinDistanceLimit,
                                                            m_ForceStressSensitivity,
                                                            m_DistanceSensitivity,

                                                            &m_Wait,
                                                            &m_WaitMutex,
                                                            &m_StagesStoppedFlag,
                                                            &m_StagesStoppedMutex,

                                                            ExperimentType::OneStepEvent,
                                                            parameters.distanceOrStressOrForce,
                                                            m_GageLength,
                                                            m_MountingLength,
                                                            m_MaxPosDistance,
                                                            m_CurrentDistance,
                                                            m_Area,

                                                            parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
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
 * @brief Method wich will be executed, when the user chooses max value in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousMaxValue(wxCommandEvent& event){
  m_ContinuousStressForceStepsStaticText->Show(false);
  m_ContinuousStressForceStepsSpinCtrl->Show(false);
  m_ContinuousDistanceStepsStaticText->Show(false);
  m_ContinuousDistanceStepsSpinCtrl->Show(false);

  m_ContinuousStressForceMaxValueStaticText->Show(true);
  m_ContinuousStressForceMaxValueSpinCtrl->Show(true);
  m_ContinuousStressForceMaxValueValueRadioBtn->Show(true);
  m_ContinuousStressForceMaxValuePercentRadioBtn->Show(true);
  m_ContinuousDistanceMaxValueStaticText->Show(true);
  m_ContinuousDistanceMaxValueSpinCtrl->Show(true);
  m_ContinuousDistancePanel23->Show(true);

  m_ContinuousStressForcePanel->Layout();
  m_ContinuousDistancePanel->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses steps in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousSteps(wxCommandEvent& event){
  m_ContinuousStressForceMaxValueStaticText->Show(false);
  m_ContinuousStressForceMaxValueSpinCtrl->Show(false);
  m_ContinuousStressForceMaxValueValueRadioBtn->Show(false);
  m_ContinuousStressForceMaxValuePercentRadioBtn->Show(false);
  m_ContinuousDistanceMaxValueStaticText->Show(false);
  m_ContinuousDistanceMaxValueSpinCtrl->Show(false);
  m_ContinuousDistancePanel23->Show(false);

  m_ContinuousStressForceStepsStaticText->Show(true);
  m_ContinuousStressForceStepsSpinCtrl->Show(true);
  m_ContinuousDistanceStepsStaticText->Show(true);
  m_ContinuousDistanceStepsSpinCtrl->Show(true);

  m_ContinuousStressForcePanel->Layout();
  m_ContinuousDistancePanel->Layout();
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Cancel" button in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousCancel(wxCommandEvent& event){
  if(true == m_BlockNotebookTabFlag){
    m_ContinuousSendButton->SetLabelText("Send to protocol");
    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousSendToProtocol(wxCommandEvent& event){

  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitGraph;
    minlimitvector = &m_MinStressForceLimitGraph;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitGraph;
    minlimitvector = &m_MinDistanceLimitGraph;
  }

  ContinuousEventParameters parameters;
  parameters.ramp2failure = false;
  if(true == m_ContinuousStressForceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = m_DistanceOrStressOrForce;
    parameters.holdtime = m_ContinuousStressForceHoldTimeSpinCtrl->GetValue();
    parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Distance;
    parameters.increment = m_ContinuousStressForceIncrementSpinCtrl->GetValue();
    parameters.velocity = m_ContinuousStressForceVelocitySpinCtrl->GetValue();
    parameters.maxvalue = m_ContinuousStressForceMaxValueSpinCtrl->GetValue();

    if(true == m_ContinuousStressForceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousStressForceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousStressForceMaxValueRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousStressForceMaxValuePercentRadioBtn->GetValue()){
        parameters.ramp2failure = true;
      }else if(true == m_ContinuousStressForceMaxValueValueRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Distance;
      }
    } else if(true == m_ContinuousStressForceStepsRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::Steps;
      parameters.steps = m_ContinuousStressForceStepsSpinCtrl->GetValue();
    }

  }else if(true == m_ContinuousDistanceRadioBtn->GetValue()){
    parameters.distanceOrStressOrForce = DistanceOrStressOrForce::Distance;
    parameters.velocity = m_ContinuousDistanceVelocitySpinCtrl->GetValue();
    parameters.holdtime = m_ContinuousDistanceHoldTimeSpinCtrl->GetValue();
    parameters.increment = m_ContinuousDistanceIncrementSpinCtrl->GetValue();
    parameters.maxvalue = m_ContinuousDistanceMaxValueSpinCtrl->GetValue();

    if(true == m_ContinuousDistanceVelocityMmRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousDistanceVelocityPercentRadioBtn->GetValue()){
      parameters.velocityDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousDistanceIncrementMmRadioBtn->GetValue()){
      parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousDistanceIncrementPercentRadioBtn->GetValue()){
      parameters.incrementDistanceOrPercentage = DistanceOrPercentage::Percentage;
    }

    if(true == m_ContinuousDistanceMaxValueRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousDistanceMaxValueMmRelRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::DistanceRelative;
      }else if(true == m_ContinuousDistanceMaxValueMmRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Distance;
      }else if(true == m_ContinuousDistanceMaxValuePercentRadioBtn->GetValue()){
        parameters.maxValueDistanceOrPercentage = DistanceOrPercentage::Percentage;
      }
    } else if(true == m_ContinuousDistanceStepsRadioBtn->GetValue()){
      parameters.stepsOrMaxValue = StepsOrMaxValue::Steps;
      parameters.steps = m_ContinuousDistanceStepsSpinCtrl->GetValue();
    }
  }

  if(true == m_ContinuousEndOfEventRepeatCheckBox->GetValue()){
    parameters.cycles = m_ContinuousEndOfEventRepeatSpinCtrl->GetValue();
  }else{
    parameters.cycles = 1;
  }

  if(true == m_ContinuousEndOfEventStopRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::Stop;
  }else if(true == m_ContinuousEndOfEventL0RadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToL0;
  }else if(true == m_ContinuousEndOfEventMLRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::GoToML;
  }else if(true == m_ContinuousEndOfEventHoldRadioBtn->GetValue()){
    parameters.behaviorAfterStop = BehaviorAfterStop::HoldAForce;
  }

  parameters.holdForceStress = m_ContinuousEndOfEventHoldSpinCtrl->GetValue() * m_Area * 10.0;

  if(true == m_BlockNotebookTabFlag){
    m_ContinuousSendButton->SetLabelText("Send to protocol");

    std::shared_ptr<ContinuousEvent> continuousevent = std::dynamic_pointer_cast<ContinuousEvent>(m_CurrentProtocol->getEditExperiment());
    continuousevent->setParameters(parameters);
    m_CurrentProtocol->updateEditedExperimentParameters();

    // Unblock tab.
    m_BlockNotebookTabFlag = false;
  }else{
    std::unique_ptr<Experiment> experiment(new ContinuousEvent(m_StageFrame,
                                                               m_ForceSensorMessageHandler,
                                                               &m_ForceStressDistanceGraph,
                                                               &m_ForceStressDisplacementGraph,
                                                               &m_VectorLayerMutex,
                                                               maxlimitvector,
                                                               minlimitvector,
                                                               this,
                                                               m_StoragePath,
                                                               m_MaxForceLimit,
                                                               m_MinForceLimit,
                                                               m_MaxDistanceLimit,
                                                               m_MinDistanceLimit,
                                                               m_ForceStressSensitivity,
                                                               m_DistanceSensitivity,

                                                               &m_Wait,
                                                               &m_WaitMutex,
                                                               &m_StagesStoppedFlag,
                                                               &m_StagesStoppedMutex,

                                                               ExperimentType::ContinuousEvent,
                                                               parameters.distanceOrStressOrForce,
                                                               parameters.ramp2failure,
                                                               m_GageLength,
                                                               m_MountingLength,
                                                               m_MaxPosDistance,
                                                               m_CurrentDistance,
                                                               m_Area,

                                                               parameters));

    m_CurrentProtocol->addExperiment(experiment);
  }
}

/**
 * @brief Method wich will be executed, when the user sets the limits.
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

  m_StageFrame->setMaxDistanceLimit(m_MaxDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_StageFrame->setMinDistanceLimit(m_MinDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_MaxDistanceLimit /= 0.00009921875/*mm per micro step*/;
  m_MinDistanceLimit /= 0.00009921875/*mm per micro step*/;

  checkProtocol();
  m_CurrentProtocol->setLimits(m_MinDistanceLimit, m_MaxDistanceLimit, m_MinForceLimit, m_MaxForceLimit);
}

/**
 * @brief Method wich will be executed, when the decrease stage distance button is pushed down.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistanceStart(wxCommandEvent& event){
  //std::cout << "MyFrame event Id: " << event.GetId() << std::endl;
  if(false == m_DisableDecreaseDistanceFlag){
    m_StageFrame->moveForward(1/*mm/s*/);
  }else{
    wxLogMessage("MyFrame: Decrease distance disabled");
  }
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
  if(false == m_DisableIncreaseDistanceFlag){
    m_StageFrame->moveBackward(1/*mm/s*/);
  }else{
    wxLogMessage("MyFrame: Increase distance disabled");
  }
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
  if(0 != m_LinearStages.size()){
    (m_LinearStages.at(0))->stop();
    (m_LinearStages.at(1))->stop();
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
 * @brief Method wich will be executed, when the user clicks on the export csv button. Checks if there is any data to export. Opens the export dialog if there is some data.
 * @param event Occuring event
 */
void MyFrame::OnExportCSV(wxCommandEvent& event){
  checkProtocol();
  // Checks if there is any data to export.
  if(true == m_CurrentProtocol->hasData()){
    std::unique_ptr<MyExportDialog> dialog = std::unique_ptr<MyExportDialog>(new MyExportDialog(m_CurrentProtocol, m_CurrentProtocol->getExperimentNames(), m_StoragePath));
    dialog->ShowModal();
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the export png button.
 * @param event Occuring event
 */
void MyFrame::OnExportPNG(wxCommandEvent& event){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));

  std::string pathAndFilename = m_StoragePath + "/" + "Graph_" + std::string(mbstr) + ".png";

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save png file"), "", "", "PNG files (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  saveFileDialog.SetPath(pathAndFilename);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }

  m_Graph->SaveScreenshot(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
  wxLogMessage(std::string("Graph saved in: " + saveFileDialog.GetPath()).c_str());
}

/**
 * @brief Method wich will be executed, when the user clicks on the clear log button.
 * @param event Occuring event
 */
void MyFrame::OnClearLog(wxCommandEvent& event){
  m_LogTextCtrl->Clear();
}

/**
 * @brief Method wich will be executed, when the user clicks on the save log button.
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
    m_ForceStressDistanceGraph.Clear();
    m_ForceStressDisplacementGraph.Clear();
  }
  wxLogMessage("MyFrame cleared graph.");
  m_Graph->Fit();
  //delete m_CurrentExperimentValues;
}

/**
 * @brief Method wich will be executed, when the user clicks on the delete experiment button.
 * @param event Occuring event
 */
void MyFrame::OnDeleteExperiment(wxCommandEvent& event){
  if(wxNOT_FOUND != m_ProtocolsListBox->GetSelection()){
    m_CurrentProtocol->removeExperiment(m_ProtocolsListBox->GetSelection());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment up button.
 * @param event Occuring event
 */
void MyFrame::OnMoveUpExperiment(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->moveExperimentUp(m_ProtocolsListBox->GetSelection());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the move experiment down button.
 * @param event Occuring event
 */
void MyFrame::OnMoveDownExperiment(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->moveExperimentDown(m_ProtocolsListBox->GetSelection());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the pause experiment down button.
 * @param event Occuring event
 */
void MyFrame::OnPauseExperiment(wxCommandEvent& event){
  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitGraph;
    minlimitvector = &m_MinStressForceLimitGraph;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitGraph;
    minlimitvector = &m_MinDistanceLimitGraph;
  }

  std::unique_ptr<Experiment> experiment(new Pause(m_StageFrame,
                                                   m_ForceSensorMessageHandler,
                                                   &m_ForceStressDistanceGraph,
                                                   &m_ForceStressDisplacementGraph,
                                                   &m_VectorLayerMutex,
                                                   maxlimitvector,
                                                   minlimitvector,
                                                   this,
                                                   m_StoragePath,
                                                   m_MaxForceLimit,
                                                   m_MinForceLimit,
                                                   m_MaxDistanceLimit,
                                                   m_MinDistanceLimit,

                                                   &m_Wait,
                                                   &m_WaitMutex,

                                                   ExperimentType::Pause,
                                                   DistanceOrStressOrForce::Distance,
                                                   m_GageLength,
                                                   m_MountingLength,
                                                   m_MaxPosDistance,
                                                   m_CurrentDistance,
                                                   m_Area));

  Pause *ptr = dynamic_cast<Pause*>(experiment.get());

  std::unique_ptr<MyPauseDialog> dialog = std::unique_ptr<MyPauseDialog>(new MyPauseDialog(ptr));
  dialog->ShowModal();

  if(true == dialog->getCreateExperimentFlag()){
    m_CurrentProtocol->addExperiment(experiment);
  }
  //dialog->Close();
}

/**
 * @brief Method wich will be executed, when the user clicks on the pause experiment down button.
 * @param event Occuring event
 */
void MyFrame::OnPauseResumeExperiment(wxCommandEvent& event){
  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitGraph;
    minlimitvector = &m_MinStressForceLimitGraph;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitGraph;
    minlimitvector = &m_MinDistanceLimitGraph;
  }

  std::unique_ptr<Experiment> experiment(new PauseResume(m_StageFrame,
                                                         m_ForceSensorMessageHandler,
                                                         &m_ForceStressDistanceGraph,
                                                         &m_ForceStressDisplacementGraph,
                                                         &m_VectorLayerMutex,
                                                         maxlimitvector,
                                                         minlimitvector,
                                                         this,
                                                         m_StoragePath,
                                                         m_MaxForceLimit,
                                                         m_MinForceLimit,
                                                         m_MaxDistanceLimit,
                                                         m_MinDistanceLimit,

                                                         &m_Wait,
                                                         &m_WaitMutex,

                                                         ExperimentType::PauseResume,
                                                         DistanceOrStressOrForce::Distance,
                                                         m_GageLength,
                                                         m_MountingLength,
                                                         m_MaxPosDistance,
                                                         m_CurrentDistance,
                                                         m_Area));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Shows pause/resume dialog.
 */
void MyFrame::showPauseResumeDialogFromPauseResume(std::condition_variable *wait, std::mutex *mutex){
  CallAfter(&MyFrame::showPauseResumeDialog, wait, mutex);
}

/**
 * @brief Opens a pop-up warning dialog.
 */
bool MyFrame::showHighVelocityWarningFromExperiments(void){
  CallAfter(&MyFrame::showHighVelocityWarning);

  // Wait until dialog is closed.
  std::unique_lock<std::mutex> lck(m_WaitHighVelocityMutex);
  m_WaitHighVelocity.wait(lck);
  return(m_HighVelocityAbort);
}

/**
 * @brief Shows the export dialog.
 */
void MyFrame::showExportCSVDialogFromProtocols(void){
  CallAfter(&MyFrame::showExportCSVDialog);
}

/**
 * @brief Shows pause/resume dialog.
 */
void MyFrame::showPauseResumeDialog(std::condition_variable *wait, std::mutex *mutex){
  std::unique_ptr<wxMessageDialog> popup = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this, "Push the button OK to resume the protocol."));
  popup->ShowModal();
  std::lock_guard<std::mutex> lck(*mutex);
  wait->notify_all();
}

/**
 * @brief Method wich will be executed, when the user makes a right click in the list box.
 * @param event Occuring event
 */
void MyFrame::OnProtocolsListRightClick(wxMouseEvent& event){
  // Creat context menu with an edit entry
  std::unique_ptr<wxMenu> popup = std::unique_ptr<wxMenu>(new wxMenu);
  popup->Append(wxID_OPEN, wxT("Edit"));

  // Bind the context menu events, pop up the dialog and unbind.
  Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnEditExperiment, this);
  PopupMenu(popup.get());
  Unbind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::OnEditExperiment, this);
}

/**
 * @brief Method wich will be executed, when the user clicks on edit in the contect menu.
 * @param event Occuring event
 */
void MyFrame::OnEditExperiment(wxCommandEvent& event){

  switch(m_CurrentProtocol->getEditExperimentType()){
    case ExperimentType::Preload:
      {
        // Change to preload tab.
        m_Experiments->SetSelection(3);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get preload parameters.
        std::shared_ptr<Preload> preload = std::dynamic_pointer_cast<Preload>(m_CurrentProtocol->getEditExperiment());
        PreloadParameters parameters = preload->getParametersForGUI();

        m_PreloadLimitSpinCtrl->SetValue(parameters.stressForceLimit);
        m_PreloadSpeedMmSpinCtrl->SetValue(parameters.velocity);
        m_PreloadSpeedPreloadSpinCtrl->SetValue(parameters.velocity / (m_GageLength * 0.00009921875/*mm per micro step*/) * 100/*%*/);

        m_PreloadSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::OneStepEvent:
      {
        // Change to the one step event tab.
        m_Experiments->SetSelection(4);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get one step event parameters.
        std::shared_ptr<OneStepEvent> onestepevent = std::dynamic_pointer_cast<OneStepEvent>(m_CurrentProtocol->getEditExperiment());
        OneStepEventParameters parameters = onestepevent->getParametersForGUI();

        m_OneStepDistanceVelocitySpinCtrl->SetValue(parameters.velocity);
        m_OneStepStressForceVelocitySpinCtrl->SetValue(parameters.velocity);

        if(DistanceOrPercentage::Distance == parameters.velocityDistanceOrPercentage){
          m_OneStepStressForceVelocityMmRadioBtn->SetValue(true);
          m_OneStepStressForceVelocityPercentRadioBtn->SetValue(false);

          m_OneStepDistanceVelocityMmRadioBtn->SetValue(true);
          m_OneStepDistanceVelocityPercentRadioBtn->SetValue(false);
        }else if(DistanceOrPercentage::Percentage == parameters.velocityDistanceOrPercentage){
          m_OneStepStressForceVelocityMmRadioBtn->SetValue(false);
          m_OneStepStressForceVelocityPercentRadioBtn->SetValue(true);

          m_OneStepDistanceVelocityMmRadioBtn->SetValue(false);
          m_OneStepDistanceVelocityPercentRadioBtn->SetValue(true);
        }

        m_OneStepDistanceDelaySpinCtrl->SetValue(parameters.delay);
        m_OneStepStressForceDelaySpinCtrl->SetValue(parameters.delay);

        m_OneStepDistanceDwellSpinCtrl->SetValue(parameters.dwell);
        m_OneStepStressForceDwellSpinCtrl->SetValue(parameters.dwell);

        if(DistanceOrStressOrForce::Distance == parameters.distanceOrStressOrForce){
          m_OneStepStressForcePanel->Show(false);
          m_OneStepDistancePanel->Show(true);
          m_OneStepPanel21->Layout();

          m_OneStepStressForceRadioBtn->SetValue(false);
          m_OneStepDistanceRadioBtn->SetValue(true);

          m_OneStepDistanceLimitSpinCtrl->SetValue(parameters.limit);
          if(DistanceOrPercentage::DistanceRelative == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRadioBtn->SetValue(false);
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Distance == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitMmRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(false);
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Percentage == parameters.limitDistanceOrPercentage){
            m_OneStepDistanceLimitPercentRadioBtn->SetValue(true);
            m_OneStepDistanceLimitMmRadioBtn->SetValue(false);
            m_OneStepDistanceLimitMmRelRadioBtn->SetValue(false);
          }

        }else{
          m_OneStepDistancePanel->Show(false);
          m_OneStepStressForcePanel->Show(true);
          m_OneStepPanel21->Layout();

          m_OneStepDistanceRadioBtn->SetValue(false);
          m_OneStepStressForceRadioBtn->SetValue(true);

          m_OneStepStressForceLimitSpinCtrl->SetValue(parameters.limit);
        }

        if(1 < parameters.cycles){
          m_OneStepEndOfEventRepeatCheckBox->SetValue(true);
          m_OneStepEndOfEventRepeatSpinCtrl->SetValue(parameters.cycles);
        }

        switch(parameters.behaviorAfterStop){
          case BehaviorAfterStop::Stop:
            m_OneStepEndOfEventStopRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::HoldADistance:
            m_OneStepEndOfEventHoldRadioBtn->SetValue(true);
            m_OneStepEndOfEventHoldMmRadioBtn->SetValue(true);
            m_OneStepEndOfEventHoldSpinCtrl->SetValue(parameters.holdDistance);
            break;
          case BehaviorAfterStop::GoToL0:
            m_OneStepEndOfEventL0RadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToML:
            m_OneStepEndOfEventMLRadioBtn->SetValue(true);
            break;
        }

        m_OneStepSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::ContinuousEvent:
      {
        // Change to the continuous event tab.
        m_Experiments->SetSelection(5);
        // Block tab.
        m_BlockNotebookTabFlag = true;

        // Get continuous event parameters.
        std::shared_ptr<ContinuousEvent> continuousevent = std::dynamic_pointer_cast<ContinuousEvent>(m_CurrentProtocol->getEditExperiment());
        ContinuousEventParameters parameters = continuousevent->getParametersForGUI();

        m_ContinuousDistanceVelocitySpinCtrl->SetValue(parameters.velocity);
        m_ContinuousStressForceVelocitySpinCtrl->SetValue(parameters.velocity);

        m_ContinuousDistanceHoldTimeSpinCtrl->SetValue(parameters.holdtime);
        m_ContinuousStressForceHoldTimeSpinCtrl->SetValue(parameters.holdtime);

        if(DistanceOrPercentage::Distance == parameters.velocityDistanceOrPercentage){
          m_ContinuousDistanceVelocityMmRadioBtn->SetValue(true);
          m_ContinuousDistanceVelocityPercentRadioBtn->SetValue(false);

          m_ContinuousStressForceVelocityMmRadioBtn->SetValue(true);
          m_ContinuousStressForceVelocityPercentRadioBtn->SetValue(false);
        }else if(DistanceOrPercentage::Percentage == parameters.velocityDistanceOrPercentage){
          m_ContinuousDistanceVelocityMmRadioBtn->SetValue(false);
          m_ContinuousDistanceVelocityPercentRadioBtn->SetValue(true);

          m_ContinuousStressForceVelocityMmRadioBtn->SetValue(false);
          m_ContinuousStressForceVelocityPercentRadioBtn->SetValue(true);
        }

        if(DistanceOrStressOrForce::Distance == parameters.distanceOrStressOrForce){
          m_ContinuousStressForcePanel->Show(false);
          m_ContinuousDistancePanel->Show(true);
          m_ContinuousPanel21->Layout();

          m_ContinuousStressForceRadioBtn->SetValue(false);
          m_ContinuousDistanceRadioBtn->SetValue(true);

          m_ContinuousDistanceIncrementSpinCtrl->SetValue(parameters.increment);
          if(DistanceOrPercentage::Distance == parameters.incrementDistanceOrPercentage){
            m_ContinuousDistanceIncrementMmRadioBtn->SetValue(true);
            m_ContinuousDistanceIncrementPercentRadioBtn->SetValue(false);
          }else if(DistanceOrPercentage::Percentage == parameters.incrementDistanceOrPercentage){
            m_ContinuousDistanceIncrementPercentRadioBtn->SetValue(true);
            m_ContinuousDistanceIncrementMmRadioBtn->SetValue(false);
          }

          if(StepsOrMaxValue::MaxValue == parameters.stepsOrMaxValue){
            m_ContinuousDistanceStepsRadioBtn->SetValue(false);
            m_ContinuousDistanceMaxValueRadioBtn->SetValue(true);

            m_ContinuousDistanceMaxValueSpinCtrl->SetValue(parameters.maxvalue);
            if(DistanceOrPercentage::Distance == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(false);
            }else if(DistanceOrPercentage::DistanceRelative == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(false);
            }else if(DistanceOrPercentage::Percentage == parameters.maxValueDistanceOrPercentage){
              m_ContinuousDistanceMaxValuePercentRadioBtn->SetValue(true);
              m_ContinuousDistanceMaxValueMmRelRadioBtn->SetValue(false);
              m_ContinuousDistanceMaxValueMmRadioBtn->SetValue(false);
            }
          }else{
            m_ContinuousDistanceMaxValueRadioBtn->SetValue(false);
            m_ContinuousDistanceStepsRadioBtn->SetValue(true);

            m_ContinuousDistanceStepsSpinCtrl->SetValue(parameters.steps);
          }

        }else{
          m_ContinuousDistancePanel->Show(false);
          m_ContinuousStressForcePanel->Show(true);
          m_ContinuousPanel21->Layout();

          m_ContinuousDistanceRadioBtn->SetValue(false);
          m_ContinuousStressForceRadioBtn->SetValue(true);

          m_ContinuousStressForceIncrementSpinCtrl->SetValue(parameters.increment);

          if(StepsOrMaxValue::MaxValue == parameters.stepsOrMaxValue){
            m_ContinuousStressForceStepsRadioBtn->SetValue(false);
            m_ContinuousStressForceMaxValueRadioBtn->SetValue(true);

            m_ContinuousStressForceMaxValueSpinCtrl->SetValue(parameters.maxvalue);
            if(true == parameters.ramp2failure){
              m_ContinuousStressForceMaxValueValueRadioBtn->SetValue(false);
              m_ContinuousStressForceMaxValuePercentRadioBtn->SetValue(true);
            }else{
              m_ContinuousStressForceMaxValuePercentRadioBtn->SetValue(false);
              m_ContinuousStressForceMaxValueValueRadioBtn->SetValue(true);
            }
          }else{
            m_ContinuousStressForceMaxValueRadioBtn->SetValue(false);
            m_ContinuousStressForceStepsRadioBtn->SetValue(true);

            m_ContinuousStressForceStepsSpinCtrl->SetValue(parameters.steps);
          }

        }

        if(1 < parameters.cycles){
          m_ContinuousEndOfEventRepeatCheckBox->SetValue(true);
          m_ContinuousEndOfEventRepeatSpinCtrl->SetValue(parameters.cycles);
        }

        switch(parameters.behaviorAfterStop){
          case BehaviorAfterStop::Stop:
            m_ContinuousEndOfEventStopRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToL0:
            m_ContinuousEndOfEventL0RadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::GoToML:
            m_ContinuousEndOfEventMLRadioBtn->SetValue(true);
            break;
          case BehaviorAfterStop::HoldAForce:
            m_ContinuousEndOfEventHoldRadioBtn->SetValue(true);
            m_ContinuousEndOfEventHoldSpinCtrl->SetValue(parameters.holdForceStress);
            break;
        }
        m_ContinuousSendButton->SetLabelText("Save changes");
      }
      break;

    case ExperimentType::Pause:
      // Get pause parameters
      std::shared_ptr<Pause> pause = std::dynamic_pointer_cast<Pause>(m_CurrentProtocol->getEditExperiment());
      double pausetime = pause->getParametersForGUI();

      std::unique_ptr<MyPauseDialog> dialog = std::unique_ptr<MyPauseDialog>(new MyPauseDialog(std::dynamic_pointer_cast<Pause>(m_CurrentProtocol->getEditExperiment()).get(),
                                                                                               pausetime));
      dialog->ShowModal();
      m_CurrentProtocol->updateEditedExperimentParameters();

      /*
      if(true == dialog->getCreateExperimentFlag()){
        //m_CurrentProtocol->addExperiment(experiment);
      }
      */
      break;
  }
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
 * @brief A message dialog asks the user if he/she wants to save the recorded data. If yes, the export dialog will show up.
 */
void MyFrame::showExportCSVDialog(void){
  // Ask if recorded data should be saved.
  std::unique_ptr<wxMessageDialog> dialog = std::unique_ptr<wxMessageDialog>(new wxMessageDialog(this, "The protocol is finished, do you want to save the recorded values?", wxMessageBoxCaptionStr, wxOK|wxCANCEL));
  // Show the export dialog if yes.
  if(wxID_OK == dialog->ShowModal()){
    std::unique_ptr<MyExportDialog> dialog = std::unique_ptr<MyExportDialog>(new MyExportDialog(m_CurrentProtocol, m_CurrentProtocol->getExperimentNames(), m_StoragePath));
    dialog->ShowModal();
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the preview protocol button.
 * @param event Occuring event
 */
void MyFrame::OnPreviewProtocol(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->makePreview();
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the run protocol button.
 * @param event Occuring event
 */
void MyFrame::OnRunProtocol(wxCommandEvent& event){
  /*
  wxMessageDialog *popup = new wxMessageDialog(this, "Limit will exeed, check your experiment settings.");
  popup->Show();
  popup->ShowModal();
  delete popup;
  */
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->runProtocol();
  }
}

/**
 * @brief Method wich will be executed, when the user activates Loop in protocol.
 * @param event Occuring event
 */
void MyFrame::OnLoopProtocol(wxCommandEvent& event){
  if(nullptr != m_CurrentProtocol){
    m_CurrentProtocol->setLoopFlag(m_ProtocolsLoopCheckBox->GetValue());
  }
}

/**
 * @brief Method wich will be executed, when the user clicks on the stop protocol button.
 * @param event Occuring event
 */
void MyFrame::OnStopProtocol(wxCommandEvent& event){
  //OnMotorStop(event);
  m_StageFrame->stop();
  m_CurrentProtocol->stopProtocol();
}

/**
 * @brief Method wich will be executed, when the user clicks on the save protocol button.
 * @param event Occuring event
 */
void MyFrame::OnSaveProtocol(wxCommandEvent& event){
  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));

  std::string pathAndFilename = m_StoragePath + "/" + "Protocol_" + std::string(mbstr) + ".xml";

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save protocol file"), "", "", "Protocol files (*.xml)|*.xml", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  saveFileDialog.SetPath(pathAndFilename);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }

  m_CurrentProtocol->saveProtocol(saveFileDialog.GetPath().ToStdString());
  wxLogMessage(std::string("Protocol saved in: " + saveFileDialog.GetPath()).c_str());
}

/**
 * @brief Method wich will be executed, when the user clicks on the load protocol button.
 * @param event Occuring event
 */
void MyFrame::OnLoadProtocol(wxCommandEvent& event){
  checkProtocol();

  // Let user choose path and file name.
  wxFileDialog saveFileDialog(this, _("Save protocol file"), "", "", "Protocol files (*.xml)|*.xml", wxFD_OPEN);
  saveFileDialog.SetPath(m_StoragePath);

  if(wxID_CANCEL == saveFileDialog.ShowModal()){
    return;
  }
  m_CurrentProtocol->loadProtocol(saveFileDialog.GetPath().ToStdString(), m_GageLength, m_MountingLength, m_MaxPosDistance, m_CurrentDistance);
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
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    tmp << (static_cast<double>(((m_CurrentForce) / 10000.0) / m_Area) * 1000) << m_ForceUnit;
  } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
    tmp << (static_cast<double>(m_CurrentForce) / 10000.0) << m_ForceUnit;
  }
  m_ForceStaticText->SetLabel(tmp);
}

/**
 * @brief Prepares the graph to show the experiment values.
 */
void MyFrame::createValuesGraph(void){
  // Indicate, that graph is active.
  m_ShowGraphFlag = true;

  // Remove layers
  m_Graph->DelLayer(m_XAxis.get());
  m_Graph->DelLayer(m_Y1Axis.get());
  m_Graph->DelLayer(m_Y2Axis.get());
  m_Graph->DelLayer(&m_ForceStressDistanceGraph);
  m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
  m_Graph->DelLayer(&m_StressForcePreviewGraph);
  m_Graph->DelLayer(&m_DistancePreviewGraph);
  m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
  m_Graph->DelLayer(&m_MinStressForceLimitGraph);
  m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
  m_Graph->DelLayer(&m_MinDistanceLimitGraph);

  // Clear vectors
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_ForceStressDistanceGraph.Clear();
    m_ForceStressDisplacementGraph.Clear();
  }

  // Defines pen styles and font.
  wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
  wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);
  wxPen vectorpenLimit(*wxRED, 2, wxSOLID);
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

  // Set up and add axis.
  m_XAxis.reset(new mpScaleX(wxT("Distance/Displacement [mm]"), mpALIGN_BOTTOM, true, mpX_NORMAL));

  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    m_Y1Axis.reset(new mpScaleY(wxT("Force [N]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
  } else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    m_Y1Axis.reset(new mpScaleY(wxT("Stress [kPa]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
  }
  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis.get());
  m_Graph->AddLayer(m_Y1Axis.get());

  // Add vectors
  m_Graph->AddLayer(&m_ForceStressDistanceGraph);
  m_Graph->AddLayer(&m_ForceStressDisplacementGraph);
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    m_MaxStressForceLimitGraph.SetPen(vectorpenLimit);
    m_MinStressForceLimitGraph.SetPen(vectorpenLimit);
    m_Graph->AddLayer(&m_MaxStressForceLimitGraph);
    m_Graph->AddLayer(&m_MinStressForceLimitGraph);
    m_ForceStressDistanceGraph.SetPen(vectorpenStressForce);
    m_ForceStressDisplacementGraph.SetPen(vectorpenStressForce);
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    m_MaxDistanceLimitGraph.SetPen(vectorpenLimit);
    m_MinDistanceLimitGraph.SetPen(vectorpenLimit);
    m_Graph->AddLayer(&m_MaxDistanceLimitGraph);
    m_Graph->AddLayer(&m_MinDistanceLimitGraph);
    m_ForceStressDistanceGraph.SetPen(vectorpenDistance);
    m_ForceStressDisplacementGraph.SetPen(vectorpenDistance);
  }

  // Update graph.
  m_Graph->Fit();
}

/**
 * @brief Method which will be called from the class ExperimentValues to update the graph. Executes updateGraph() from the main thread.
 */
void MyFrame::updateGraphFromExperimentValues(){
  std::lock_guard<std::mutex> lck{m_UpdateGraphMutex};
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
  // Indicate, that graph is active.
  m_ShowGraphFlag = true;

  // Remove layers
  m_Graph->DelLayer(m_XAxis.get());
  m_Graph->DelLayer(m_Y1Axis.get());
  m_Graph->DelLayer(m_Y2Axis.get());
  m_Graph->DelLayer(&m_ForceStressDistanceGraph);
  m_Graph->DelLayer(&m_ForceStressDisplacementGraph);
  m_Graph->DelLayer(&m_StressForcePreviewGraph);
  m_Graph->DelLayer(&m_DistancePreviewGraph);
  m_Graph->DelLayer(&m_MaxStressForceLimitGraph);
  m_Graph->DelLayer(&m_MinStressForceLimitGraph);
  m_Graph->DelLayer(&m_MaxDistanceLimitGraph);
  m_Graph->DelLayer(&m_MinDistanceLimitGraph);

  // Defines pen styles and font.
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
  wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);

  // Set up and add axis.
  m_XAxis.reset(new mpScaleX(wxT("Time"), mpALIGN_BOTTOM, true, mpX_NORMAL));
  if(DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce){
    m_Y1Axis.reset(new mpScaleY(wxT("Force [N]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
  } else if(DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce){
    m_Y1Axis.reset(new mpScaleY(wxT("Stress [kPa]"), mpALIGN_BORDER_LEFT, true));
    m_Y1Axis->SetPen(vectorpenStressForce);
  }
  m_Y2Axis.reset(new mpScaleY(wxT("Distance [mm]"), mpALIGN_BORDER_RIGHT, true));
  m_Y2Axis->SetPen(vectorpenDistance);
  m_XAxis->SetFont(graphFont);
  m_Y1Axis->SetFont(graphFont);
  m_XAxis->SetDrawOutsideMargins(false);
  m_Y1Axis->SetDrawOutsideMargins(false);

  // Set up and add vectors.
  m_MaxStressForceLimitGraph.SetPen(vectorpenStressForce);
  m_MinStressForceLimitGraph.SetPen(vectorpenStressForce);
  m_MaxDistanceLimitGraph.SetPen(vectorpenDistance);
  m_MinDistanceLimitGraph.SetPen(vectorpenDistance);

  m_StressForcePreviewGraph.SetPen(vectorpenStressForce);
  m_DistancePreviewGraph.SetPen(vectorpenDistance);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis.get());
  m_Graph->AddLayer(m_Y1Axis.get());
  m_Graph->AddLayer(m_Y2Axis.get());
  m_Graph->AddLayer(&m_StressForcePreviewGraph);
  m_Graph->AddLayer(&m_DistancePreviewGraph);
  m_Graph->AddLayer(&m_MaxStressForceLimitGraph);
  m_Graph->AddLayer(&m_MinStressForceLimitGraph);
  m_Graph->AddLayer(&m_MaxDistanceLimitGraph);
  m_Graph->AddLayer(&m_MinDistanceLimitGraph);

  m_Graph->Fit();
}

/**
 * @brief Checks if a protocol object is already created, otherwise creates it.
 */
void MyFrame::checkProtocol(void){
  if(nullptr == m_CurrentProtocol){
  m_CurrentProtocol = std::shared_ptr<Protocols>(new Protocols(m_ProtocolsListBox,
                                                               this,
                                                               m_StageFrame,
                                                               m_ForceSensorMessageHandler,
                                                               &m_VectorLayerMutex,
                                                               m_GageLength,
                                                               m_MountingLength,
                                                               m_MaxPosDistance,
                                                               m_CurrentDistance,
                                                               &m_StagesStoppedFlag,
                                                               &m_StagesStoppedMutex,
                                                               &m_WaitMutex,
                                                               &m_Wait,
                                                               &m_PreloadDoneFlag,
                                                               &m_PreloadDoneMutex,

                                                               m_ProtocolsLoopCheckBox->GetValue(),
                                                               m_Area,
                                                               m_MaxDistanceLimit,
                                                               m_MinDistanceLimit,
                                                               m_MaxForceLimit,
                                                               m_MinForceLimit,
                                                               m_ForceStressSensitivity,
                                                               m_DistanceSensitivity,

                                                               &m_ForceStressDistanceGraph,
                                                               &m_ForceStressDisplacementGraph,
                                                               &m_StressForcePreviewGraph,
                                                               &m_DistancePreviewGraph,
                                                               &m_MaxStressForceLimitGraph,
                                                               &m_MinStressForceLimitGraph,
                                                               &m_MaxDistanceLimitGraph,
                                                               &m_MinDistanceLimitGraph,
                                                               m_StoragePath));
  }
}
