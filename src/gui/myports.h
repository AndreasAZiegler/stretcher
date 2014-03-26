#ifndef MYPORTS_H
#define MYPORTS_H

/*************** Includes ************/
#include <wx/window.h>
#include <wx/string.h>
#include "myports_base.h"

class MyPorts : public MyPorts_Base
{
  public:
    MyPorts(wxWindow *parent=NULL);
  private:
    void OnOK(wxCommandEvent& event);


    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_PortsOK = 10,
};

#endif // MYPORTS_H
