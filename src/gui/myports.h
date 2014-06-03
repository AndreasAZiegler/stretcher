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
    MyPorts(Settings *settings, std::vector<std::shared_ptr<LinearStage>> &linearstage, std::shared_ptr<ForceSensor> forcesensor, wxWindow *parent=NULL);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    std::string choiceToString(int i);

    Settings *m_Settings;												/**< Pointer to the Settings object. */
    std::vector<std::shared_ptr<LinearStage>> m_LinearStages;	/**< Vector containing the pointers to the linear stage objects */
    std::shared_ptr<ForceSensor> m_ForceSensor;									/**< Pointer to the force sensor object */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_PortsOK = 61
};

#endif // MYPORTS_H
