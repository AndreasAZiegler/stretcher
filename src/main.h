/**
 * @file main.h
 * @brief The main file.
 * @author Andreas Ziegler
 */

#ifndef MAIN_H
#define MAIN_H

/*************** Includes ************/
#include <thread>
#include "wx/app.h"
#include "./gui/myframe.h"
#include "settings.h"
#include "./hardware/linearstage.h"
#include "./hardware/linearstagemessagehandler.h"
#include "./hardware/stageframe.h"
#include "./hardware/forcesensor.h"
#include "./hardware/forcesensormessagehandler.h"

/**
 * @class MyApp main.h "main.h"
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
    Settings m_MySettings;																									/**< Settings object (Reads settings from "config.cfg" at start and stores at exit */
    StageFrame m_StageFrame;																								/**< StageFrame object */

    std::vector<std::thread> m_LinearStagesReceivers;												/**< Vector containing the threads for the linear stage receivers */
    std::thread m_ForceSensorReceiver;																			/**< Thread for the force sensor receiver */

};

#endif // MAIN_H
