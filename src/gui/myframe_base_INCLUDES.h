// Includes
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "mybutton.h"
#include <wx/spinctrl.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/scrolwin.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <mathplot.h>
#include <wx/xrc/xmlres.h>

 wxMyButton* m_DecreaseDistanceButton;
 wxMyButton* m_IncreaseDistanceButton;

wxBoxSizer* m_GraphSizer1;
wxBoxSizer* m_GraphSizer2;

m_GraphSizer2 = static_cast<wxBoxSizer*>(m_GraphExportCSVButton->GetContainingSizer());
m_GraphSizer1 = static_cast<wxBoxSizer*>(m_GraphPanel->GetSizer());

MyFrame_Base(const wxString& title, wxWindow *parent=NULL){




 wxButton* wxID_PreloadOK;
 wxButton* wxID_PreloadCANCEL;

 wxButton* wxID_ConditioningOK;
 wxButton* wxID_ConditioningCANCEL;

 wxButton* wxID_R2FOK;
 wxButton* wxID_R2FCANCEL;

 wxButton* wxID_RelaxationOK;
 wxButton* wxID_RelaxationCANCEL;

 wxButton* wxID_CreepOK;
 wxButton* wxID_CreepCANCEL;

 wxButton* wxID_FatigueOK;
 wxButton* wxID_FatigueCANCEL;

 wxButton* wxID_ChamberStretchOK;
 wxButton* wxID_ChamberStretchCANCEL;

  wxID_PreloadOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_PreloadCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_ConditioningOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_ConditioningCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_R2FOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_R2FCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_RelaxationOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_RelaxationCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_CreepOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_CreepCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_FatigueOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_FatigueCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);

  wxID_ChamberStretchOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_ChamberStretchCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
