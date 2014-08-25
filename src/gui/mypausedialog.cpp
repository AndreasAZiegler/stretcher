/**
 * @file mypausedialog.cpp
 * @brief The pause dialog.
 * @author Andreas Ziegler
 */

// Includes
#include "mypausedialog.h"

// The event table.
wxBEGIN_EVENT_TABLE(MyPauseDialog, MyPauseDialog_Base)
  EVT_BUTTON(ID_PauseDialogOK, MyPauseDialog::OnOK)
wxEND_EVENT_TABLE()

MyPauseDialog::MyPauseDialog(Pause *pauseExperiment, double pausetime)
  : m_PauseExperiment(pauseExperiment),
    m_CreateExperimentFlag(false)
{
  wxID_PauseDialogOK->SetId(ID_PauseDialogOK);
  m_PauseDialogWaitingTimeSpinCtrl->SetDigits(2);
  m_PauseDialogWaitingTimeSpinCtrl->SetValue(pausetime);
}


/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyPauseDialog::OnOK(wxCommandEvent &event){
  m_PauseExperiment->setPauseTime(m_PauseDialogWaitingTimeSpinCtrl->GetValue());
  m_CreateExperimentFlag = true;

  Close(true);
}

/**
 * @brief Returns the create exeriment flag.
 * @return The create exeriment flag.
 */
bool MyPauseDialog::getCreateExperimentFlag(void){
 return(m_CreateExperimentFlag);
}
