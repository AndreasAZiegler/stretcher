///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NONAME_H__
#define __NONAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/spinctrl.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/scrolwin.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame_Base
///////////////////////////////////////////////////////////////////////////////
class MyFrame_Base : public wxFrame 
{
	private:
	
	protected:
		wxNotebook* m_Experiments;
		wxPanel* m_InitializePanel;
		wxStaticText* m_InitializeWarningStaticText;
		wxButton* m_InitializeHomeMotorsButton;
		wxPanel* m_ClampingPosPanel;
		wxStaticText* m_ClampingPositionStaticText;
		wxSpinCtrl* m_ClampingPositionSpinCtrl;
		wxButton* m_m_ClampingPositionButton;
		wxPanel* m_PreloadPanel;
		wxRadioBox* m_PreloadUnitRadioBox;
		wxPanel* m_PreloadCalculateDiameterPanel;
		wxCheckBox* m_PreloadCalculateDiameterCheckBox;
		wxStaticText* m_PreloadYStaticText;
		wxSpinCtrl* m_PreloadYSpinCtrl;
		wxStaticText* m_PreloadXStaticText;
		wxRadioBox* m_PreloadXRadioBox;
		wxSpinCtrl* m_PreloadXSpinCtrl;
		wxStaticText* m_PreloadLimitStaticText;
		wxSpinCtrl* m_PreloadLimitSpinCtrl;
		wxStaticText* m_PreloadSpeedStaticText;
		wxSpinCtrl* m_PreloadSpeedSpinCtrl;
		wxStdDialogButtonSizer* m_PreloadSdbSizer;
		wxButton* m_PreloadSdbSizerOK;
		wxButton* m_PreloadSdbSizerCancel;
		wxPanel* m_ConditioningPanel;
		wxStaticText* m_ConditioningCyclesStaticText;
		wxSpinCtrl* m_ConditioningCyclesSpinCtrl;
		wxStaticText* m_ConditioningSpeedStaticText;
		wxSpinCtrl* m_ConditioningSpeedPreloadSpinCtrl;
		wxSpinCtrl* m_ConditioningSpeedMmSpinCtrl;
		wxStaticText* m_ConditioningStressForceLimitStaticText;
		wxSpinCtrl* m_ConditioningStressForceLimitSpinCtrl;
		wxRadioBox* m_ConditioningLimitRadioBox;
		wxStaticText* m_ConditioningDistanceLimitStaticText;
		wxSpinCtrl* m_ConditioningDistanceLimitSpinCtrl;
		wxRadioBox* m_ConditioningDisctanceLimitRadioBox;
		wxStdDialogButtonSizer* m_ConditioningSdbSizer;
		wxButton* m_ConditioningSdbSizerOK;
		wxButton* m_ConditioningSdbSizerCancel;
		wxPanel* m_Ramp2FailurePanel;
		wxStaticText* m_R2FSpeedStaticText;
		wxSpinCtrl* m_R2FSpeedSpinCtrl;
		wxStaticText* m_R2FDropBStopStaticText;
		wxSpinCtrl* m_R2FDropBStopSpinCtrl;
		wxStaticText* m_R2FAfterFailureStaticText;
		wxRadioBox* m_R2FAfterFailureRadioBox;
		wxSpinCtrl* m_R2FGoToSpinCtrl;
		wxRadioBox* m_R2FGoToRadioBox;
		wxStdDialogButtonSizer* m_R2FSdbSizer;
		wxButton* m_R2FSdbSizerOK;
		wxButton* m_R2FSdbSizerCancel;
		wxPanel* m_RelaxationPanel;
		wxStaticText* m_RelaxationRampStaticText;
		wxSpinCtrl* m_RelaxationRampSpinCtrl;
		wxRadioBox* m_RelaxationRampRadioBox;
		wxStaticText* m_RelaxationPauseStaticText;
		wxSpinCtrl* m_RelaxationPauseSpinCtrl;
		wxStaticText* m_RelaxationStepsStaticText;
		wxSpinCtrl* m_RelaxationStepsSpinCtrl;
		wxStdDialogButtonSizer* m_RelaxationSdbSizer;
		wxButton* m_RelaxationSdbSizerOK;
		wxButton* m_RelaxationSdbSizerCancel;
		wxPanel* m_CreepPanel;
		wxStaticText* m_CreepSpeedStaticText;
		wxSpinCtrl* m_CreepSpeedSpinCtrl;
		wxStaticText* m_CreepHoldForceStressStaticText;
		wxSpinCtrl* m_CreepHoldForceStressSpinCtrl;
		wxStaticText* m_CreepHoldTimeStaticText;
		wxSpinCtrl* m_CreepHoldTimeSpinCtrl;
		wxStdDialogButtonSizer* m_CreepSdbSizer;
		wxButton* m_CreepSdbSizerOK;
		wxButton* m_CreepSdbSizerCancel;
		wxPanel* m_FatiguePanel;
		wxStaticText* m_FatigueCyclesStaticText;
		wxSpinCtrl* m_FatigueCyclesSpinCtrl;
		wxStaticText* m_FatigueTotalTimeStaticText;
		wxSpinCtrl* m_FatigueTotalTimeSpinCtrl;
		wxStaticText* m_FatigueAmplitudeTimeStaticText;
		wxSpinCtrl* m_FatigueAmplitudeSpinCtrl;
		wxRadioBox* m_AmplitudeRadioBox;
		wxStaticText* m_RestTimeStaticText;
		wxSpinCtrl* m_RestTimeSpinCtrl;
		wxStaticText* m_FrequencyStaticText;
		wxSpinCtrl* m_spinCtrl24;
		wxCheckBox* m_checkBox2;
		wxButton* m_DecreaseDistanceButton;
		wxButton* m_IncreaseDistanceButton;
		wxButton* m_StopButton;
		wxStaticText* m_ForceStaticText;
		wxPanel* m_Protocols;
		wxListBox* m_ProtocolsListBox;
		wxPanel* m_ProtocolsNavigationPanel;
		wxButton* m_ProtocolsExperiment1Button;
		wxStaticText* m_staticText32;
		wxButton* m_ProtocolsExperiment2Button;
		wxButton* m_ProtocolsUpButton;
		wxButton* m_ProtocolsExperiment3Button;
		wxButton* m_ProtocolsXButton;
		wxButton* m_ProtocolsExperiment4Button;
		wxButton* m_ProtocolsDownButton;
		wxButton* m_ProtocolsExperiment5Button;
		wxCheckBox* m_checkBox3;
		wxScrolledWindow* m_Results;
		wxListCtrl* m_ResultListCtrl;
		wxButton* m_ResultSaveButton;
		wxPanel* m_Graph;
		wxMenuBar* m_MenuBar;
		wxMenu* m_SettingsMenu;
		wxMenu* m_SamplingFrequencyMenu;
		wxMenu* m_PortsMenu;
		wxMenu* m_FileOutputMenu;
	
