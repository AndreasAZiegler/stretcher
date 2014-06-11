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

    /**
     * @brief Returns the create exeriment flag.
     * @return The create exeriment flag.
     */
    bool getCreateExperimentFlag(void);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    Pause *m_PauseExperiment;										/**< Pointer to the pause experiment. */
    bool m_CreateExperimentFlag;								/**< Indicates if the pause experiment should be created. */

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_PauseDialogOK = 53
};

#endif // MYPAUSEDIALOG_H
