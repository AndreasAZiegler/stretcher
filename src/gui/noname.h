///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 24 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NONAME_H__
#define __NONAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/scrolwin.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

#define wxANY_ID 1000

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame_Base
///////////////////////////////////////////////////////////////////////////////
class MyFrame_Base : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_MenuBar;
		wxMenu* m_SettingsMenu;
		wxMenu* m_AdvancedMenu;
		wxNotebook* m_Experiments;
		wxPanel* m_InitializePanel;
		wxPanel* m_InitializePanel21;
		wxStaticText* m_InitializePresetStaticText;
		wxFilePickerCtrl* m_InitializePresetFilePicker;
		wxButton* m_InitializeApplyPresetButton;
		wxButton* m_InitializeSavePresetButton;
		wxStaticText* m_InitializeMaxPosStaticText;
		wxStaticText* m_InitializeMaxPosShowStaticText;
		wxStaticText* m_InitializeMountingLengthStaticText;
		wxStaticText* m_InitializeMountingLengthShowStaticText;
		wxStaticText* m_InitializeMinDistanceStaticText;
		wxStaticText* m_InitializeMinDistanceShowStaticText;
		wxStaticText* m_InitializeMaxDistanceStaticText;
		wxStaticText* m_InitializeMaxDistanceShowStaticText;
		wxStaticText* m_InitializeMinForceStaticText;
		wxStaticText* m_InitializeMinForceShowStaticText;
		wxStaticText* m_InitializeMaxForceStaticText;
		wxStaticText* m_InitializeMaxForceShowStaticText;
		wxStaticText* m_InitializeDistanceSensitivityStaticText;
		wxStaticText* m_InitializeDistanceSensitivityShowStaticText;
		wxStaticText* m_InitializeForceStressSensitivityStaticText;
		wxStaticText* m_InitializeForceStressSensitivityShowStaticText;
		wxRadioBox* m_InitializeUnitRadioBox;
		wxPanel* m_InitializeCalculateDiameterPanel;
		wxStaticText* m_InitializeCrossSectionStaticText;
		wxSpinCtrl* m_InitializeCrossSectionSpinCtrl;
		wxPanel* m_LimitsPanel;
		wxPanel* m_LimitsLimitPanel1;
		wxStaticText* m_LimitsLimitStaticText60;
		wxStaticText* m_staticText61;
		wxStaticText* m_LimitsLimitMinDistanceStaticText;
		wxSpinCtrl* m_LimitsLimitMinDistanceSpinCtrl;
		wxStaticText* m_LimitsLimitMaxDistanceStaticText;
		wxSpinCtrl* m_LimitsLimitMaxDistanceSpinCtrl;
		wxStaticText* m_LimitsLimitMinForceStaticText;
		wxSpinCtrl* m_LimitsLimitMinForceSpinCtrl;
		wxStaticText* m_LimitsLimitMaxForceStaticText;
		wxSpinCtrl* m_LimitsLimitMaxForceSpinCtrl;
		wxButton* m_LimitsLimitSetButton;
		wxPanel* m_LimitsLimitPanel2;
		wxButton* m_LimitsLimitSet1Button;
		wxButton* m_LimitsLimitSet2Button;
		wxButton* m_LimitsLimitSet3Button;
		wxButton* m_LimitsLimitSet4Button;
		wxPanel* m_LengthsPanel;
		wxPanel* m_LengthsPanel21;
		wxStaticText* m_LengthsMoveStaticText;
		wxStaticText* m_LengthsGoToStaticText;
		wxSpinCtrl* m_LengthsGoToSpinCtrl;
		wxButton* m_LengthsGoToButton;
		wxButton* m_LengthsMountingLengthButton;
		wxPanel* m_LengthsPanel22;
		wxStaticText* m_LengthsSensitivityStaticText;
		wxStaticText* m_LengthsForceStressSensitivityStaticText;
		wxSpinCtrl* m_LengthsForceStressSensitivitySpinCtrl;
		wxStaticText* m_LengthsDistanceSensitivityStaticText;
		wxSpinCtrl* m_LengthsDistanceSensitivitySpinCtrl;
		wxButton* m_LengthsSensitivityButton;
		wxPanel* m_LengthsPanel23;
		wxStaticText* m_LengthsZeroStaticText;
		wxButton* m_LengthsSetDistanceZeroButton;
		wxButton* m_LengthsSetForceZeroButton;
		wxPanel* m_PreloadPanel;
		wxPanel* m_PreloadCalculateDiameterPanel;
		wxStaticText* m_PreloadLimitStaticText;
		wxSpinCtrl* m_PreloadLimitSpinCtrl;
		wxStaticText* m_PreloadSpeedStaticText;
		wxPanel* m_PreloadSpeedPanel;
		wxSpinCtrl* m_PreloadSpeedPreloadSpinCtrl;
		wxStaticText* m_PreloadSpeedPreloadStaticText;
		wxSpinCtrl* m_PreloadSpeedMmSpinCtrl;
		wxStaticText* m_PreloadSpeedMmStaticText;
		wxButton* m_PreloadCancelButton;
		wxButton* m_PreloadSendButton;
		wxPanel* m_OneStepPanel1;
		wxPanel* m_OneStepPanel21;
		wxRadioButton* m_OneStepStressForceRadioBtn;
		wxRadioButton* m_OneStepDistanceRadioBtn;
		wxPanel* m_OneStepStressForcePanel;
		wxStaticText* m_OneStepStressForceVelocityStaticText;
		wxSpinCtrl* m_OneStepStressForceVelocitySpinCtrl;
		wxRadioButton* m_OneStepStressForceVelocityMmRadioBtn;
		wxRadioButton* m_OneStepStressForceVelocityPercentRadioBtn;
		wxStaticText* m_OneStepStressForceDelayStaticText;
		wxSpinCtrl* m_OneStepStressForceDelaySpinCtrl;
		wxStaticText* m_staticText78;
		wxStaticText* m_OneStepStressForceLimitStaticText;
		wxSpinCtrl* m_OneStepStressForceLimitSpinCtrl;
		wxCheckBox* m_OneStepStressForceHoldLimitCheckBox;
		wxStaticText* m_OneStepStressForceDwellStaticText;
		wxSpinCtrl* m_OneStepStressForceDwellSpinCtrl;
		wxStaticText* m_staticText82;
		wxCheckBox* m_OneStepStressForcePhotoCheckBox;
		wxPanel* m_OneStepDistancePanel;
		wxStaticText* m_OneStepDistanceVelocityStaticText;
		wxSpinCtrl* m_OneStepDistanceVelocitySpinCtrl;
		wxRadioButton* m_OneStepDistanceVelocityMmRadioBtn;
		wxRadioButton* m_OneStepDistanceVelocityPercentRadioBtn;
		wxStaticText* m_OneStepDistanceDelayStaticText;
		wxSpinCtrl* m_OneStepDistanceDelaySpinCtrl;
		wxStaticText* m_staticText781;
		wxStaticText* m_OneStepDistanceUpperLimitStaticText;
		wxSpinCtrl* m_OneStepDistanceLimitSpinCtrl;
		wxRadioButton* m_OneStepDistanceLimitMmRelRadioBtn;
		wxRadioButton* m_OneStepDistanceLimitMmRadioBtn;
		wxRadioButton* m_OneStepDistanceLimitPercentRadioBtn;
		wxStaticText* m_OneStepDistanceDwellStaticText;
		wxSpinCtrl* m_OneStepDistanceDwellSpinCtrl;
		wxStaticText* m_staticText821;
		wxCheckBox* m_OneStepDistancePhotoCheckBox;
		wxPanel* m_OneStepPanel22;
		wxCheckBox* m_OneStepEndOfEventRepeatCheckBox;
		wxSpinCtrl* m_OneStepEndOfEventRepeatSpinCtrl;
		wxStaticText* m_staticText75;
		wxStaticText* m_OneStepEndOfEventStaticText;
		wxStaticText* m_staticText711;
		wxStaticText* m_staticText72;
		wxRadioButton* m_OneStepEndOfEventStopRadioBtn;
		wxStaticText* m_staticText84;
		wxStaticText* m_staticText85;
		wxRadioButton* m_OneStepEndOfEventHoldRadioBtn;
		wxSpinCtrl* m_OneStepEndOfEventHoldSpinCtrl;
		wxPanel* m_OneStepPanel3;
		wxRadioButton* m_OneStepEndOfEventHoldMmRelRadioBtn;
		wxRadioButton* m_OneStepEndOfEventHoldMmRadioBtn;
		wxRadioButton* m_OneStepEndOfEventHoldPercentRadioBtn;
		wxRadioButton* m_OneStepEndOfEventL0RadioBtn;
		wxStaticText* m_staticText803;
		wxStaticText* m_staticText81;
		wxRadioButton* m_OneStepEndOfEventMLRadioBtn;
		wxStaticText* m_staticText142;
		wxButton* m_OneStepCancelButton;
		wxButton* m_OneStepSendButton;
		wxPanel* m_ContinuosPanel;
		wxPanel* m_ContinuousPanel21;
		wxRadioButton* m_ContinuousStressForceRadioBtn;
		wxRadioButton* m_ContinuousDistanceRadioBtn;
		wxPanel* m_ContinuousStressForcePanel;
		wxStaticText* m_ContinuousStressForceVelocityStaticText;
		wxSpinCtrl* m_ContinuousStressForceVelocitySpinCtrl;
		wxRadioButton* m_ContinuousStressForceVelocityMmRadioBtn;
		wxRadioButton* m_ContinuousStressForceVelocityPercentRadioBtn;
		wxStaticText* m_ContinuousStressForceHoldTimeStaticText;
		wxSpinCtrl* m_ContinuousStressForceHoldTimeSpinCtrl;
		wxStaticText* m_staticText782;
		wxStaticText* m_ContinuousStressForceIncrementStaticText;
		wxSpinCtrl* m_ContinuousStressForceIncrementSpinCtrl;
		wxStaticText* m_staticText802;
		wxRadioButton* m_ContinuousStressForceMaxValueRadioBtn;
		wxRadioButton* m_ContinuousStressForceStepsRadioBtn;
		wxStaticText* m_ContinuousStressForceMaxValueStaticText;
		wxSpinCtrl* m_ContinuousStressForceMaxValueSpinCtrl;
		wxRadioButton* m_ContinuousStressForceMaxValueValueRadioBtn;
		wxRadioButton* m_ContinuousStressForceMaxValuePercentRadioBtn;
		wxStaticText* m_ContinuousStressForceStepsStaticText;
		wxSpinCtrl* m_ContinuousStressForceStepsSpinCtrl;
		wxStaticText* m_staticText76;
		wxCheckBox* m_ContinuousStressForcePhotoCheckBox;
		wxPanel* m_ContinuousDistancePanel;
		wxStaticText* m_ContinuousDistanceVelocityStaticText;
		wxSpinCtrl* m_ContinuousDistanceVelocitySpinCtrl;
		wxPanel* m_ContinuousDistancePanel21;
		wxRadioButton* m_ContinuousDistanceVelocityMmRadioBtn;
		wxRadioButton* m_ContinuousDistanceVelocityPercentRadioBtn;
		wxStaticText* m_ContinuousDistanceHoldTimeStaticText;
		wxSpinCtrl* m_ContinuousDistanceHoldTimeSpinCtrl;
		wxStaticText* m_staticText7811;
		wxStaticText* m_ContinuousDistanceIncrementStaticText;
		wxSpinCtrl* m_ContinuousDistanceIncrementSpinCtrl;
		wxPanel* m_ContinuousDistancePanel22;
		wxRadioButton* m_ContinuousDistanceIncrementMmRadioBtn;
		wxRadioButton* m_ContinuousDistanceIncrementPercentRadioBtn;
		wxRadioButton* m_ContinuousDistanceMaxValueRadioBtn;
		wxRadioButton* m_ContinuousDistanceStepsRadioBtn;
		wxStaticText* m_ContinuousDistanceMaxValueStaticText;
		wxSpinCtrl* m_ContinuousDistanceMaxValueSpinCtrl;
		wxPanel* m_ContinuousDistancePanel23;
		wxRadioButton* m_ContinuousDistanceMaxValueMmRelRadioBtn;
		wxRadioButton* m_ContinuousDistanceMaxValueMmRadioBtn;
		wxRadioButton* m_ContinuousDistanceMaxValuePercentRadioBtn;
		wxStaticText* m_ContinuousDistanceStepsStaticText;
		wxSpinCtrl* m_ContinuousDistanceStepsSpinCtrl;
		wxStaticText* m_staticText801;
		wxCheckBox* m_ContinuousDistancePhotoCheckBox;
		wxPanel* m_ContinuousPanel22;
		wxCheckBox* m_ContinuousEndOfEventRepeatCheckBox;
		wxSpinCtrl* m_ContinuousEndOfEventRepeatSpinCtrl;
		wxStaticText* m_staticText94;
		wxStaticText* m_OneStepEndOfEventStaticText1;
		wxStaticText* m_staticText7111;
		wxStaticText* m_staticText95;
		wxRadioButton* m_ContinuousEndOfEventStopRadioBtn;
		wxStaticText* m_staticText140;
		wxStaticText* m_staticText96;
		wxRadioButton* m_ContinuousEndOfEventHoldDistanceRadioBtn;
		wxSpinCtrl* m_ContinuousEndOfEventHoldDistanceSpinCtrl;
		wxPanel* m_ContinuousPanel31;
		wxRadioButton* m_ContinuousEndOfEventHoldMmRelRadioBtn;
		wxRadioButton* m_ContinuousEndOfEventHoldMmRadioBtn;
		wxRadioButton* m_ContinuousEndOfEventHoldPercentRadioBtn;
		wxRadioButton* m_ContinuousEndOfEventL0RadioBtn;
		wxStaticText* m_staticText83;
		wxStaticText* m_staticText97;
		wxRadioButton* m_ContinuousEndOfEventMLRadioBtn;
		wxStaticText* m_staticText86;
		wxStaticText* m_staticText98;
		wxRadioButton* m_ContinuousEndOfEventHoldStressForceRadioBtn;
		wxSpinCtrl* m_ContinuousEndOfEventHoldStressForceSpinCtrl;
		wxStaticText* m_staticText143;
		wxButton* m_ContinuousCancelButton;
		wxButton* m_ContinuousSendButton;
		wxButton* m_DecreaseDistanceButton;
		wxButton* m_IncreaseDistanceButton;
		wxButton* m_StopButton;
		wxStaticText* m_ForceStaticText;
		wxStaticText* m_DistanceStaticText;
		wxPanel* m_Protocols;
		wxListBox* m_ProtocolsListBox;
		wxPanel* m_ProtocolsNavigationPanel;
		wxButton* m_ProtocolsUpButton;
		wxButton* m_ProtocolsTriggerButton;
		wxButton* m_ProtocolsXButton;
		wxButton* m_ProtocolsPauseButton;
		wxButton* m_ProtocolsDownButton;
		wxButton* m_ProtocolsPauseResumeButton;
		wxCheckBox* m_ProtocolsLoopCheckBox;
		wxSpinCtrl* m_ProtocolsLoopSpinCtrl;
		wxStaticText* m_ProtocolsCurrentLoopStaticText;
		wxStaticText* m_ProtocolsCurrentLoopNrStaticText;
		wxButton* m_ProtocolsPreviewButton;
		wxStaticText* m_staticText91;
		wxButton* m_ProtocolsRunButton;
		wxButton* m_ProtocolsStopButton;
		wxButton* m_ProtocolsSaveButton;
		wxButton* m_ProtocolsLoadButton;
		wxScrolledWindow* m_Results;
		wxTextCtrl* m_LogTextCtrl;
		wxButton* m_LogClearButton;
		wxButton* m_LogSaveButton;
		wxPanel* m_GraphPanel;
		wxButton* m_GraphExportCSVButton;
		wxButton* m_GraphExportPNGButton;
		wxButton* m_GraphClearButton;
		wxComboBox* m_GraphTypeComboBox;
		wxCheckBox* m_GraphLimitsCheckBox;
		
		// Virtual event handlers, overide them in your derived class
		virtual void 2( wxCommandEvent& event ) { event.Skip(); }
		virtual void 3( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyFrame_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Stretcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 2169,1358 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
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
		
		MySamplingFrequency_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sampling Frequency"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 382,165 ), long style = wxDEFAULT_DIALOG_STYLE ); 
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
/// Class MyFileOutput_Base
///////////////////////////////////////////////////////////////////////////////
class MyFileOutput_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_FileOutputNameStaticText;
		wxDirPickerCtrl* m_FileOutputNameDirPicker;
		wxStdDialogButtonSizer* m_FileOutputSdbSizer;
		wxButton* m_FileOutputSdbSizerOK;
		wxButton* m_FileOutputSdbSizerCancel;
	
	public:
		
		MyFileOutput_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("File Output"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 249,123 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyFileOutput_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyHomeStages_Base
///////////////////////////////////////////////////////////////////////////////
class MyHomeStages_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText62;
		wxStdDialogButtonSizer* m_HomeStagesSdbSizer;
		wxButton* m_HomeStagesSdbSizerOK;
		wxButton* m_HomeStagesSdbSizerCancel;
	
	public:
		
		MyHomeStages_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 703,95 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyHomeStages_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyExportDialog_Base
///////////////////////////////////////////////////////////////////////////////
class MyExportDialog_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_ExportDialog1StaticText;
		wxFilePickerCtrl* m_ExportDialogFilePicker;
		wxStaticText* m_ExportDialog2StaticText;
		wxListBox* m_ExportDialogListBox;
		wxStdDialogButtonSizer* m_ExportDialogSdbSizer;
		wxButton* m_ExportDialogSdbSizerOK;
		wxButton* m_ExportDialogSdbSizerCancel;
	
	public:
		
		MyExportDialog_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,651 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER ); 
		~MyExportDialog_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyPauseDialog_Base
///////////////////////////////////////////////////////////////////////////////
class MyPauseDialog_Base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText69;
		wxSpinCtrl* m_PauseDialogWaitingTimeSpinCtrl;
		wxStdDialogButtonSizer* m_PauseDialogSdbSizer;
		wxButton* m_PauseDialogSdbSizerOK;
		wxButton* m_PauseDialogSdbSizerCancel;
	
	public:
		
		MyPauseDialog_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 248,132 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyPauseDialog_Base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyStartUpDialog_Base
///////////////////////////////////////////////////////////////////////////////
class MyStartUpDialog_Base : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_StartUpDialogPanel1;
		wxStaticText* m_StartUpHomingStaticText;
		wxStaticText* m_StartUpLEStaticText;
		wxPanel* m_StartUpDialogPanel2;
		wxButton* m_StartUpDecreaseDistanceButton;
		wxButton* m_StartUpIncreaseDistanceButton;
		wxSpinCtrl* m_StartUpInputSpinCtrl;
		wxButton* m_StartUpSetButton;
		wxStdDialogButtonSizer* m_StartUpDialogSdbSizer;
		wxButton* m_StartUpDialogSdbSizerOK;
		wxButton* m_StartUpDialogSdbSizerCancel;
	
	public:
		
		MyStartUpDialog_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 476,329 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MyStartUpDialog_Base();
	
};

#endif //__NONAME_H__
