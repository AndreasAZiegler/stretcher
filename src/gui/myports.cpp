#include <wx/wx.h>
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include "myports.h"

wxBEGIN_EVENT_TABLE(MyPorts, MyPorts_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_PortsOK, MyPorts::OnOK)
wxEND_EVENT_TABLE()


/**
 * @brief Creates the window for the com port settings
 * @param parent Pointer to the parent object.
 * @param settings Pointer to the Settings object.
 */
MyPorts::MyPorts(Settings *settings, std::vector<LinearStage *> *linearstage, ForceSensor *forcesensor, wxWindow *parent)
  : MyPorts_Base(parent),
    m_Settings(settings),
    m_LinearStages(linearstage),
    m_ForceSensor(forcesensor)
{
  wxID_PortsOK->SetId(ID_PortsOK);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyPorts::OnOK(wxCommandEvent &event){
   m_Settings->setLinMot1ComPort(choiceToString(m_PortsMotor1Choice->GetSelection()));
   m_Settings->setLinMot2ComPort(choiceToString(m_PortsMotor2Choice->GetSelection()));
   m_Settings->setForceSensorComPort(choiceToString(m_PortsForceChoice->GetSelection()));

   if(NULL != m_LinearStages){
     m_LinearStages->at(0)->disconnect();
     m_LinearStages->at(1)->disconnect();

     m_LinearStages->at(0)->connect(m_Settings->getLinMot1ComPort());
     m_LinearStages->at(1)->connect(m_Settings->getLinMot2ComPort());
   }
   if(NULL != m_ForceSensor){
     m_ForceSensor->disconnect();
     m_ForceSensor->connect(m_Settings->getForceSensorComPort());
   }

   Close(true);

}

std::string MyPorts::choiceToString(int i){
  switch(i){
    case 0:
      return(std::string("/dev/ttyUSB0"));
      break;
    case 1:
      return(std::string("/dev/ttyUSB1"));
      break;
    case 2:
      return(std::string("/dev/ttyUSB2"));
      break;
    case 3:
      return(std::string("/dev/ttyUSB3"));
      break;
    case 4:
      return(std::string("/dev/ttyUSB4"));
      break;
  }
}
