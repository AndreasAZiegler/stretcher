#ifndef MYFRAME_H
#define MYFRAME_H

#include "resource.h"

class MyFrame : public MyFrame_Base
{
  public:
    MyFrame(const wxString& title, wxWindow* parent = (wxWindow *)NULL);
  private:
    void OnExit(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // MYFRAME_H
