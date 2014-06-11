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
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  EVT_BUTTON(ID_LoadStoredPosition, MyFrame::OnInitializeLoadStoredPosition)
  EVT_BUTTON(ID_HomeStages, MyFrame::OnInitializeHomeLinearStages)
  EVT_BUTTON(ID_SetDistanceWActuatorCollision, MyFrame::OnLengthsSetDistanceWActuatorCollision)
  EVT_BUTTON(ID_SetMountingLength, MyFrame::OnLengthsSetMountingLength)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_BUTTON(ID_LoadLimitSet1, MyFrame::OnLimitsLoadSet1)
  EVT_BUTTON(ID_LoadLimitSet2, MyFrame::OnLimitsLoadSet2)
  EVT_BUTTON(ID_LoadLimitSet3, MyFrame::OnLimitsLoadSet3)
  EVT_BUTTON(ID_LoadLimitSet4, MyFrame::OnLimitsLoadSet4)
  EVT_BUTTON(ID_LimitsDistanceGoTo, MyFrame::OnLimitsGoTo)
  EVT_BUTTON(ID_SetLimits, MyFrame::OnLimitsSetLimits)
  EVT_BUTTON(ID_LimitsSetL0, MyFrame::OnSetL0)
  EVT_RADIOBUTTON(ID_OneStepStressForce, MyFrame::OnOneStepStressForce)
  EVT_RADIOBUTTON(ID_OneStepDistance, MyFrame::OnOneStepDistance)
  EVT_BUTTON(ID_OneStepSendToProtocol, MyFrame::OnOneStepSendToProtocol)
  EVT_RADIOBUTTON(ID_ContinuousStressForce, MyFrame::OnContinuousStressForce)
  EVT_RADIOBUTTON(ID_ContinuousDistance, MyFrame::OnContinuousDistance)
  EVT_RADIOBUTTON(ID_ContinuousMaxValue, MyFrame::OnContinuousMaxValue)
  EVT_RADIOBUTTON(ID_ContinuousSteps, MyFrame::OnContinuousSteps)
  EVT_BUTTON(ID_ContinuousSendToProtocol, MyFrame::OnContinuousSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedPercent, MyFrame::OnPreloadSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedMm, MyFrame::OnPreloadSpeedMmChanged)
  EVT_BUTTON(ID_PreloadSendToProtocol, MyFrame::OnPreloadSendToProtocol)
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
    m_MinDistanceLimit(0),
    m_MaxForceLimit(50000.0),
    m_MinForceLimit(-50000.0),
    m_DistanceLimitExceededFlag(false),
    m_DisableDecreaseDistanceFlag(false),
    m_DisableIncreaseDistanceFlag(false),
    m_GageLength(0),
    m_ZeroLength(0),
    m_Area(0),
    m_PreloadDoneFlag(true),
    m_CurrentForceUpdateDelay(0),
    m_ShowGraphFlag(false),
    m_VectorLayer(_("Vector")),
    m_XAxis(nullptr),
    m_Y1Axis(nullptr),
    m_Y2Axis(nullptr),
    m_MessageHandlersFinishedNumber(0)
{

  SetIcon(wxICON(sample));

  // Set the required ID's
  m_DecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_IncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_InitializeHomeMotorsButton->SetId(ID_HomeStages);
  m_LengthsLEButton->SetId(ID_SetDistanceWActuatorCollision);
  m_LengthsMountingLengthButton->SetId(ID_SetMountingLength);
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
  m_LimitsSetL0Button->SetId(ID_LimitsSetL0);
  m_PreloadSpeedPreloadSpinCtrl->SetId(ID_PreloadSpeedPercent);
  m_PreloadSpeedMmSpinCtrl->SetId(ID_PreloadSpeedMm);
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

  // Create graph
  m_Graph = std::unique_ptr<mpWindow>(new mpWindow(m_GraphPanel, wxID_ANY));

  // Define layer for the graph
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_VectorLayer.SetContinuity(true);
    m_StressForcePreviewVector.SetContinuity(true);
    m_DistancePreviewVector.SetContinuity(true);
    wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
    wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);
    m_VectorLayer.SetPen(vectorpenStressForce);
    m_StressForcePreviewVector.SetPen(vectorpenStressForce);
    m_DistancePreviewVector.SetPen(vectorpenDistance);
    m_VectorLayer.SetDrawOutsideMargins(false);
    m_StressForcePreviewVector.SetDrawOutsideMargins(false);
    m_DistancePreviewVector.SetDrawOutsideMargins(false);

    m_MaxStressForceLimitVector.SetPen(vectorpenStressForce);
    m_MaxStressForceLimitVector.SetContinuity(true);
    m_MaxStressForceLimitVector.SetName("Maximal stress/force limit");
    m_MinStressForceLimitVector.SetPen(vectorpenStressForce);
    m_MinStressForceLimitVector.SetContinuity(true);
    m_MinStressForceLimitVector.SetName("Minimal stress/force limit");
    m_MaxDistanceLimitVector.SetPen(vectorpenDistance);
    m_MaxDistanceLimitVector.SetContinuity(true);
    m_MaxDistanceLimitVector.SetName("Maximal distance limit");
    m_MinDistanceLimitVector.SetPen(vectorpenDistance);
    m_MinDistanceLimitVector.SetContinuity(true);
    m_MinDistanceLimitVector.SetName("Minimal distance limit");
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
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);
  m_Graph->DelLayer(&m_MaxStressForceLimitVector);
  m_Graph->DelLayer(&m_MinStressForceLimitVector);
  m_Graph->DelLayer(&m_MaxDistanceLimitVector);
  m_Graph->DelLayer(&m_MinDistanceLimitVector);
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
          }else if((m_MinDistanceLimit + 0.03 / 0.00009921875/*mm per micro step*//*distance threshold*/) <= m_CurrentDistance){
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
          }else if(m_MinForceLimit < m_CurrentForce){
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

  // Hide steps in ContinuousEvent
  m_ContinuousStressForceStepsStaticText->Show(false);
  m_ContinuousStressForceStepsSpinCtrl->Show(false);
  m_ContinuousDistanceStepsStaticText->Show(false);
  m_ContinuousDistanceStepsSpinCtrl->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_LengthsLESpinCtrl->SetDigits(2);
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
  m_ContinuousStressForceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousStressForceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousStressForceMaxValueSpinCtrl->SetDigits(2);
  m_ContinuousDistanceVelocitySpinCtrl->SetDigits(2);
  m_ContinuousDistanceHoldTimeSpinCtrl->SetDigits(2);
  m_ContinuousDistanceIncrementSpinCtrl->SetDigits(2);
  m_ContinuousDistanceMaxValueSpinCtrl->SetDigits(2);
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
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal stress [kPa]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal stress [kPa]:");
    m_PreloadLimitStaticText->SetLabelText("Stress Limit [kPa]");
    m_OneStepStressForceUpperLimitStaticText->SetLabelText("Upper limit [kPa]:");
    m_OneStepStressForceLowerLimitStaticText->SetLabelText("Lower limit [kPa]:");
    m_OneStepStressForceUpperLimitStaticText->SetLabelText("Upper limit [kPa]:");
    m_OneStepStressForceLowerLimitStaticText->SetLabelText("Lower limit [kPa]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText("Incrementd [dkPa]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("kPa");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Smax.");
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
    m_LimitsLimitMaxForceStaticText->SetLabelText("Maximal force [N]:");
    m_LimitsLimitMinForceStaticText->SetLabelText("Minimal force [N]:");
    m_OneStepStressForceUpperLimitStaticText->SetLabelText("Upper limit [N]:");
    m_OneStepStressForceLowerLimitStaticText->SetLabelText("Lower limit [N]:");
    m_PreloadLimitStaticText->SetLabelText("Force Limit [N]");
    m_OneStepStressForceUpperLimitStaticText->SetLabelText("Upper limit [N]:");
    m_OneStepStressForceLowerLimitStaticText->SetLabelText("Lower limit [N]:");
    m_OneStepStressForceUpperLimitStaticText->SetLabelText("Upper limit [N]:");
    m_OneStepStressForceLowerLimitStaticText->SetLabelText("Lower limit [N]:");
    m_ContinuousStressForceIncrementStaticText->SetLabelText("Incrementd [dN]:");
    m_ContinuousStressForceMaxValueValueRadioBtn->SetLabelText("N");
    m_ContinuousStressForceMaxValuePercentRadioBtn->SetLabelText("%Fmax.");
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
 * @brief Method wich will be executed, when the user klicks on load stored position button.
 * @param event Occuring event
 */
void MyFrame::OnInitializeLoadStoredPosition(wxCommandEvent& event){
  (m_LinearStages.at(0))->loadStoredPosition();
  (m_LinearStages.at(1))->loadStoredPosition();
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
  // Reset limit
  m_MaxDistanceLimit = 153 / 0.00009921875/*mm per micro step*/;
  m_StageFrame->setMaxDistanceLimit(153);
  m_StageFrame->setMinDistanceLimit(0);

  MyHomeStages *homestages = new MyHomeStages(m_LinearStages, this);
  homestages->Show();
}

/**
 * @brief Method wich will be executed, when the user clicks on the set Le button.
 * @param event Occuring event
 */
void MyFrame::OnLengthsSetDistanceWActuatorCollision(wxCommandEvent& event){
  m_DistanceWActuatorCollisionSetFlag = true;
  // Set min distance.
  m_StageFrame->setMinDistanceLimit((m_CurrentDistance) * 0.00009921875/*mm per micro step*/);
  m_StageFrame->setDistanceWActuatorCollision(m_LengthsLESpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/);
}

/**
 * @brief Method wich will be executed, when the user clicks on the set length button.
 * @param event Occuring event
 */
void MyFrame::OnLengthsSetMountingLength(wxCommandEvent& event){
  m_GageLength = m_CurrentDistance;
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
 * @brief Method wich will be executed, when the user clicks on the "Go to" button in clamping position.
 * @param event Occuring event
 */
void MyFrame::OnLimitsGoTo(wxCommandEvent& event){
  m_StageFrame->gotoMMDistance(m_LimitsGoToSpinCtrl->GetValue());
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Set L0" button in limits.
 * @param event Occuring event
 */
void MyFrame::OnSetL0(wxCommandEvent& event){
  m_GageLength = m_CurrentDistance;
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
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in preload.
 * @param event Occuring event
 */
void MyFrame::OnPreloadSendToProtocol(wxCommandEvent& event){
  checkProtocol();

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitVector;
    minlimitvector = &m_MinStressForceLimitVector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitVector;
    minlimitvector = &m_MinDistanceLimitVector;
  }
  int limit = 0;
  if(0 == m_InitializeUnitRadioBox->GetSelection()){
    limit = m_PreloadLimitSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() * 10;
  } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
    limit = m_PreloadLimitSpinCtrl->GetValue() * 10000.0;
  }

  //Experiment* experiment = new Preload(ExperimentType::Preload,
  std::unique_ptr<Experiment> experiment(new Preload(m_StageFrame,
                                                     m_ForceSensorMessageHandler,
                                                     &m_VectorLayer,
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
                                                     &m_StagesStoppedFlag,
                                                     &m_StagesStoppedMutex,

                                                     ExperimentType::Preload,
                                                     m_DistanceOrStressOrForce,
                                                     m_GageLength,
                                                     m_ZeroLength,
                                                     m_CurrentDistance,
                                                     m_InitializeCrossSectionSpinCtrl->GetValue(),

                                                     limit,
                                                     m_PreloadSpeedMmSpinCtrl->GetValue()));

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

  mpFXYVector *maxlimitvector;
  mpFXYVector *minlimitvector;
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    maxlimitvector = &m_MaxStressForceLimitVector;
    minlimitvector = &m_MinStressForceLimitVector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitVector;
    minlimitvector = &m_MinDistanceLimitVector;
  }

  DistanceOrStressOrForce distanceOrStressOrForce;
  Experiment::DistanceOrPercentage velocityDistanceOrPercentage;
  double velocity = 0;
  double holdtime1 = 0;
  Experiment::DistanceOrPercentage upperlimitDistanceOrPercentage;
  double upperlimitpercent = 0;
  long upperlimit = 0;
  double holdtime2 = 0;
  Experiment::DistanceOrPercentage lowerlimitDistanceOrPercentage;
  double lowerlimitpercent = 0;
  long lowerlimit = 0;
  if(true == m_OneStepStressForceRadioBtn->GetValue()){
    distanceOrStressOrForce = m_DistanceOrStressOrForce;
    holdtime1 = m_OneStepStressForceHoldTime1SpinCtrl->GetValue();
    holdtime2 = m_OneStepStressForceHoldTime2SpinCtrl->GetValue();
    if(0 == m_InitializeUnitRadioBox->GetSelection()){
      upperlimit = m_OneStepStressForceUpperLimitSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() * 10.0;
      lowerlimit = m_OneStepStressForceLowerLimitSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() * 10.0;
    } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
      upperlimit = m_OneStepStressForceUpperLimitSpinCtrl->GetValue() * 10000.0;
      lowerlimit = m_OneStepStressForceLowerLimitSpinCtrl->GetValue() * 10000.0;
    }

    if(true == m_OneStepStressForceVelocityMmRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
    } else if(true == m_OneStepStressForceVelocityPercentRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
    }
    velocity = m_OneStepStressForceVelocitySpinCtrl->GetValue();
  }else if(true == m_OneStepDistanceRadioBtn->GetValue()){
    distanceOrStressOrForce = DistanceOrStressOrForce::Distance;
    holdtime1 = m_OneStepDistanceHoldTime1SpinCtrl->GetValue();

    if(true == m_OneStepDistanceVelocityMmRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
    } else if(true == m_OneStepDistanceVelocityPercentRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
    }
    velocity = m_OneStepDistanceVelocitySpinCtrl->GetValue();

    if(true == m_OneStepDistanceUpperLimitMmRadioBtn->GetValue()){
      upperlimitDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
      upperlimit = m_OneStepDistanceUpperLimitSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
    } else if(true == m_OneStepDistanceUpperLimitPercentRadioBtn->GetValue()){
      upperlimitDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
      upperlimitpercent = m_OneStepDistanceUpperLimitSpinCtrl->GetValue();
    }
    holdtime2 = m_OneStepDistanceHoldTime2SpinCtrl->GetValue();

    if(true == m_OneStepDistanceLowerLimitMmRadioBtn->GetValue()){
      lowerlimitDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
      lowerlimit = m_OneStepDistanceLowerLimitSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
    } else if(true == m_OneStepDistanceLowerLimitPercentRadioBtn->GetValue()){
      lowerlimitDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
      lowerlimitpercent = m_OneStepDistanceLowerLimitSpinCtrl->GetValue();
    }
  }
  wxLogMessage(std::string("MyFrame: upper limit: " + std::to_string(upperlimit) +
                           " lower limit: " + std::to_string(lowerlimit)).c_str());

  Experiment::DistanceOrPercentage holddistanceOrPercentage;
  if(true == m_OneStepEndOfEventHoldMmRadioBtn->GetValue()){
    holddistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
  } else if(true == m_OneStepEndOfEventHoldPercentRadioBtn->GetValue()){
    holddistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
  }

  int cycles = 1;
  if(true == m_OneStepEndOfEventRepeatCheckBox->GetValue()){
    cycles = m_OneStepEndOfEventRepeatSpinCtrl->GetValue();
  }

  Experiment::BehaviorAfterStop behaviorAfterStop;
  if(true == m_OneStepEndOfEventHoldRadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::HoldADistance;
  } else if(true == m_OneStepEndOfEventL0RadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::GoToL0;
  }

  std::unique_ptr<Experiment> experiment(new OneStepEvent(m_StageFrame,
                                                          m_ForceSensorMessageHandler,
                                                          &m_VectorLayer,
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
                                                          &m_StagesStoppedFlag,
                                                          &m_StagesStoppedMutex,

                                                          ExperimentType::OneStepEvent,
                                                          distanceOrStressOrForce,
                                                          m_GageLength,
                                                          m_ZeroLength,
                                                          m_CurrentDistance,
                                                          m_InitializeCrossSectionSpinCtrl->GetValue(),

                                                          velocityDistanceOrPercentage,
                                                          velocity,
                                                          velocity,
                                                          holdtime1,
                                                          upperlimitDistanceOrPercentage,
                                                          upperlimitpercent,
                                                          upperlimit,
                                                          holdtime2,
                                                          lowerlimitDistanceOrPercentage,
                                                          lowerlimitpercent,
                                                          lowerlimit,
                                                          holddistanceOrPercentage,
                                                          m_OneStepEndOfEventHoldSpinCtrl->GetValue(),
                                                          m_OneStepEndOfEventHoldSpinCtrl->GetValue(),
                                                          cycles,
                                                          behaviorAfterStop));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Method wich will be executed, when the user chooses stress/force in the continuous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousStressForce(wxCommandEvent& event){
  m_ContinuousDistancePanel1->Show(false);
  m_ContinuousStressForcePanel->Show(true);
  m_ContinuousPanel21->Layout();
}

/**
 * @brief Method wich will be executed, when the user chooses distance in the contiunous event.
 * @param event Occuring event
 */
void MyFrame::OnContinuousDistance(wxCommandEvent& event){
  m_ContinuousStressForcePanel->Show(false);
  m_ContinuousDistancePanel1->Show(true);
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
    maxlimitvector = &m_MaxStressForceLimitVector;
    minlimitvector = &m_MinStressForceLimitVector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitVector;
    minlimitvector = &m_MinDistanceLimitVector;
  }

  DistanceOrStressOrForce distanceOrStressOrForce;
  bool ramptofailureactiveflag = false;
  Experiment::DistanceOrPercentage velocityDistanceOrPercentage;
  double velocity = 0;
  double holdtime = 0;
  Experiment::DistanceOrPercentage incrementDistanceOrPercentage;
  double incrementpercent = 0;
  double increment = 0;
  ContinuousEvent::StepsOrMaxValue stepsOrMaxValue;
  Experiment::DistanceOrPercentage maxvalueDistanceOrPercentage;
  double maxvaluepercent = 0;
  double maxvalue = 0;
  double ramptofailurepercent = 0;
  Experiment::DistanceOrPercentage stepsDistanceOrPercentage;
  int steps = 0;
  if(true == m_ContinuousStressForceRadioBtn->GetValue()){
    distanceOrStressOrForce = m_DistanceOrStressOrForce;
    holdtime = m_ContinuousStressForceHoldTimeSpinCtrl->GetValue();
    incrementDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
    if(0 == m_InitializeUnitRadioBox->GetSelection()){
      increment = m_ContinuousStressForceIncrementSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() * 10.0;
    } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
      increment = m_ContinuousStressForceIncrementSpinCtrl->GetValue() * 10000.0;
    }

    if(true == m_ContinuousStressForceVelocityMmRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousStressForceVelocityPercentRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
    }
    velocity = m_ContinuousStressForceVelocitySpinCtrl->GetValue();

    if(true == m_ContinuousStressForceMaxValueRadioBtn->GetValue()){
      stepsOrMaxValue = ContinuousEvent::StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousStressForceMaxValueValueRadioBtn->GetValue()){
        steps = m_ContinuousStressForceMaxValueSpinCtrl->GetValue() / m_ContinuousStressForceIncrementSpinCtrl->GetValue();
      } else if(true == m_ContinuousStressForceMaxValuePercentRadioBtn->GetValue()){
        ramptofailureactiveflag = true;
        ramptofailurepercent = m_ContinuousStressForceMaxValueSpinCtrl->GetValue();
      }
      //std::cout << "MyFrame: steps: " << steps << std::endl;
    } else if(true == m_ContinuousStressForceStepsRadioBtn->GetValue()){
      stepsOrMaxValue = ContinuousEvent::StepsOrMaxValue::Steps;
      steps = m_ContinuousStressForceStepsSpinCtrl->GetValue();
    }
    if(0 == m_InitializeUnitRadioBox->GetSelection()){
      maxvalue = m_ContinuousStressForceMaxValueSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() * 10.0;
    } else if(1 == m_InitializeUnitRadioBox->GetSelection()){
      maxvalue = m_ContinuousStressForceMaxValueSpinCtrl->GetValue() * 10000.0;
    }
    stepsDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;

  }else if(true == m_ContinuousDistanceRadioBtn->GetValue()){
    distanceOrStressOrForce = DistanceOrStressOrForce::Distance;
    holdtime = m_ContinuousDistanceHoldTimeSpinCtrl->GetValue();

    if(true == m_ContinuousDistanceVelocityMmRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
    } else if(true == m_ContinuousDistanceVelocityPercentRadioBtn->GetValue()){
      velocityDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
    }
    velocity = m_ContinuousDistanceVelocitySpinCtrl->GetValue();

    if(true == m_ContinuousDistanceIncrementMmRadioBtn->GetValue()){
      incrementDistanceOrPercentage = Experiment::DistanceOrPercentage::Distance;
      increment = m_ContinuousDistanceIncrementSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
    } else if(true == m_ContinuousDistanceIncrementPercentRadioBtn->GetValue()){
      incrementDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
      incrementpercent = m_ContinuousDistanceIncrementSpinCtrl->GetValue();
    }

    if(true == m_ContinuousDistanceMaxValueRadioBtn->GetValue()){
      stepsOrMaxValue = ContinuousEvent::StepsOrMaxValue::MaxValue;

      if(true == m_ContinuousDistanceMaxValueMmRadioBtn->GetValue()){
        maxvalueDistanceOrPercentage == Experiment::DistanceOrPercentage::Distance;
        maxvalue = m_ContinuousDistanceMaxValueSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/;
        if(true == m_ContinuousDistanceIncrementMmRadioBtn->GetValue()){
          stepsOrMaxValue = ContinuousEvent::StepsOrMaxValue::Steps;
          steps = maxvalue / increment;
          //std::cout << "MyFrame: steps: " << steps << std::endl;
        }
      } else if(true == m_ContinuousDistanceMaxValuePercentRadioBtn->GetValue()){
        maxvalueDistanceOrPercentage = Experiment::DistanceOrPercentage::Percentage;
        maxvaluepercent = m_ContinuousDistanceMaxValueSpinCtrl->GetValue();
      }
    } else if(true == m_ContinuousDistanceStepsRadioBtn->GetValue()){
      stepsOrMaxValue = ContinuousEvent::StepsOrMaxValue::Steps;
      steps = m_ContinuousDistanceStepsSpinCtrl->GetValue();
    }
  }

  int cycles = 1;
  if(true == m_ContinuousEndOfEventRepeatCheckBox->GetValue()){
    cycles = m_ContinuousEndOfEventRepeatSpinCtrl->GetValue();
  }

  Experiment::BehaviorAfterStop behaviorAfterStop;
  if(true == m_ContinuousEndOfEventStopRadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::Stop;
  } else if(true == m_ContinuousEndOfEventL0RadioBtn->GetValue()){
    behaviorAfterStop = Experiment::BehaviorAfterStop::GoToL0;
  }

  std::unique_ptr<Experiment> experiment(new ContinuousEvent(m_StageFrame,
                                                             m_ForceSensorMessageHandler,
                                                             &m_VectorLayer,
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
                                                             &m_StagesStoppedFlag,
                                                             &m_StagesStoppedMutex,

                                                             ExperimentType::ContinuousEvent,
                                                             distanceOrStressOrForce,
                                                             ramptofailureactiveflag,
                                                             m_GageLength,
                                                             m_ZeroLength,
                                                             m_CurrentDistance,
                                                             m_InitializeCrossSectionSpinCtrl->GetValue(),

                                                             velocityDistanceOrPercentage,
                                                             velocity,
                                                             velocity,
                                                             holdtime,
                                                             incrementDistanceOrPercentage,
                                                             incrementpercent,
                                                             increment,
                                                             stepsOrMaxValue,
                                                             maxvalueDistanceOrPercentage,
                                                             maxvaluepercent,
                                                             maxvalue,
                                                             steps,
                                                             ramptofailurepercent,
                                                             cycles,
                                                             behaviorAfterStop));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Method wich will be executed, when the user sets the limits.
 * @param event Occuring event
 */
