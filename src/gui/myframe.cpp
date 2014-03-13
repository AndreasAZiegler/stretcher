/*************** Includes ************/
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/image.h>
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

	m_ClampingPositionSpinCtrl->SetValue(m_ClampingPositionSpinCtrl->GetValue() + " mm");

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
