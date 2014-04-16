// Includes
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/image.h>
#include <mutex>
#include <functional>
#include "../../include/ctb-0.13/serport.h"
#include "myframe.h"
#include "mysamplingfrequency_base.h"
#include "myports.h"
#include "myfileoutput_base.h"
#include "../experiments/preload.h"
#include "../experiments/conditioning.h"
#include "../experiments/ramp2failure.h"

#include <iostream>

// MyLissajoux

class MyLissajoux : public mpFXY
{
    double m_rad;
    int    m_idx;
public:
    MyLissajoux(double rad) : mpFXY( wxT("Lissajoux")) { m_rad=rad; m_idx=0; m_drawOutsideMargins = false;}
    virtual bool GetNextXY( double & x, double & y )
    {
        if (m_idx < 360)
        {
            x = m_rad * cos(m_idx / 6.283185*360);
            y = m_rad * sin(m_idx / 6.283185*360*3);
            m_idx++;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    virtual void Rewind() { m_idx=0; }
    virtual double GetMinX() { return -m_rad; }
    virtual double GetMaxX() { return  m_rad; }
    virtual double GetMinY() { return -m_rad; }
    virtual double GetMaxY() { return  m_rad; }
};


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
  EVT_MENU(XRCID("m_PreloadCalculateDiameterCheckBox"), MyFrame::OnCalculateDiameter)
  EVT_CHECKBOX(ID_CalculateDiameter, MyFrame::OnCalculateDiameter)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_RADIOBOX(ID_ChamberMeasurement, MyFrame::OnChamberMeasurement)
  EVT_RADIOBUTTON(ID_DistanceLimit, MyFrame::OnDistanceLimit)
  EVT_RADIOBUTTON(ID_StressLimit, MyFrame::OnStressLimit)
  EVT_RADIOBOX(ID_GoTo, MyFrame::OnGoTo)
  EVT_BUTTON(ID_MotorDecreaseDistance,	MyFrame::OnMotorDecreaseDistance)
  EVT_BUTTON(ID_MotorIncreaseDistance,	MyFrame::OnMotorIncreaseDistance)
  EVT_BUTTON(ID_MotorStop,	MyFrame::OnMotorStop)
  EVT_BUTTON(ID_HomeStages, MyFrame::OnHomeLinearStages)
  EVT_SPINCTRLDOUBLE(ID_ClampingPosValue, MyFrame::OnClampingPosValueChanged)
  EVT_BUTTON(ID_ClampingGoTo, MyFrame::OnClampingGoTo)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedPercent, MyFrame::OnPreloadSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_PreloadSpeedMm, MyFrame::OnPreloadSpeedMmChanged)
  EVT_BUTTON(ID_PreloadSendToProtocol, MyFrame::OnPreloadSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedPercent, MyFrame::OnConditioningSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_ConditioningSpeedMm, MyFrame::OnConditioningSpeedMmChanged)
  EVT_BUTTON(ID_ConditioningSendToProtocol, MyFrame::OnConditioningSendToProtocol)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedPercent, MyFrame::OnRamp2FailureSpeedPercentChanged)
  EVT_SPINCTRLDOUBLE(ID_Ramp2FailureSpeedMm, MyFrame::OnRamp2FailureSpeedMmChanged)
  EVT_BUTTON(ID_Ramp2FailureSendToProtocol, MyFrame::OnRamp2FailureSendToProtocol)
wxEND_EVENT_TABLE()

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
    m_PreloadDistance(0),
    m_StressOrForce(Experiment::StressOrForce::Force),
    m_CurrentExperiment(NULL),
    m_Area(0),
    m_ExperimentRunningFlag(false),
    m_PreloadDoneFlag(false)
{
  SetIcon(wxICON(sample));

  // Set the required ID's
  m_PreloadCalculateCrosssectionCheckBox->SetId(ID_CalculateDiameter);
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
  m_ClampingPositionSpinCtrl->SetId(ID_ClampingPosValue);
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

  // Create graph
  m_Graph = new mpWindow(m_GraphPanel, wxID_ANY);

  //---------------------------- Test Data for Graph
  mpLayer* l;

  // Create a mpFXYVector layer
  mpFXYVector* vectorLayer = new mpFXYVector(_("Vector"));
  // Create two vectors for x,y and fill them with data
  std::vector<double> vectorx, vectory;
  double xcoord;
  for (unsigned int p = 0; p < 100; p++) {
    xcoord = ((double)p-50.0)*5.0;
    vectorx.push_back(xcoord);
    vectory.push_back(0.0001*pow(xcoord, 3));
  }
  vectorLayer->SetData(vectorx, vectory);
  vectorLayer->SetContinuity(true);
  wxPen vectorpen(*wxBLUE, 2, wxSOLID);
  vectorLayer->SetPen(vectorpen);
  vectorLayer->SetDrawOutsideMargins(false);


  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
  mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
  xaxis->SetFont(graphFont);
  yaxis->SetFont(graphFont);
  xaxis->SetDrawOutsideMargins(false);
  yaxis->SetDrawOutsideMargins(false);

  m_Graph->SetMargins(20, 20, 30, 50);
  m_Graph->EnableMousePanZoom(true);
  m_Graph->AddLayer(xaxis);
  m_Graph->AddLayer(yaxis);
  m_Graph->AddLayer( l = new MyLissajoux( 125.0 ) );
  m_Graph->AddLayer(     vectorLayer );
  m_Graph->AddLayer(     new mpText(wxT("mpText sample"), 10, 10) );
  wxBrush hatch(wxColour(200,200,200), wxSOLID);
  wxBrush hatch2(wxColour(163,208,212), wxSOLID);
  mpInfoLegend* leg;
  m_Graph->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
  leg->SetVisible(true);

  wxPen mypen(*wxRED, 5, wxSOLID);
  l->SetPen( mypen);
  //----------------------------

  // Add graph to window
  m_Graph->Fit();
  m_GraphSizer1->Insert(0, m_Graph, 0, wxEXPAND);
  m_GraphPanel->Layout();

  }