void MyFrame::OnLimitsSetLimits(wxCommandEvent& event){
  m_MaxDistanceLimit = m_LimitsLimitMaxDistanceSpinCtrl->GetValue();
  m_MinDistanceLimit = m_LimitsLimitMinDistanceSpinCtrl->GetValue();
  if(1 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = m_LimitsLimitMaxForceSpinCtrl->GetValue() * 10000.0;
    m_MinForceLimit = m_LimitsLimitMinForceSpinCtrl->GetValue() * 10000.0;
  } else if(0 == m_InitializeUnitRadioBox->GetSelection()){
    m_MaxForceLimit = (m_LimitsLimitMaxForceSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() / 1000) * 10000.0;
    m_MinForceLimit = (m_LimitsLimitMinForceSpinCtrl->GetValue() * m_InitializeCrossSectionSpinCtrl->GetValue() / 1000) * 10000.0;
  }

  m_StageFrame->setMaxDistanceLimit(m_MaxDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_StageFrame->setMinDistanceLimit(m_MinDistanceLimit);
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000)));
  m_MaxDistanceLimit /= 0.00009921875/*mm per micro step*/;
  m_MinDistanceLimit /= 0.00009921875/*mm per micro step*/;
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
 * @brief Method wich will be executed, when the user clicks on the export csv button.
 * @param event Occuring event
 */
