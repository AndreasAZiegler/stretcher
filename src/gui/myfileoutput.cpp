#include <wx/wx.h>
#include "myfileoutput.h"

wxBEGIN_EVENT_TABLE(MyFileOutput, MyFileOutput_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_FileOutputOK, MyFileOutput::OnOK)
wxEND_EVENT_TABLE()

MyFileOutput::MyFileOutput(MyFrame *myframe, Settings *settings, std::string path, wxWindow *parent)
  : MyFileOutput_Base(parent),
    m_MyFrame(myframe),
    m_Settings(settings),
    m_Path(path)
{
  wxID_FileOutputOK->SetId(ID_FileOutputOK);
  //m_FileOutputNameDirPicker->SetPath(m_Path);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyFileOutput::OnOK(wxCommandEvent &event){
  //const wxScopedCharBuffer p(m_FileOutputNameDirPicker->GetPath());
  //const char *  p = m_FileOutputNameDirPicker->GetPath().ToUTF8();
  std::cout << m_FileOutputNameDirPicker->GetPath().ToStdString() << std::endl;
  m_Settings->setStoragePath(m_FileOutputNameDirPicker->GetPath().wc_str());
  m_MyFrame->updateStoragePath(m_FileOutputNameDirPicker->GetPath().ToStdString());

  Close(true);
}
