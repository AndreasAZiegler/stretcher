#ifndef MYPAUSEDIALOG_H
#define MYPAUSEDIALOG_H

// Includes
#include <memory>
#include "mypausedialog_base.h"
#include "../experiments/pause.h"

class MyPauseDialog : public MyPauseDialog_Base
{
  public:
    MyPauseDialog(Pause *pauseExperiment);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    Pause *m_PauseExperiment;

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_PauseDialogOK = 60
};

#endif // MYPAUSEDIALOG_H
