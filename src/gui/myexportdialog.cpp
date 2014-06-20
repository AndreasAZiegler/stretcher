// Includes
#include "myexportdialog.h"

wxBEGIN_EVENT_TABLE(MyExportDialog, MyExportDialog_Base)
//  EVT_MENU(wxID_OK, MyPorts::OnOK)
  EVT_BUTTON(ID_ExportDialogOK, MyExportDialog::OnOK)
wxEND_EVENT_TABLE()

MyExportDialog::MyExportDialog(std::shared_ptr<Protocols> &protocols, std::vector<std::string> stringvector, std::string storagepath)
  : m_CurrentProtocol(protocols),
    m_PathAndFileName("")
{
  wxID_ExportDialogOK->SetId(ID_ExportDialogOK);

  // Creating file name
  std::time_t time = std::time(NULL);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%Y%m%d_%H:%M:%S", std::localtime(&time));
  //std::cout << mbstr << std::endl;
  //std::string pathAndFilename = m_StoragePath + "/" + experimentTypeToString() + "_" + std::string(mbstr) + ".txt";
  std::string pathAndFilename = storagepath + "/" + "Protocol_" + std::string(mbstr) + ".txt";

  //m_ExportDialogFilePicker->SetInitialDirectory(storagepath);
  m_ExportDialogFilePicker->SetPath(pathAndFilename);
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
  m_CurrentProtocol->exportCSV(getSelection(), m_ExportDialogFilePicker->GetPath().ToStdString());

  Close(true);
}

/**
 * @brief Method wich will be executed, when the user has choosen a path and a file name.
 * @param event Occuring event
 */
void MyExportDialog::OnChangePathAndName(wxFileDirPickerEvent& event){
  m_PathAndFileName = m_ExportDialogFilePicker->GetPath();
}