void MyFrame::OnExportCSV(wxCommandEvent& event){
  checkProtocol();
  std::unique_ptr<MyExportDialog> dialog = std::unique_ptr<MyExportDialog>(new MyExportDialog(m_CurrentProtocol, m_CurrentProtocol->getExperimentNames()));
  dialog->ShowModal();
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

  m_Graph->SaveScreenshot(pathAndFilename, wxBITMAP_TYPE_PNG);
  wxLogMessage(std::string("Graph saved in: " + pathAndFilename).c_str());
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

  m_LogTextCtrl->SaveFile(pathAndFilename);

  wxLogMessage(std::string("Log saved in: " + pathAndFilename).c_str());
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
    maxlimitvector = &m_MaxStressForceLimitVector;
    minlimitvector = &m_MinStressForceLimitVector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitVector;
    minlimitvector = &m_MinDistanceLimitVector;
  }

  std::unique_ptr<Experiment> experiment(new Pause(m_StageFrame,
                                                   m_ForceSensorMessageHandler,
                                                   &m_VectorLayer,
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
                                                   m_ZeroLength,
                                                   m_CurrentDistance,
                                                   m_InitializeCrossSectionSpinCtrl->GetValue()));

  Pause *ptr = dynamic_cast<Pause*>(experiment.get());

  std::unique_ptr<MyPauseDialog> dialog = std::unique_ptr<MyPauseDialog>(new MyPauseDialog(ptr));
  dialog->ShowModal();

  if(true == dialog->getCreateExperimentFlag()){
    m_CurrentProtocol->addExperiment(experiment);
  }
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
    maxlimitvector = &m_MaxStressForceLimitVector;
    minlimitvector = &m_MinStressForceLimitVector;
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    maxlimitvector = &m_MaxDistanceLimitVector;
    minlimitvector = &m_MinDistanceLimitVector;
  }

  std::unique_ptr<Experiment> experiment(new PauseResume(m_StageFrame,
                                                         m_ForceSensorMessageHandler,
                                                         &m_VectorLayer,
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
                                                         m_ZeroLength,
                                                         m_CurrentDistance,
                                                         m_InitializeCrossSectionSpinCtrl->GetValue()));

  m_CurrentProtocol->addExperiment(experiment);
}

