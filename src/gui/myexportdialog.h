/**
 * @file myexportdialog.h
 * @brief Dialog for the csv export. Let the user choose a directory and a filename and mute some experiments if desired.
 * @author Andreas Ziegler
 */

#ifndef MYEXPORTDIALOG_H
#define MYEXPORTDIALOG_H

// Includes
#include <vector>
#include "myexportdialog_base.h"
#include "protocols.h"

/**
 * @class MyExportDialog myexportdialog.h "gui/myexportdialog.h"
 * @brief Export dialog for the export of the measured data in to a .csv file.
 */
class MyExportDialog : public MyExportDialog_Base
{
  public:

    /**
     * @brief Initialize all the required parameters, Set the ID of the OK button, generate a default path and file name and add the experiment names to the wxListBox.
     * @param protocols Pointer to the protocol object.
     * @param stringvector Vector containing the names of the experiments.
     * @param storagepath The default storage path.
     */
    MyExportDialog(std::shared_ptr<Protocols> &protocols, std::vector<std::string> stringvector, std::string storagepath);

  private:

    /**
     * @brief Return the selected experiments for the export as a boolean vector.
     * @return The selected experiments for the export as a boolean vector.
     */
    std::vector<bool> getSelection(void);

    /**
     * @brief Method wich will be executed, when the user clicks on the OK button.
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

/**
 * @enum ID for events in MyExportDialog
 */
enum
{
  ID_ExportDialogOK = 59
};

#endif // MYEXPORTDIALOG_H