/**
 * @brief Register the liner stages and the stage frame, registers the update method at the stage frame and registers the stop wait conditional variable at the stage frame.
 * @param linearstage Pointer to the vector containing the linear motors.
 */
void MyFrame::registerLinearStage(std::vector<LinearStage *> *linearstage, StageFrame *stageframe){
  m_LinearStages = linearstage;
  m_StageFrame = stageframe;

  // Registers update methods at stage frame.
  m_DistanceId = m_StageFrame->registerUpdateMethod(&UpdatedValuesReceiver::updateValues, this);

  // Registers stop wait conditional variable and mutex.
  m_StageFrame->registerStagesStopped(&m_StagesStoppedFlag, &m_StagesStoppedMutex);
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

  delete m_Graph;
}

/**
 * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
 * 				thread. (CallAfter() asynchronously call the updateDistance method)
 * @param value The position of a stage or a force.
 * @param type	Defines the type of the value (position of stage 1, 2 or force)
 */
void MyFrame::updateValues(long value, UpdatedValuesReceiver::ValueType type){
  switch(type){
    case UpdatedValuesReceiver::ValueType::Distance:
      m_CurrentDistance = value;
      CallAfter(&MyFrame::updateDistance);
      break;
    case UpdatedValuesReceiver::ValueType::Force:
      m_CurrentForce = value;
      CallAfter(&MyFrame::updateForce);
      break;
  }
}

/**
 * @brief Hides calculate diameter options, hides cells panel in chamber stretch, hides distance limit options, hides go to options,
 * 				sets digits for the wxSpinCtrlDouble.
 */
void MyFrame::startup(void){
	// Hide calculate diameter options
	m_PreloadYStaticText->Show(false);
	m_PreloadYSpinCtrl->Show(false);
	m_PreloadXRadioBox->Show(false);
	m_PreloadXSpinCtrl->Show(false);

  // Hide cells panel in Chamber stretch
  m_ChamberStretchCellsPanel->Show(false);

  // Hide distance limit options
  m_ConditioningDistanceLimitStaticText->Show(false);
  m_ConditioningDistanceLimitSpinCtrl->Show(false);
  m_ConditioningDisctanceLimitRadioBox->Show(false);

  // Hide Go to options
  m_R2FGoToSpinCtrl->Show(false);
  m_R2FGoToRadioBox->Show(false);

  // Set digits for the wxSpinCtrlDouble
  m_ClampingPositionSpinCtrl->SetDigits(2);
  m_PreloadYSpinCtrl->SetDigits(2);
  m_PreloadXSpinCtrl->SetDigits(2);
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
	MyFileOutput_Base *fileOutput = new MyFileOutput_Base(this);
	fileOutput->Show();
}

/**
 * @brief Method wich will be executed, when the user activates the calculate diameter settings.
 * @param event Occuring event
 */
void MyFrame::OnCalculateDiameter(wxCommandEvent& event){
	if(!m_PreloadYStaticText->IsShown()){
		m_PreloadYStaticText->Show(true);
		m_PreloadYSpinCtrl->Show(true);
		m_PreloadXRadioBox->Show(true);
		m_PreloadXSpinCtrl->Show(true);
	}else{
		m_PreloadYStaticText->Show(false);
		m_PreloadYSpinCtrl->Show(false);
		m_PreloadXRadioBox->Show(false);
		m_PreloadXSpinCtrl->Show(false);
	}
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
    m_ForceUnit = wxT(" kPa");
    m_StressOrForce = Experiment::StressOrForce::Stress;
  }else{
    m_PreloadLimitStaticText->SetLabelText("Force Limit [N]");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Force Limit [N]");
    m_CreepHoldForceStressStaticText->SetLabelText("Hold Force [N]");
    m_ForceUnit = wxT(" N");
    m_StressOrForce = Experiment::StressOrForce::Force;
  }
}

