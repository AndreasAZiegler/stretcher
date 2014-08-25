/**
 * @file myhomestages.h
 * @brief The home stages dialog.
 * @author Andreas Ziegler
 */

#ifndef MYHOMESTAGES_H
#define MYHOMESTAGES_H

// Includes
#include <vector>
#include "myhomestages_base.h"
#include "myframe.h"
#include "../hardware/stageframe.h"

/**
 * @class MyHomeStages myhomestages.h "gui/myhomestages.h"
 * @brief The home stages dialog.
 */
class MyHomeStages : public MyHomeStages_Base
{
  public:
    /**
     * @brief Initializes all the needed variables.
     * @param stageframe Pointer to the stage frame object.
     * @param myframe Pointer to the main frame object.
     * @param parent Pointer to the parent window.
     */
    MyHomeStages(std::shared_ptr<StageFrame> stageframe, MyFrame *myframe, wxWindow *parent);

  private:

    /**
     * @brief Method wich will be executed, when the user klicks on the OK button. Resets the distance limits and homes the stage frame.
     * @param event Occuring event
     */
    void OnOK(wxCommandEvent& event);

    std::shared_ptr<StageFrame> m_StageFrame;				/**< Pointer to the stage frame object. */
    MyFrame *m_MyFrame;															/**< Pointer to the main frame object. */

    wxDECLARE_EVENT_TABLE();
};

/**
 * @enum ID for events in MyHomeStages
 */
enum
{
  ID_HomeStagesOK = 58
};

#endif // MYHOMESTAGES_H