	public:
		
		MyFrame_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Stretcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1212,646 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MyFrame_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MySamplingFrequency_Base
///////////////////////////////////////////////////////////////////////////////
class MySamplingFrequency_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_SamplingFrequencyDistanceStaticText;
		wxSpinCtrl* m_SamplingFrequencyDistanceSpinCtrl;
		wxStaticText* m_SamplingFrequencyForceStaticText;
		wxSpinCtrl* m_SamplingFrequencyForceSpinCtrl;
		wxStdDialogButtonSizer* m_SamplingFrequencySdbSizer;
		wxButton* m_SamplingFrequencySdbSizerOK;
		wxButton* m_SamplingFrequencySdbSizerCancel;
	
	public:
		
		MySamplingFrequency_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sampling Frequency"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 322,130 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MySamplingFrequency_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyPorts_Base
///////////////////////////////////////////////////////////////////////////////
class MyPorts_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_PortsMotor1StaticText;
		wxChoice* m_PortsMotor1Choice;
		wxStaticText* m_PortsMotor2StaticText;
		wxChoice* m_PortsMotor2Choice;
		wxStaticText* m_PortsForceStaticText;
		wxChoice* m_PortsForceChoice;
		wxStdDialogButtonSizer* m_PortsSdbSizer;
		wxButton* m_PortsSdbSizerOK;
		wxButton* m_PortsSdbSizerCancel;
	
	public:
		
		MyPorts_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Ports"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyPorts_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyFileOutput_Vase
///////////////////////////////////////////////////////////////////////////////
class MyFileOutput_Vase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_FileOutputNameStaticText;
		wxFilePickerCtrl* m_FileOutputNameFilePicker;
		wxStdDialogButtonSizer* m_FileOutputSdbSizer;
		wxButton* m_FileOutputSdbSizerOK;
		wxButton* m_FileOutputSdbSizerCancel;
	
	public:
		
		MyFileOutput_Vase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("File Output"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyFileOutput_Vase();
	
};

#endif //__NONAME_H__