/**
 * @brief Shows pause/resume dialog.
 */
void MyFrame::showPauseResumeDialogFromPauseResume(std::condition_variable *wait, std::mutex *mutex){
  CallAfter(&MyFrame::showPauseResumeDialog, wait, mutex);
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
    tmp << (static_cast<double>(((m_CurrentForce) / 10000.0) / m_InitializeCrossSectionSpinCtrl->GetValue()) * 1000) << m_ForceUnit;
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
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);
  m_Graph->DelLayer(&m_MaxStressForceLimitVector);
  m_Graph->DelLayer(&m_MinStressForceLimitVector);
  m_Graph->DelLayer(&m_MaxDistanceLimitVector);
  m_Graph->DelLayer(&m_MinDistanceLimitVector);

  // Clear vectors
  {
    std::lock_guard<std::mutex> lck{m_VectorLayerMutex};
    m_VectorLayer.Clear();
  }

  // Defines pen styles and font.
  wxPen vectorpenStressForce(*wxBLUE, 2, wxSOLID);
  wxPen vectorpenDistance(*wxGREEN, 2, wxSOLID);
  wxPen vectorpenLimit(*wxRED, 2, wxSOLID);
  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

  // Set up and add axis.
  m_XAxis.reset(new mpScaleX(wxT("Distance [mm]"), mpALIGN_BOTTOM, true, mpX_NORMAL));

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
  m_Graph->AddLayer(&m_VectorLayer);
  if((DistanceOrStressOrForce::Stress == m_DistanceOrStressOrForce) || (DistanceOrStressOrForce::Force == m_DistanceOrStressOrForce)){
    m_MaxStressForceLimitVector.SetPen(vectorpenLimit);
    m_MinStressForceLimitVector.SetPen(vectorpenLimit);
    m_Graph->AddLayer(&m_MaxStressForceLimitVector);
    m_Graph->AddLayer(&m_MinStressForceLimitVector);
    m_VectorLayer.SetPen(vectorpenStressForce);
  } else if(DistanceOrStressOrForce::Distance == m_DistanceOrStressOrForce){
    m_MaxDistanceLimitVector.SetPen(vectorpenLimit);
    m_MinDistanceLimitVector.SetPen(vectorpenLimit);
    m_Graph->AddLayer(&m_MaxDistanceLimitVector);
    m_Graph->AddLayer(&m_MinDistanceLimitVector);
    m_VectorLayer.SetPen(vectorpenDistance);
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
  m_Graph->DelLayer(&m_VectorLayer);
  m_Graph->DelLayer(&m_StressForcePreviewVector);
  m_Graph->DelLayer(&m_DistancePreviewVector);
  m_Graph->DelLayer(&m_MaxStressForceLimitVector);
  m_Graph->DelLayer(&m_MinStressForceLimitVector);
  m_Graph->DelLayer(&m_MaxDistanceLimitVector);
  m_Graph->DelLayer(&m_MinDistanceLimitVector);

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
  m_MaxStressForceLimitVector.SetPen(vectorpenStressForce);
  m_MinStressForceLimitVector.SetPen(vectorpenStressForce);
  m_MaxDistanceLimitVector.SetPen(vectorpenDistance);
  m_MinDistanceLimitVector.SetPen(vectorpenDistance);

  m_StressForcePreviewVector.SetPen(vectorpenStressForce);
  m_DistancePreviewVector.SetPen(vectorpenDistance);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(m_XAxis.get());
  m_Graph->AddLayer(m_Y1Axis.get());
  m_Graph->AddLayer(m_Y2Axis.get());
  m_Graph->AddLayer(&m_StressForcePreviewVector);
  m_Graph->AddLayer(&m_DistancePreviewVector);
  m_Graph->AddLayer(&m_MaxStressForceLimitVector);
  m_Graph->AddLayer(&m_MinStressForceLimitVector);
  m_Graph->AddLayer(&m_MaxDistanceLimitVector);
  m_Graph->AddLayer(&m_MinDistanceLimitVector);

  m_Graph->Fit();
}

