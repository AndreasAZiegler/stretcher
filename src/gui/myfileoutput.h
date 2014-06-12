#ifndef MYFILEOUTPUT_H
#define MYFILEOUTPUT_H

// Includes
#include <wx/window.h>
#include "myfileoutput_base.h"
#include "../settings.h"
#include "myframe.h"

/**
 * @brief Handles the file output name window.
 */
class MyFileOutput : public MyFileOutput_Base
{
  public:
    MyFileOutput(MyFrame *myframe, Settings *settings, std::string path, wxWindow *parent=NULL);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    MyFrame *m_MyFrame;													/**< Pointer to the MyFrame object. */
    Settings *m_Settings;												/**< Pointer to the Settings object. */

    std::string m_Path;													/**< Path to the file as a std::string */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_FileOutputOK = 52
};

#endif // MYFILEOUTPUT_H
