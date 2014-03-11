#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/image.h>
#include "myframe.h"
#include "mysamplingfrequency_base.h"
#include "myports_base.h"
#include "myfileoutput_base.h"

//-----------------------------------------------------------------------------
// Regular resources (the non-XRC kind).
//-----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

wxBEGIN_EVENT_TABLE(MyFrame, MyFrame_Base)
  EVT_MENU(wxID_EXIT,	MyFrame::OnExit)
  EVT_MENU(XRCID("m_SamplingFrequencyMenuItem"), MyFrame::OnSamplingFrequencySettings)
  EVT_MENU(XRCID("m_PortsMenuMenuItem"), MyFrame::OnPortsSettings)
  EVT_MENU(XRCID("m_FileOutputMenuItem"), MyFrame::OnFileOutputSettings)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title, wxWindow *parent)
  : MyFrame_Base(title, parent)
{
  SetIcon(wxICON(sample));

}

void MyFrame::OnExit(wxCommandEvent& event){
  Close(true);
}

void MyFrame::OnSamplingFrequencySettings(wxCommandEvent& event){
  MySamplingFrequency_Base *samplingFrequency = new MySamplingFrequency_Base(this);
  samplingFrequency->Show();
}

void MyFrame::OnPortsSettings(wxCommandEvent& event){
	MyPorts_Base *ports = new MyPorts_Base(this);
	ports->Show();
}

void MyFrame::OnFileOutputSettings(wxCommandEvent& event){
	MyFileOutput_Base *fileOutput = new MyFileOutput_Base(this);
	fileOutput->Show();
}