/**
 * @brief Checks if a protocol object is already created, otherwise creates it.
 */
void MyFrame::checkProtocol(void){
  if(nullptr == m_CurrentProtocol){
  m_CurrentProtocol = std::shared_ptr<Protocols>(new Protocols(m_ProtocolsListBox,
                                                               this,
                                                               &m_StagesStoppedFlag,
                                                               &m_StagesStoppedMutex,
                                                               &m_WaitMutex,
                                                               &m_Wait,
                                                               &m_PreloadDoneFlag,
                                                               &m_PreloadDoneMutex,

                                                               m_ProtocolsLoopCheckBox->GetValue(),
                                                               m_InitializeCrossSectionSpinCtrl->GetValue(),
                                                               m_LimitsLimitMaxDistanceSpinCtrl->GetValue(),
                                                               m_LimitsLimitMinDistanceSpinCtrl->GetValue(),
                                                               m_LimitsLimitMaxForceSpinCtrl->GetValue(),
                                                               m_LimitsLimitMinForceSpinCtrl->GetValue(),

                                                               &m_VectorLayer,
                                                               &m_StressForcePreviewVector,
                                                               &m_DistancePreviewVector,
                                                               &m_MaxStressForceLimitVector,
                                                               &m_MinStressForceLimitVector,
                                                               &m_MaxDistanceLimitVector,
                                                               &m_MinDistanceLimitVector,
                                                               m_StoragePath));
  }
}
