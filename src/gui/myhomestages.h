#ifndef MYHOMESTAGES_H
#define MYHOMESTAGES_H

// Includes
#include <vector>
#include "myhomestages_base.h"
#include "../hardware/linearstage.h"

class MyHomeStages : public MyHomeStages_Base
{
  public:
    MyHomeStages(std::vector<LinearStage *> *linearstages, wxWindow *parent);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    std::vector<LinearStage *> *m_LinearStages;

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_HomeStagesOK = 52
};

#endif // MYHOMESTAGES_H
