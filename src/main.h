#ifndef MAIN_H
#define MAIN_H

/*************** Includes ************/
#include "wx/app.h"

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
};

#endif // MAIN_H
