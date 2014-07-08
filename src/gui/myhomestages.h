#ifndef MYHOMESTAGES_H
#define MYHOMESTAGES_H

// Includes
#include <vector>
#include "myhomestages_base.h"
#include "myframe.h"
#include "../hardware/stageframe.h"

class MyHomeStages : public MyHomeStages_Base
{
  public:
    MyHomeStages(std::shared_ptr<StageFrame> stageframe, MyFrame *myframe, wxWindow *parent);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    std::shared_ptr<StageFrame> m_StageFrame;
    MyFrame *m_MyFrame;

    wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_HomeStagesOK = 57
};

#endif // MYHOMESTAGES_H
