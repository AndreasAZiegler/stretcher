//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __mypausedialog_base_h__
#define __mypausedialog_base_h__

// Include
#include <wx/xrc/xmlres.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/spinctrl.h>

class MyPauseDialog_Base : public wxDialog {
protected:
 wxStaticText* m_staticText69;
 wxSpinCtrlDouble* m_PauseDialogWaitingTimeSpinCtrl;
 wxButton* wxID_PauseDialogOK;
 wxButton* wxID_PauseDialogCANCEL;

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MyPauseDialog_Base"), wxT("wxDialog"));
  m_staticText69 = XRCCTRL(*this,"m_staticText69",wxStaticText);
  m_PauseDialogWaitingTimeSpinCtrl = XRCCTRL(*this,"m_PauseDialogWaitingTimeSpinCtrl",wxSpinCtrlDouble);
  wxID_PauseDialogOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_PauseDialogCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
 }
public:
MyPauseDialog_Base(wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
