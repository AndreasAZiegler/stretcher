#ifndef MYSTARTUPDIALOG_H
#define MYSTARTUPDIALOG_H

// Includes
#include "mystartupdialog_base.h"

class MyStartUpDialog : public MyStartUpDialog_Base
{
  public:
    MyStartUpDialog(MyFrame *myframe);

  private:

    /**
     * @brief Method wich will be executed, when the user clicks on the Set button.
     * @param event Occuring event
     */
    void OnSet(wxCommandEvent& event);

    MyFrame *m_MyFrame;

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_Set = 56
};

#endif // MYSTARTUPDIALOG_H
