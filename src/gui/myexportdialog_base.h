//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __myexportdialog_base_h__
#define __myexportdialog_base_h__

// Includes
#include <wx/dialog.h>
#include <wx/xrc/xmlres.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/listbox.h>

class MyExportDialog_Base : public wxDialog {
protected:
 wxStaticText* m_ExportDialog1StaticText;
 wxFilePickerCtrl* m_ExportDialogFilePicker;
 wxStaticText* m_ExportDialog2StaticText;
 wxListBox* m_ExportDialogListBox;
 wxButton* wxID_ExportDialogOK;
 wxButton* wxID_ExportDialogCANCEL;

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MyExportDialog_Base"), wxT("wxDialog"));
  m_ExportDialog1StaticText = XRCCTRL(*this,"m_ExportDialog1StaticText",wxStaticText);
  m_ExportDialogFilePicker = XRCCTRL(*this,"m_ExportDialogFilePicker",wxFilePickerCtrl);
  m_ExportDialog2StaticText = XRCCTRL(*this,"m_ExportDialog2StaticText",wxStaticText);
  m_ExportDialogListBox = XRCCTRL(*this,"m_ExportDialogListBox",wxListBox);
  wxID_ExportDialogOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_ExportDialogCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
 }
public:
MyExportDialog_Base(wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
