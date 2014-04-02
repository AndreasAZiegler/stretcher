#ifndef MAIN_H
#define MAIN_H

/*************** Includes ************/
#include <thread>
#include "wx/app.h"
#include "./gui/myframe.h"
#include "settings.h"
#include "./hardware/linearstage.h"
#include "./hardware/linearstagemessagehandler.h"

/**
 * @brief The MyApp class
 */
class MyApp : public wxApp
{
  public:
    /**
     * @brief Initialization
     * @return true if successful
     */
    virtual bool OnInit();

  private:
    MyFrame* m_MyFrame;																											/**< Pointer to the main frame object */
    Settings m_MySettings;																									/**< Settings object (Reads settings from "config.cfg" at start and stores at exit */
    std::vector<LinearStage*> m_LinearStages;																/**< Vector containing the two linear stage object pointer */
    std::vector<LinearStageMessageHandler*> m_LinearStagesMessageHandlers;	/**< Vector containing the two linear stage message handler object pointer */

    std::vector<std::thread> m_LinearStagesReceivers;												/**< Vector containing the threads for the linear stage receivers */
};

#endif // MAIN_H
