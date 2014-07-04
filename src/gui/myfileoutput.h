/**
 * @file myfileoutput.h
 * @brief Dialog to set the default path.
 * @author Andreas Ziegler
 */

#ifndef MYFILEOUTPUT_H
#define MYFILEOUTPUT_H

// Includes
#include <wx/window.h>
#include "myfileoutput_base.h"
#include "../settings.h"
#include "myframe.h"

/**
 * @class MyFileOutput myfileoutput.h "experiments/myfileoutput.h"
 * @brief Let the user choose the default path for outputs.
 */
class MyFileOutput : public MyFileOutput_Base
{
  public:

    /**
     * @brief Initialize all the required parameters and sets the ID of the OK button.
     * @param myframe Pointer to the main frame object.
     * @param settings Pointer to the settings object.
     * @param path Default path.
     * @param parent Pointer to the wxWindow parent.
     */
    MyFileOutput(MyFrame *myframe, Settings *settings, std::string path, wxWindow *parent=NULL);

  private:

    /**
     * @brief Method wich will be executed, when the user clicks on the OK button. Saves the default path in the configuration file and updates it in the main frame.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    MyFrame *m_MyFrame;													/**< Pointer to the MyFrame object. */
    Settings *m_Settings;												/**< Pointer to the Settings object. */

    std::string m_Path;													/**< Path to the file as a std::string */

    wxDECLARE_EVENT_TABLE();
};

/**
 * @enum ID for events in MyFileOutput
 */
enum
{
  ID_FileOutputOK = 56
};

#endif // MYFILEOUTPUT_H
