#ifndef MYEXPORTDIALOG_H
#define MYEXPORTDIALOG_H

// Includes
#include <vector>
#include "myexportdialog_base.h"
#include "protocols.h"

class MyExportDialog : public MyExportDialog_Base
{
  public:
    MyExportDialog(std::shared_ptr<Protocols> &protocols, std::vector<std::string> stringvector);

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

    std::shared_ptr<Protocols> m_CurrentProtocol;								/**< Pointer to the current protocol */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_ExportDialogOK = 51
};

#endif // MYEXPORTDIALOG_H
