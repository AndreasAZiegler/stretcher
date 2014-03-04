#include <wx/wx.h>
#include <wx/image.h>
#include "myframe.h"

//-----------------------------------------------------------------------------
// Regular resources (the non-XRC kind).
//-----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "sample.xpm"
#endif

wxBEGIN_EVENT_TABLE(MyFrame, MyFrame_Base)
  EVT_MENU(wxID_EXIT,	MyFrame::OnExit)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title, wxWindow *parent)
  : MyFrame_Base(title)
{
  SetIcon(wxICON(sample));
}

void MyFrame::OnExit(wxCommandEvent& event){
  Close(true);
}
