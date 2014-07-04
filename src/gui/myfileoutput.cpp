/**
 * @file myfileoutput.cpp
 * @brief Dialog to set the default path.
 * @author Andreas Ziegler
 */

// Includes
#include <wx/wx.h>
#include "myfileoutput.h"

// Event table.
wxBEGIN_EVENT_TABLE(MyFileOutput, MyFileOutput_Base)
  EVT_BUTTON(ID_FileOutputOK, MyFileOutput::OnOK)
wxEND_EVENT_TABLE()

/**
 * @brief Initialize all the required parameters and sets the ID of the OK button.
 * @param myframe Pointer to the main frame object.
 * @param settings Pointer to the settings object.
 * @param path Default path.
 * @param parent Pointer to the wxWindow parent.
 */
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
 * @brief Method wich will be executed, when the user clicks on the OK button. Saves the default path in the configuration file and updates it in the main frame.
 * @param event Occuring event
 */
void MyFileOutput::OnOK(wxCommandEvent &event){
  wxLogMessage(m_FileOutputNameDirPicker->GetPath().ToStdString().c_str());
  m_Settings->setStoragePath(m_FileOutputNameDirPicker->GetPath().ToStdString());
  m_MyFrame->updateStoragePath(m_FileOutputNameDirPicker->GetPath().ToStdString());

  Close(true);
}
