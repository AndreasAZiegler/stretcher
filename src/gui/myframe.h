#ifndef MYFRAME_H
#define MYFRAME_H

/*************** Includes ************/
#include <wx/window.h>
#include <wx/string.h>
#include "myframe_base.h"

/**
 * @brief The Main Frame class
 */
class MyFrame : public MyFrame_Base
{
  public:
    /**
     * @brief Constructor of the main frame. Sets the icon.
     * @param title Title of the software.
     * @param parent Pointer to the parent object.
     */
    MyFrame(const wxString& title, wxWindow* parent = (wxWindow *)NULL);

    void startup(void);
  private:
    /**
     * @brief Method wich will be executed, when the software will be closed by the user.
     * @param event Occuring event
     */
    void OnExit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the frequency settings.
     * @param event Occuring event
     */
    void OnSamplingFrequencySettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the port settings.
     * @param event Occuring event
     */
    void OnPortsSettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the file output settings.
     * @param event Occuring event
     */
    void OnFileOutputSettings(wxCommandEvent& event);

    void OnCalculateDiameter(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_SamplingFrequency = 1,
	ID_Ports = 2,
	ID_FileOutput = 3,
	ID_CalculateDiameter = 44
};

#endif // MYFRAME_H
