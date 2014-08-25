/**
 * @file mystartupdialog.cpp
 * @brief The start up dialog to set the distance at maximum positions.
 * @author Andreas Ziegler
 */

// Includes
#include "mystartupdialog.h"
#include "myframe.h"

// The event table.
wxBEGIN_EVENT_TABLE(MyStartUpDialog, MyStartUpDialog_Base)
  EVT_BUTTON(ID_Set, MyStartUpDialog::OnSet)
wxEND_EVENT_TABLE()

/**
 * @brief Initializes all the needed variables.
 * @param myframe Pointer to the main frame object.
 */
MyStartUpDialog::MyStartUpDialog(MyFrame *myframe)
  : m_MyFrame(myframe)
{
  // Sets the ID's of the objects.
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
