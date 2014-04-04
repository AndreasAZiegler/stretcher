#ifndef MYPORTS_H
#define MYPORTS_H

/*************** Includes ************/
#include <vector>
#include <wx/window.h>
#include <wx/string.h>
#include "myports_base.h"
#include "../settings.h"
#include "../hardware/linearstage.h"
#include "../hardware/forcesensor.h"

/**
 * @brief Handles the com port settings window.
 */
class MyPorts : public MyPorts_Base
{
  public:

    /**
     * @brief Creates the window for the com port settings
     * @param parent Pointer to the parent object.
     * @param settings Pointer to the Settings object.
     */
    MyPorts(Settings *settings, std::vector<LinearStage *> *linearstage, ForceSensor *forcesensor, wxWindow *parent=NULL);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    const char* choiceToString(int i);

    Settings *m_Settings;												/**< Pointer to the Settings object. */
    std::vector<LinearStage*> *m_LinearStages;	/**< Vector containing the pointer to the linear stage objects */
    ForceSensor *m_ForceSensor;									/**< Pointer to the force sensor object */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_PortsOK = 10,
};

#endif // MYPORTS_H
