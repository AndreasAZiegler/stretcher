/**
 * @file mystartupdialog.h
 * @brief The start up dialog to set the distance at maximum positions.
 * @author Andreas Ziegler
 */

#ifndef MYSTARTUPDIALOG_H
#define MYSTARTUPDIALOG_H

// Includes
#include "mystartupdialog_base.h"

/**
 * @class MyStartUpDialog mystartupdialog.h "gui/mystartupdialog.h"
 * @brief The start up dialog to set the distance at maximum positions.
 */
class MyStartUpDialog : public MyStartUpDialog_Base
{
  public:
    /**
     * @brief Initializes all the needed variables.
     * @param myframe Pointer to the main frame object.
     */
    MyStartUpDialog(MyFrame *myframe);

  private:

    /**
     * @brief Method wich will be executed, when the user clicks on the Set button.
     * @param event Occuring event
     */
    void OnSet(wxCommandEvent& event);

    MyFrame *m_MyFrame;									/**< Pointer to the main frame object. */

    wxDECLARE_EVENT_TABLE();
};

/**
 * @enum ID for events in MyStartUpDialog
 */
enum
{
  ID_Set = 65
};

#endif // MYSTARTUPDIALOG_H
