//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __mysamplingfrequency_base_h__
#define __mysamplingfrequency_base_h__
class MySamplingFrequency_Base : public wxDialog {
protected:
 wxStaticText* m_SamplingFrequencyDistanceStaticText;
 wxSpinCtrl* m_SamplingFrequencyDistanceSpinCtrl;
 wxStaticText* m_SamplingFrequencyForceStaticText;
 wxSpinCtrl* m_SamplingFrequencyForceSpinCtrl;
 wxButton* wxID_OK;
 wxButton* wxID_CANCEL;

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MySamplingFrequency_Base"), wxT("wxDialog"));
  m_SamplingFrequencyDistanceStaticText = XRCCTRL(*this,"m_SamplingFrequencyDistanceStaticText",wxStaticText);
  m_SamplingFrequencyDistanceSpinCtrl = XRCCTRL(*this,"m_SamplingFrequencyDistanceSpinCtrl",wxSpinCtrl);
  m_SamplingFrequencyForceStaticText = XRCCTRL(*this,"m_SamplingFrequencyForceStaticText",wxStaticText);
  m_SamplingFrequencyForceSpinCtrl = XRCCTRL(*this,"m_SamplingFrequencyForceSpinCtrl",wxSpinCtrl);
  wxID_OK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_CANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
 }
public:
MySamplingFrequency_Base(wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
