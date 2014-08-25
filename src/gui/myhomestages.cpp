#include <wx/wx.h>
#include "myhomestages.h"

wxBEGIN_EVENT_TABLE(MyHomeStages, MyHomeStages_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_HomeStagesOK, MyHomeStages::OnOK)
wxEND_EVENT_TABLE()

MyHomeStages::MyHomeStages(std::shared_ptr<StageFrame> stageframe, MyFrame *myframe, wxWindow *parent)
  : MyHomeStages_Base(parent),
    m_StageFrame(stageframe),
    m_MyFrame(myframe)
{
  wxID_HomeStagesOK->SetId(ID_HomeStagesOK);
  //m_FileOutputNameDirPicker->SetPath(m_Path);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyHomeStages::OnOK(wxCommandEvent &event){
  // Reset limit
  m_StageFrame->setMaxDistanceLimit(153);
  m_StageFrame->setMinDistanceLimit(0);
  m_MyFrame->setMaxDistanceLimit(153 / 0.00009921875/*mm per micro step*/);

  m_StageFrame->home();

  Close(true);
}
