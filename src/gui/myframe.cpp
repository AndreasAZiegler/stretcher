/*************** Includes ************/
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/image.h>
#include "../gpPanel/gpLineLayer.h"
#include "../gpPanel/gpPanel.h"
#include "myframe.h"
#include "mysamplingfrequency_base.h"
#include "myports_base.h"
#include "myfileoutput_base.h"

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
  EVT_MENU(XRCID("m_PreloadCalculateDiameterCheckBox"), MyFrame::OnCalculateDiameter)
  EVT_CHECKBOX(ID_CalculateDiameter, MyFrame::OnCalculateDiameter)
  EVT_RADIOBOX(ID_Unit, MyFrame::OnUnit)
  EVT_RADIOBUTTON(ID_DistanceLimit, MyFrame::OnDistanceLimit)
  EVT_RADIOBUTTON(ID_StressLimit, MyFrame::OnStressLimit)
  EVT_RADIOBOX(ID_GoTo, MyFrame::OnGoTo)
wxEND_EVENT_TABLE()

/**
 * @brief Constructor of the main frame. Sets the icon.
 * @param Title of the software.
 * @param Pointer to the parent object.
 */
MyFrame::MyFrame(const wxString &title, wxWindow *parent)
  : MyFrame_Base(title, parent)
{
  SetIcon(wxICON(sample));

	m_PreloadCalculateDiameterCheckBox->SetId(ID_CalculateDiameter);
}

void MyFrame::startup(void){
	// Hide calculate diameter options
	m_PreloadYStaticText->Show(false);
	m_PreloadYSpinCtrl->Show(false);
	m_PreloadXRadioBox->Show(false);
	m_PreloadXSpinCtrl->Show(false);
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
  m_PreloadSpeedSpinCtrl->SetDigits(2);
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

  // Set the required ID's
  wxString str;
  str << m_ClampingPositionSpinCtrl->GetValue();
  m_ClampingPositionSpinCtrl->SetValue(str + " mm");
	m_PreloadUnitRadioBox->SetId(ID_Unit);
  m_ConditioningStressRadioBtn->SetId(ID_StressLimit);
  m_ConditioningDistanceRadioBtn->SetId(ID_DistanceLimit);
  m_R2FAfterFailureRadioBox->SetId(ID_GoTo);

  /*
  gpPanel* graphPanel = new gpPanel(this, wxNewId(), wxDefaultPosition, wxSize(240,336));
  gpLineLayer *line = new gpLineLayer(_("my line"), _("x-label"), _("y-label"));
  m_Graph->AddChild(graphPanel);
  */

  //Push data to gpLayer
  /*
  for(int i=0;i<12;i++)
     line->DataPush( i, (rand()+1)%1024);
  */

  //add gpLayer to gpPanel
  //graphPanel->AddLayer( line,1 );

  //Finally, updated graphics data
  //line->Refresh();
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
	MyPorts_Base *ports = new MyPorts_Base(this);
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
    m_PreloadLimitStaticText->SetLabelText("Stress Limit");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Stress Limit");
    m_CreepHoldForceStressStaticText->SetLabelText("Hold Stress");
  }else{
    m_PreloadLimitStaticText->SetLabelText("Force Limit");
    m_ConditioningStressForceLimitStaticText->SetLabelText("Force Limit");
    m_CreepHoldForceStressStaticText->SetLabelText("Hold Force");
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

void MyFrame::OnGoTo(wxCommandEvent& event){
  if(2 == m_R2FAfterFailureRadioBox->GetSelection()){
    m_R2FGoToSpinCtrl->Show(true);
    m_R2FGoToRadioBox->Show(true);
  }else{
    m_R2FGoToSpinCtrl->Show(false);
    m_R2FGoToRadioBox->Show(false);
  }
}
