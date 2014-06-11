// Includes
#include "mypausedialog.h"

wxBEGIN_EVENT_TABLE(MyPauseDialog, MyPauseDialog_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_PauseDialogOK, MyPauseDialog::OnOK)
wxEND_EVENT_TABLE()

MyPauseDialog::MyPauseDialog(Pause *pauseExperiment)
  : m_PauseExperiment(pauseExperiment),
    m_CreateExperimentFlag(false)
{
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
