// Includes
#include "myexportdialog.h"

wxBEGIN_EVENT_TABLE(MyExportDialog, MyExportDialog_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_ExportDialogOK, MyExportDialog::OnOK)
wxEND_EVENT_TABLE()

MyExportDialog::MyExportDialog(std::shared_ptr<Protocols> &protocols, std::vector<std::string> stringvector)
  : m_CurrentProtocol(protocols)
{
  wxID_ExportDialogOK->SetId(ID_ExportDialogOK);
  for(int i = 0; i < stringvector.size(); ++i){
    m_ExportDialogListBox->Append(stringvector[i]);
  }
}

/**
 * @brief Return the selected experiments for the export as a boolean vector.
 * @return The selected experiments for the export as a boolean vector.
 */
std::vector<bool> MyExportDialog::getSelection(void){
  std::vector<bool> selection;

  for(int i = 0; i < m_ExportDialogListBox->GetCount(); ++i){
    selection.push_back(m_ExportDialogListBox->IsSelected(i));
  }

  return(selection);
}

/**
 * @brief Method wich will be executed, when the user klicks on the OK button.
 * @param event Occuring event
 */
void MyExportDialog::OnOK(wxCommandEvent& event){
  m_CurrentProtocol->exportCSV(getSelection());

  Close(true);
}