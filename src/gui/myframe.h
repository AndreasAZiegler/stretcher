#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/window.h>
#include <wx/string.h>
#include "myframe_base.h"

class MyFrame : public MyFrame_Base
{
  public:
    MyFrame(const wxString& title, wxWindow* parent = (wxWindow *)NULL);
  private:
    void OnExit(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // MYFRAME_H
