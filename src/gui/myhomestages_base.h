//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __myhomestages_base_h__
#define __myhomestages_base_h__

// Includes
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>

class MyHomeStages_Base : public wxDialog {
protected:
 wxStaticText* m_staticText62;
 wxButton* wxID_HomeStagesOK;
 wxButton* wxID_HomeStagesCANCEL;

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MyHomeStages_Base"), wxT("wxDialog"));
  m_staticText62 = XRCCTRL(*this,"m_staticText62",wxStaticText);
  wxID_HomeStagesOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_HomeStagesCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
 }
public:
MyHomeStages_Base(wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
