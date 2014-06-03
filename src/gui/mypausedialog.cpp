// Includes
#include "mypausedialog.h"

wxBEGIN_EVENT_TABLE(MyPauseDialog, MyPauseDialog_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_PauseDialogOK, MyPauseDialog::OnOK)
wxEND_EVENT_TABLE()

MyPauseDialog::MyPauseDialog(Pause *pauseExperiment)
  : m_PauseExperiment(pauseExperiment)
{
}


/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyPauseDialog::OnOK(wxCommandEvent &event){
  m_PauseExperiment->setPauseTime(m_PauseDialogWaitingTimeSpinCtrl->GetValue());

  Close(true);
}
