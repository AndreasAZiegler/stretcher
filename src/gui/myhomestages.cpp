#include <wx/wx.h>
#include "myhomestages.h"

wxBEGIN_EVENT_TABLE(MyHomeStages, MyHomeStages_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_HomeStagesOK, MyHomeStages::OnOK)
wxEND_EVENT_TABLE()

MyHomeStages::MyHomeStages(std::vector<LinearStage *> *linearstages, wxWindow *parent)
  : MyHomeStages_Base(parent),
    m_LinearStages(linearstages)
{
  wxID_HomeStagesOK->SetId(ID_HomeStagesOK);
  //m_FileOutputNameDirPicker->SetPath(m_Path);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyHomeStages::OnOK(wxCommandEvent &event){
  (m_LinearStages->at(0))->home();
  (m_LinearStages->at(1))->home();

  Close(true);
}