/**
 * @brief Method wich will be executed, when the user chooses distance as limit.
 * @param event Occuring event
 */
void MyFrame::OnDistanceLimit(wxCommandEvent& event){
  m_ConditioningDistanceLimitStaticText->Show(true);
  m_ConditioningDistanceLimitSpinCtrl->Show(true);
  m_ConditioningDisctanceLimitRadioBox->Show(true);

  m_ConditioningStressForceLimitStaticText->Show(false);
  m_ConditioningStressForceLimitSpinCtrl->Show(false);
}

/**
 * @brief Method wich will be executed, when the user chooses stress as limit.
 * @param event Occuring event
 */
void MyFrame::OnStressLimit(wxCommandEvent& event){
  m_ConditioningStressForceLimitStaticText->Show(true);
  m_ConditioningStressForceLimitSpinCtrl->Show(true);

  m_ConditioningDistanceLimitStaticText->Show(false);
  m_ConditioningDistanceLimitSpinCtrl->Show(false);
  m_ConditioningDisctanceLimitRadioBox->Show(false);
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
 * @brief Method wich will be executed, when the user klicks on the home stage button.
 * @param event Occuring event
 */
void MyFrame::OnHomeLinearStages(wxCommandEvent& event){
  (m_LinearStages->at(0))->home();
  (m_LinearStages->at(1))->home();
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
  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }

  if(true == m_PreloadCalculateCrosssectionCheckBox->IsChecked()){
    switch(m_PreloadXRadioBox->GetSelection()){
      case 0:
        m_Area = (m_PreloadYSpinCtrl->GetValue()) * (1 + m_PreloadXSpinCtrl->GetValue() / 100);
        break;
      case 1:
        m_Area = m_PreloadYSpinCtrl->GetValue() * m_PreloadXSpinCtrl->GetValue();
        break;
    }
  }
  m_CurrentExperiment = new Preload(Experiment::ExperimentType::Preload,
                                    m_StressOrForce,
                                    m_StageFrame,
                                    m_ForceSensorMessageHandler,
                                    &m_Wait,
                                    &m_WaitMutex,
                                    &m_StagesStoppedFlag,
                                    &m_StagesStoppedMutex,
                                    m_PreloadLimitSpinCtrl->GetValue(),
                                    m_PreloadSpeedMmSpinCtrl->GetValue(),
                                    m_Area);

  {
    std::lock_guard<std::mutex> lck{m_PreloadDoneMutex};
    m_PreloadDoneFlag = false;
  }
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = true;
  }
  std::thread t1(&Experiment::process, m_CurrentExperiment, Preload::Event::evStart);
  t1.join();

  m_ExperimentRunningThread = new std::thread(&MyFrame::checkFinishedExperiment, this);
  m_ExperimentRunningThread->detach();

  return;
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSpeedPercentChanged(wxSpinDoubleEvent& event){
 double speedmm = m_PreloadDistance * 0.00009921875/*mm per micro step*/ * (m_ConditioningSpeedPreloadSpinCtrl->GetValue() / 100);
 m_ConditioningSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_ConditioningSpeedMmSpinCtrl->GetValue() / (m_PreloadDistance * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  m_ConditioningSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in conditioning.
 * @param event Occuring event
 */
void MyFrame::OnConditioningSendToProtocol(wxCommandEvent& event){
  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }

  Conditioning::DistanceOrStressForce distanceOrStressForce;
  if(true == m_ConditioningStressRadioBtn->GetValue()){
    distanceOrStressForce = Conditioning::DistanceOrStressForce::StressForce;
  }else if(true == m_ConditioningDistanceRadioBtn->GetValue()){
    distanceOrStressForce = Conditioning::DistanceOrStressForce::Distance;
  }

  long distancelimit = 0;
  switch(m_ConditioningDisctanceLimitRadioBox->GetSelection()){
    case 0:
      distancelimit = static_cast<long>(m_PreloadDistance + m_ConditioningDistanceLimitSpinCtrl->GetValue() / 0.00009921875/*mm per micro step*/);
      break;
    case 1:
      distancelimit = ((m_ConditioningDistanceLimitSpinCtrl->GetValue() / 100) + 1.0) * m_PreloadDistance;
      break;
  }

  //delete m_CurrentExperiment;
  m_CurrentExperiment = new Conditioning(Experiment::ExperimentType::Conditioning,
                                         distanceOrStressForce,
                                         m_StressOrForce,
                                         m_CurrentDistance,
                                         m_StageFrame,
                                         m_LinearStagesMessageHandlers,
                                         m_ForceSensorMessageHandler,
                                         &m_Wait,
                                         &m_WaitMutex,
                                         m_ConditioningStressForceLimitSpinCtrl->GetValue(),
                                         m_ConditioningCyclesSpinCtrl->GetValue(),
                                         distancelimit,
                                         m_ConditioningSpeedMmSpinCtrl->GetValue(),
                                         m_Area, m_PreloadDistance);

  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = true;
  }
  std::thread t1(&Experiment::process, m_CurrentExperiment, Conditioning::Event::evStart);
  t1.join();

  m_ExperimentRunningThread = new std::thread(&MyFrame::checkFinishedExperiment, this);
  m_ExperimentRunningThread->detach();

  return;
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in percent in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSpeedPercentChanged(wxSpinDoubleEvent& event){
  double speedmm = m_PreloadDistance * 0.00009921875/*mm per micro step*/ * (m_R2FSpeedPreloadSpinCtrl->GetValue() / 100.0);
  m_R2FSpeedMmSpinCtrl->SetValue(speedmm);
}

/**
 * @brief Method wich will be executed, when the user changes the speed value in mm in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSpeedMmChanged(wxSpinDoubleEvent& event){
  double speedpercent = m_R2FSpeedMmSpinCtrl->GetValue() / (m_PreloadDistance * 0.00009921875/*mm per micro step*/) * 100/*%*/;
  m_R2FSpeedPreloadSpinCtrl->SetValue(speedpercent);
}

