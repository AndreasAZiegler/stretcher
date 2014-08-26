/**
 * @file myhomestages.cpp
 * @brief The home stages dialog.
 * @author Andreas Ziegler
 */

// Includes
#include <wx/wx.h>
#include "myhomestages.h"

// The event table.
wxBEGIN_EVENT_TABLE(MyHomeStages, MyHomeStages_Base)
  EVT_BUTTON(ID_HomeStagesOK, MyHomeStages::OnOK)
wxEND_EVENT_TABLE()

/**
 * @brief Initializes all the needed variables.
 * @param stageframe Pointer to the stage frame object.
 * @param myframe Pointer to the main frame object.
 * @param parent Pointer to the parent window.
 */
MyHomeStages::MyHomeStages(std::shared_ptr<StageFrame> stageframe, MyFrame *myframe, wxWindow *parent)
  : MyHomeStages_Base(parent),
    m_StageFrame(stageframe),
    m_MyFrame(myframe)
{
  wxID_HomeStagesOK->SetId(ID_HomeStagesOK);
  //m_FileOutputNameDirPicker->SetPath(m_Path);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button. Resets the distance limits and homes the stage frame.
 * @param event Occuring event
 */
void MyHomeStages::OnOK(wxCommandEvent &event){
  // Resets the distance limit.
  m_StageFrame->setMaxDistanceLimitMM(153);
  m_StageFrame->setMinDistanceLimitMM(0);
  m_MyFrame->setMaxDistanceLimit(153 / 0.00009921875/*mm per micro step*/);

  // Homes the stage frame.
  m_StageFrame->home();

  Close(true);
}
