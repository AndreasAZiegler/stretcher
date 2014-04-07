#ifndef MYFRAME_H
#define MYFRAME_H

/*************** Includes ************/
#include <wx/window.h>
#include <wx/string.h>
#include "myframe_base.h"
#include "../updatevalues.h"
#include "../settings.h"
#include "./hardware/linearstage.h"
#include "./hardware/forcesensor.h"

/**
 * @brief The Main Frame class
 */
class MyFrame : public MyFrame_Base, public UpdateValues
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

    /**
     * @brief Register the force sensor.
     * @param forcesensor Pointer to the force sensor.
     */
    void registerForceSensor(ForceSensor *forcesensor);

    /**
     * @brief Destructor
     */
    ~MyFrame();

    /**
     * @brief Will be executed from the classes LinearStageMessageHandler and ForceSensorMessageHandler which are running in a seperate
     * 				thread. (CallAfter() asynchronously call the updateDistance method)
     * @param value The position of a stage or a force.
     * @param type	Defines the type of the value (position of stage 1, 2 or force)
     */
    virtual void updateValue(int value, UpdateValues::ValueType type);

    /**
     * @brief Hides calculate diameter options, hides cells panel in chamber stretch, hides distance limit options, hides go to options,
     * 				sets digits for the wxSpinCtrlDouble.
     */
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
     * @brief Method wich will be executed, when the user klicks on the home stage button.
     * @param event Occuring event
     */
    void OnHomeLinearStages(wxCommandEvent& event);

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

    /**
     * @brief Calculates the distance and print the value in the GUI.
     */
    void updateDistance();

    /**
     * @brief Calculates the distance and print the value in the GUI.
     */
    void updateForce();

    mpWindow* m_Graph;													/**< Pointer to the graph */
    Settings *m_Settings;												/**< Pointer to the settings object */
    std::vector<LinearStage*> *m_LinearStages;	/**< Vector containing the pointers to the linear stages */
    ForceSensor *m_ForceSensor;									/**< Pointer to the force sensor */
    std::vector<int> m_CurrentPositions;				/**< Vector with the current stage positions */
    double m_CurrentDistance;										/**< Current distance */

    double m_CurrentForce;											/**< Current force */
    wxString m_ForceUnit;												/**< Current force unit (N or kPa) */

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
  ID_MotorStop = 12,
  ID_HomeStages = 13
};

#endif // MYFRAME_H
