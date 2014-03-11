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
    void OnSamplingFrequencySettings(wxCommandEvent& event);
    void OnPortsSettings(wxCommandEvent& event);
    void OnFileOutputSettings(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_SamplingFrequency = 1,
    ID_Ports = 2,
    ID_FileOutput = 3
};

#endif // MYFRAME_H
