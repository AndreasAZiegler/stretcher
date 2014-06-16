// Includes
#include "mystartupdialog.h"
#include "myframe.h"

wxBEGIN_EVENT_TABLE(MyStartUpDialog, MyStartUpDialog_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_Set, MyStartUpDialog::OnSet)
wxEND_EVENT_TABLE()

// Costum event definitions
/*
wxDEFINE_EVENT(EVT_MYBUTTON_DOWN, wxCommandEvent);
wxDEFINE_EVENT(EVT_MYBUTTON_UP, wxCommandEvent);
*/

MyStartUpDialog::MyStartUpDialog(MyFrame *myframe)
  : m_MyFrame(myframe)
{
  m_StartUpDecreaseDistanceButton->SetId(ID_MotorDecreaseDistance);
  m_StartUpIncreaseDistanceButton->SetId(ID_MotorIncreaseDistance);
  m_StartUpSetButton->SetId(ID_Set);

  m_StartUpInputSpinCtrl->SetDigits(2);

  // Register the main frame at the two custom buttons
  m_StartUpDecreaseDistanceButton->registerMyFrame(m_MyFrame);
  m_StartUpIncreaseDistanceButton->registerMyFrame(m_MyFrame);
}

/**
 * @brief Method wich will be executed, when the user clicks on the Set button.
 * @param event Occuring event
 */
void MyStartUpDialog::OnSet(wxCommandEvent& event){
  m_MyFrame->setDistanceWActuatorCollision(m_StartUpInputSpinCtrl->GetValue());
}
