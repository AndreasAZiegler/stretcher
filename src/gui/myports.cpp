/**
 * @file myports.cpp
 * @brief The port settings dialog.
 * @author Andreas Ziegler
 */

// Includes
#include <wx/wx.h>
#include <wx-3.0/wx/ctb-0.13/serport.h>
#include "myports.h"

// The event table
wxBEGIN_EVENT_TABLE(MyPorts, MyPorts_Base)
  EVT_BUTTON(ID_PortsOK, MyPorts::OnOK)
wxEND_EVENT_TABLE()


/**
 * @brief Creates the window for the com port settings and initializes all the needed variables.
 * @param settings Pointer to the Settings object.
 * @param linearstage Vector containing the linear stages.
 * @param forcesensor Pointer to the force sensor object.
 * @param parent Pointer to the parent object.
 */
MyPorts::MyPorts(Settings *settings, std::vector<std::shared_ptr<LinearStage> > &linearstage, std::shared_ptr<ForceSensor> forcesensor, wxWindow *parent)
  : MyPorts_Base(parent),
    m_Settings(settings),
    m_ForceSensor(forcesensor)
{
  m_LinearStages.push_back(linearstage.at(0));
  m_LinearStages.push_back(linearstage.at(1));
  wxID_PortsOK->SetId(ID_PortsOK);
}

/**
 * @brief Method wich will be executed, when the user clicks on the OK button. Sets the linear stages com port settings, disconnects the linear stages and connects again.
 * 				The same for ghe force sensor com port settings.
 * @param event Occuring event
 */
void MyPorts::OnOK(wxCommandEvent &event){
   m_Settings->setLinStage1ComPort(choiceToString(m_PortsMotor1Choice->GetSelection()));
   m_Settings->setLinStage2ComPort(choiceToString(m_PortsMotor2Choice->GetSelection()));
   m_Settings->setForceSensorComPort(choiceToString(m_PortsForceChoice->GetSelection()));

   if(0 != m_LinearStages.size()){
     // Disconectes the linear stages.
     m_LinearStages.at(0)->disconnect();
     m_LinearStages.at(1)->disconnect();

     // Connects the linear stages.
     m_LinearStages.at(0)->connect(m_Settings->getLinMot1ComPort());
     m_LinearStages.at(1)->connect(m_Settings->getLinMot2ComPort());
   }
   if(NULL != m_ForceSensor){
     m_ForceSensor->disconnect();
     m_ForceSensor->connect(m_Settings->getForceSensorComPort());
   }

   Close(true);

}

/**
 * @brief Translates the port number to a settings string.
 * @param i Number of the port.
 * @return  Com port setting as a string.
 */
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
