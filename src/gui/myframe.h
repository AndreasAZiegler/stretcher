#ifndef MYFRAME_H
#define MYFRAME_H

/*************** Includes ************/
#include <wx/window.h>
#include <wx/string.h>
#include "myframe_base.h"
#include "../settings.h"
#include "./hardware/linearstage.h"

/**
 * @brief The Main Frame class
 */
class MyFrame : public MyFrame_Base
{
  public:
    /**
     * @brief Constructor of the main frame. Sets the icon.
     * @param title Title of the software.
     * @param parent Pointer to the parent object.
     */
    MyFrame(const wxString& title, Settings *settings, wxWindow* parent = (wxWindow *)NULL);

    /**
     * @brief Register the liner motors.
     * @param linearmotor Pointer to the vector containing the linear motors.
     */
    void registerLinearStage(std::vector<LinearStage *> *linearmotor);

    ~MyFrame();

    void startup(void);
  private:
    /**
     * @brief Method wich will be executed, when the software will be closed by the user.
     * @param event Occuring event
     */
    void OnExit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the frequency settings.
     * @param event Occuring event
     */
    void OnSamplingFrequencySettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the port settings.
     * @param event Occuring event
     */
    void OnPortsSettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user goes to the file output settings.
     * @param event Occuring event
     */
    void OnFileOutputSettings(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user activates the calculate diameter settings.
     * @param event Occuring event
     */
    void OnCalculateDiameter(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user changes unit.
     * @param event Occuring event
     */
    void OnUnit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses distance as limit.
     * @param event Occuring event
     */
    void OnDistanceLimit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses stress as limit.
     * @param event Occuring event
     */
    void OnStressLimit(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses "Go to" after failure in R2F.
     * @param event Occuring event
     */
    void OnGoTo(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user chooses a measurement in Chamber stretch.
     * @param event Occuring event
     */
    void OnChamberMeasurement(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the decrease distance button.
     * @param event Occuring event
     */
    void OnMotorDecreaseDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the increase distance button.
     * @param event Occuring event
     */
    void OnMotorIncreaseDistance(wxCommandEvent& event);

    /**
     * @brief Method wich will be executed, when the user klicks on the motor stop button.
     * @param event Occuring event
     */
    void OnMotorStop(wxCommandEvent& event);

    mpWindow* m_Graph;					/**< Pointer to the graph */
    Settings *m_Settings;				/**< Pointer to the settings object */
    std::vector<LinearStage*> *m_LinearStages;


    wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_SamplingFrequency = 1,
	ID_Ports = 2,
	ID_FileOutput = 3,
	ID_CalculateDiameter = 4,
	ID_Unit = 5,
	ID_DistanceLimit = 6,
	ID_StressLimit = 7,
  ID_GoTo = 8,
  ID_ChamberMeasurement = 9,
  ID_MotorDecreaseDistance = 10,
  ID_MotorIncreaseDistance = 11,
  ID_MotorStop = 12
};

#endif // MYFRAME_H
