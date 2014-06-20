#ifndef MYEXPORTDIALOG_H
#define MYEXPORTDIALOG_H

// Includes
#include <vector>
#include "myexportdialog_base.h"
#include "protocols.h"

class MyExportDialog : public MyExportDialog_Base
{
  public:
    MyExportDialog(std::shared_ptr<Protocols> &protocols, std::vector<std::string> stringvector, std::string storagepath);

  private:

    /**
     * @brief Return the selected experiments for the export as a boolean vector.
     * @return The selected experiments for the export as a boolean vector.
     */
    std::vector<bool> getSelection(void);

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user has choosen a path and a file name.
     * @param event Occuring event
     */
    void OnChangePathAndName(wxFileDirPickerEvent& event);

    std::shared_ptr<Protocols> m_CurrentProtocol;								/**< Pointer to the current protocol */
    std::string m_PathAndFileName;															/**< Choosen path and file name as std::string. */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_ExportDialogOK = 54
};

#endif // MYEXPORTDIALOG_H
