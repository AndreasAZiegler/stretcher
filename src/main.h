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
    MyFrame* m_MyFrame;
    Settings m_MySettings;
    std::vector<LinearStage*> m_LinearStages;
    std::vector<LinearStageMessageHandler*> m_LinearStagesMessageHandlers;

    std::thread m_LinearStage1Receiver;
    std::thread m_LinearStage2Receiver;
};

#endif // MAIN_H
