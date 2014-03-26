#include <wx/wx.h>
#include "myports.h"

wxBEGIN_EVENT_TABLE(MyPorts, MyPorts_Base)
  EVT_MENU(wxID_OK, MyPorts::OnOK)
wxEND_EVENT_TABLE()


MyPorts::MyPorts(wxWindow *parent)
  : MyPorts_Base(parent)
{
  wxID_PortsOK->SetId(ID_PortsOK);
}

void MyPorts::OnOK(wxCommandEvent &event){
  /*
   * settings->setLinMot1ComPort(m_PortsMotor1Choice->GetSelection());
   * settings->setLinMot2ComPort(m_PortsMotor2Choice->GetSelection());
   * settings->setForceSensorComPort(m_PortsForceChoice->GetSelection());
   *
   * m_LinMot1->disconnect();
   * m_LinMot2->disconnect();
   * m_ForceSensor->disconnect();
   *
   * m_LinMot1->connect(settings->getLinMot1ComPort());
   * m_LinMot2->connect(settings->getLinMot2ComPort());
   * m_ForceSensor->connect(settings->getForceSensorComPort());
   */

}
