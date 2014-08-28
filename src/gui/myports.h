/**
 * @file myports.h
 * @brief The port settings dialog.
 * @author Andreas Ziegler
 */

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
 * @class MyPorts myports.h "gui/myports.h"
 * @brief Handles the com port settings window.
 */
class MyPorts : public MyPorts_Base
{
  public:

    /**
     * @brief Creates the window for the com port settings and initializes all the needed variables.
     * @param settings Pointer to the Settings object.
     * @param linearstage Vector containing the linear stages.
     * @param forcesensor Pointer to the force sensor object.
     * @param parent Pointer to the parent object.
     */
    MyPorts(Settings *settings, std::vector<std::shared_ptr<LinearStage>> &linearstage, std::shared_ptr<ForceSensor> forcesensor, wxWindow *parent=NULL);

  private:

    /**
     * @brief Method wich will be executed, when the user clicks on the OK button. Sets the linear stages com port settings, disconnects the linear stages and connects again.
     * 				The same for ghe force sensor com port settings.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    /**
     * @brief Translates the port number to a settings string.
     * @param i Number of the port.
     * @return  Com port setting as a string.
     */
    std::string choiceToString(int i);

    Settings *m_Settings;																			/**< Pointer to the Settings object. */
    std::vector<std::shared_ptr<LinearStage>> m_LinearStages;	/**< Vector containing the pointers to the linear stage objects */
    std::shared_ptr<ForceSensor> m_ForceSensor;								/**< Pointer to the force sensor object */

    wxDECLARE_EVENT_TABLE();
};

/**
 * @enum ID for events in MyPorts
 */
enum
{
  ID_PortsOK = 61
};

#endif // MYPORTS_H
