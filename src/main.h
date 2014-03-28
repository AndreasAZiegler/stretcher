#ifndef MAIN_H
#define MAIN_H

/*************** Includes ************/
#include "wx/app.h"
#include "./gui/myframe.h"
#include "settings.h"
#include "./hardware/linearstage.h"

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
};

#endif // MAIN_H