/**
 * @brief Method wich will be executed, when the user clicks on the "Send to protocol" button in ramp 2 failure.
 * @param event Occuring event
 */
void MyFrame::OnRamp2FailureSendToProtocol(wxCommandEvent& event){
  // Return if an experiment is currently running
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    if(true == m_ExperimentRunningFlag){
      return;
    }
  }

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
    distanceafterfailure = m_R2FGoToSpinCtrl->GetValue() /0.00009921875/*mm per micro step*/;
  }else if(1 == m_R2FGoToRadioBox->GetSelection()){
    distanceafterfailure = ((m_R2FGoToSpinCtrl->GetValue() / 100) /*+ 1.0*/) * m_PreloadDistance;
  }

  m_CurrentExperiment = new Ramp2Failure(Experiment::ExperimentType::Ramp2Failure,
                                         m_StressOrForce,
                                         m_StageFrame,
                                         m_LinearStagesMessageHandlers,
                                         m_ForceSensorMessageHandler,
                                         &m_Wait,
                                         &m_WaitMutex,
                                         behavior,
                                         m_R2FSpeedMmSpinCtrl->GetValue(),
                                         m_R2FDropBStopSpinCtrl->GetValue(),
                                         m_Area,
                                         m_PreloadDistance,
                                         distanceafterfailure);
  {
    std::lock_guard<std::mutex> lck{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = true;
  }
  std::thread t1(&Experiment::process, m_CurrentExperiment, Ramp2Failure::Event::evStart);
  t1.join();

  m_ExperimentRunningThread = new std::thread(&MyFrame::checkFinishedExperiment, this);
  m_ExperimentRunningThread->detach();

  return;
}

/**
 * @brief Method wich will be executed, when the user klicks on the decrease distance button.
 * @param event Occuring event
 */
void MyFrame::OnMotorDecreaseDistance(wxCommandEvent& event){
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

  if(NULL != m_CurrentExperiment){
    m_CurrentExperiment = NULL;
  }
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
 * @brief Sets the m_ExperimentRunningFlag false if experiment is finished and the stages stopped and record preload distance if a preloading happend.
 */
void MyFrame::checkFinishedExperiment(){
  {
    // Wait until experiment is finised.
    std::unique_lock<std::mutex> lck1(m_WaitMutex);
    m_Wait.wait(lck1);
  }
  {
    std::lock_guard<std::mutex> lck4{m_ExperimentRunningMutex};
    m_ExperimentRunningFlag = false;
  }
  {
    std::lock_guard<std::mutex> lck2{m_PreloadDoneMutex};
    if(false == m_PreloadDoneFlag){
      // Wait until the stages stopped.
      {
        bool tmp = false;
        while(false == tmp){
          std::unique_lock<std::mutex> lck3(m_StagesStoppedMutex);
          tmp = m_StagesStoppedFlag;
        }
      }

      m_PreloadDoneFlag = true;
      m_PreloadDistance = m_CurrentDistance;
      std::cout << "m_PreloadDistance: " << m_PreloadDistance << std::endl;
    }
  }
}
