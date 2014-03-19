//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __myframe_base_h__
#define __myframe_base_h__

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/scrolwin.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/xrc/xmlres.h>
#include "xh_wxspinctrldoublexmlhandler.h"

class MyFrame_Base : public wxFrame {
protected:
 wxNotebook* m_Experiments;
 wxPanel* m_InitializePanel;
 wxStaticText* m_InitializeWarningStaticText;
 wxButton* m_InitializeHomeMotorsButton;
 wxPanel* m_ClampingPosPanel;
 wxStaticText* m_ClampingPositionStaticText;
 wxSpinCtrlDouble* m_ClampingPositionSpinCtrl;
 wxButton* m_ClampingPositionButton;
 wxPanel* m_PreloadPanel;
 wxRadioBox* m_PreloadUnitRadioBox;
 wxPanel* m_PreloadCalculateDiameterPanel;
 wxCheckBox* m_PreloadCalculateDiameterCheckBox;
 wxStaticText* m_PreloadYStaticText;
 wxSpinCtrlDouble* m_PreloadYSpinCtrl;
 wxRadioBox* m_PreloadXRadioBox;
 wxSpinCtrlDouble* m_PreloadXSpinCtrl;
 wxStaticText* m_PreloadLimitStaticText;
 wxSpinCtrlDouble* m_PreloadLimitSpinCtrl;
 wxStaticText* m_PreloadSpeedStaticText;
 wxSpinCtrlDouble* m_PreloadSpeedSpinCtrl;
 wxButton* wxID_PreloadOK;
 wxButton* wxID_PreloadCANCEL;
 wxPanel* m_ConditioningPanel;
 wxStaticText* m_ConditioningCyclesStaticText;
 wxSpinCtrl* m_ConditioningCyclesSpinCtrl;
 wxStaticText* m_ConditioningSpeedStaticText;
 wxSpinCtrlDouble* m_ConditioningSpeedPreloadSpinCtrl;
 wxStaticText* m_ConditioningSpeedPreloadStaticText;
 wxSpinCtrlDouble* m_ConditioningSpeedMmSpinCtrl;
 wxStaticText* m_ConditioningSpeedMMStaticText;
 wxStaticText* m_ConditioningStressForceLimitStaticText;
 wxSpinCtrlDouble* m_ConditioningStressForceLimitSpinCtrl;
 wxRadioButton* m_ConditioningStressRadioBtn;
 wxRadioButton* m_ConditioningDistanceRadioBtn;
 wxStaticText* m_ConditioningDistanceLimitStaticText;
 wxSpinCtrlDouble* m_ConditioningDistanceLimitSpinCtrl;
 wxRadioBox* m_ConditioningDisctanceLimitRadioBox;
 wxButton* wxID_ConditioningOK;
 wxButton* wxID_ConditioningCANCEL;
 wxPanel* m_Ramp2FailurePanel;
 wxStaticText* m_R2FSpeedStaticText;
 wxSpinCtrlDouble* m_R2FSpeedPreloadSpinCtrl;
 wxStaticText* m_R2FSpeedPreloadStaticText;
 wxSpinCtrlDouble* m_R2FSpeedMmSpinCtrl;
 wxStaticText* m_R2FSpeedMmStaticText;
 wxStaticText* m_R2FDropBStopStaticText;
 wxSpinCtrlDouble* m_R2FDropBStopSpinCtrl;
 wxStaticText* m_R2FAfterFailureStaticText;
 wxRadioBox* m_R2FAfterFailureRadioBox;
 wxSpinCtrlDouble* m_R2FGoToSpinCtrl;
 wxRadioBox* m_R2FGoToRadioBox;
 wxButton* wxID_R2FOK;
 wxButton* wxID_R2FCANCEL;
 wxPanel* m_RelaxationPanel;
 wxStaticText* m_RelaxationRampStaticText;
 wxSpinCtrlDouble* m_RelaxationRampSpinCtrl;
 wxRadioBox* m_RelaxationRampRadioBox;
 wxStaticText* m_staticText321;
 wxStaticText* m_RelaxationPauseStaticText;
 wxSpinCtrlDouble* m_RelaxationPauseSpinCtrl;
 wxStaticText* m_RelaxationStepsStaticText;
 wxSpinCtrl* m_RelaxationStepsSpinCtrl;
 wxButton* wxID_RelaxationOK;
 wxButton* wxID_RelaxationCANCEL;
 wxPanel* m_CreepPanel;
 wxStaticText* m_CreepSpeedStaticText;
 wxSpinCtrlDouble* m_CreepSpeedPreloadSpinCtrl;
 wxStaticText* m_CreepSpeedPreloadStaticText;
 wxSpinCtrlDouble* m_CreepSpeedMmSpinCtrl;
 wxStaticText* m_CreepSpeedMmStaticText;
 wxStaticText* m_CreepHoldForceStressStaticText;
 wxSpinCtrlDouble* m_CreepHoldForceStressSpinCtrl;
 wxStaticText* m_CreepHoldTimeStaticText;
 wxSpinCtrlDouble* m_CreepHoldTimeSpinCtrl;
 wxButton* wxID_CreepOK;
 wxButton* wxID_CreepCANCEL;
 wxPanel* m_FatiguePanel;
 wxStaticText* m_FatigueCyclesStaticText;
 wxSpinCtrl* m_FatigueCyclesSpinCtrl;
 wxStaticText* m_FatigueTotalTimeStaticText;
 wxSpinCtrlDouble* m_FatigueTotalTimeSpinCtrl;
 wxStaticText* m_FatigueAmplitudeTimeStaticText;
 wxSpinCtrlDouble* m_FatigueAmplitudeSpinCtrl;
 wxRadioBox* m_FatigueAmplitudeRadioBox;
 wxStaticText* m_staticText33;
 wxStaticText* m_FatigueRestTimeStaticText;
 wxSpinCtrlDouble* m_FatigueRestTimeSpinCtrl;
 wxStaticText* m_FatigueFrequencyStaticText;
 wxSpinCtrlDouble* m_FatigueFrequencySpinCtrl;
 wxCheckBox* m_FatigueLoopCheckBox;
 wxButton* wxID_FatigueOK;
 wxButton* wxID_FatigueCANCEL;
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

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MyFrame_Base"), wxT("wxFrame"));
  m_Experiments = XRCCTRL(*this,"m_Experiments",wxNotebook);
  m_InitializePanel = XRCCTRL(*this,"m_InitializePanel",wxPanel);
  m_InitializeWarningStaticText = XRCCTRL(*this,"m_InitializeWarningStaticText",wxStaticText);
  m_InitializeHomeMotorsButton = XRCCTRL(*this,"m_InitializeHomeMotorsButton",wxButton);
  m_ClampingPosPanel = XRCCTRL(*this,"m_ClampingPosPanel",wxPanel);
  m_ClampingPositionStaticText = XRCCTRL(*this,"m_ClampingPositionStaticText",wxStaticText);
  m_ClampingPositionSpinCtrl = XRCCTRL(*this,"m_ClampingPositionSpinCtrl",wxSpinCtrlDouble);
  m_ClampingPositionButton = XRCCTRL(*this,"m_ClampingPositionButton",wxButton);
  m_PreloadPanel = XRCCTRL(*this,"m_PreloadPanel",wxPanel);
  m_PreloadUnitRadioBox = XRCCTRL(*this,"m_PreloadUnitRadioBox",wxRadioBox);
  m_PreloadCalculateDiameterPanel = XRCCTRL(*this,"m_PreloadCalculateDiameterPanel",wxPanel);
  m_PreloadCalculateDiameterCheckBox = XRCCTRL(*this,"m_PreloadCalculateDiameterCheckBox",wxCheckBox);
  m_PreloadYStaticText = XRCCTRL(*this,"m_PreloadYStaticText",wxStaticText);
  m_PreloadYSpinCtrl = XRCCTRL(*this,"m_PreloadYSpinCtrl",wxSpinCtrlDouble);
  m_PreloadXRadioBox = XRCCTRL(*this,"m_PreloadXRadioBox",wxRadioBox);
  m_PreloadXSpinCtrl = XRCCTRL(*this,"m_PreloadXSpinCtrl",wxSpinCtrlDouble);
  m_PreloadLimitStaticText = XRCCTRL(*this,"m_PreloadLimitStaticText",wxStaticText);
  m_PreloadLimitSpinCtrl = XRCCTRL(*this,"m_PreloadLimitSpinCtrl",wxSpinCtrlDouble);
  m_PreloadSpeedStaticText = XRCCTRL(*this,"m_PreloadSpeedStaticText",wxStaticText);
  m_PreloadSpeedSpinCtrl = XRCCTRL(*this,"m_PreloadSpeedSpinCtrl",wxSpinCtrlDouble);
  wxID_PreloadOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_PreloadCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_ConditioningPanel = XRCCTRL(*this,"m_ConditioningPanel",wxPanel);
  m_ConditioningCyclesStaticText = XRCCTRL(*this,"m_ConditioningCyclesStaticText",wxStaticText);
  m_ConditioningCyclesSpinCtrl = XRCCTRL(*this,"m_ConditioningCyclesSpinCtrl",wxSpinCtrl);
  m_ConditioningSpeedStaticText = XRCCTRL(*this,"m_ConditioningSpeedStaticText",wxStaticText);
  m_ConditioningSpeedPreloadSpinCtrl = XRCCTRL(*this,"m_ConditioningSpeedPreloadSpinCtrl",wxSpinCtrlDouble);
  m_ConditioningSpeedPreloadStaticText = XRCCTRL(*this,"m_ConditioningSpeedPreloadStaticText",wxStaticText);
  m_ConditioningSpeedMmSpinCtrl = XRCCTRL(*this,"m_ConditioningSpeedMmSpinCtrl",wxSpinCtrlDouble);
  m_ConditioningSpeedMMStaticText = XRCCTRL(*this,"m_ConditioningSpeedMMStaticText",wxStaticText);
  m_ConditioningStressForceLimitStaticText = XRCCTRL(*this,"m_ConditioningStressForceLimitStaticText",wxStaticText);
  m_ConditioningStressForceLimitSpinCtrl = XRCCTRL(*this,"m_ConditioningStressForceLimitSpinCtrl",wxSpinCtrlDouble);
  m_ConditioningStressRadioBtn = XRCCTRL(*this,"m_ConditioningStressRadioBtn",wxRadioButton);
  m_ConditioningDistanceRadioBtn = XRCCTRL(*this,"m_ConditioningDistanceRadioBtn",wxRadioButton);
  m_ConditioningDistanceLimitStaticText = XRCCTRL(*this,"m_ConditioningDistanceLimitStaticText",wxStaticText);
  m_ConditioningDistanceLimitSpinCtrl = XRCCTRL(*this,"m_ConditioningDistanceLimitSpinCtrl",wxSpinCtrlDouble);
  m_ConditioningDisctanceLimitRadioBox = XRCCTRL(*this,"m_ConditioningDisctanceLimitRadioBox",wxRadioBox);
  wxID_ConditioningOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_ConditioningCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_Ramp2FailurePanel = XRCCTRL(*this,"m_Ramp2FailurePanel",wxPanel);
  m_R2FSpeedStaticText = XRCCTRL(*this,"m_R2FSpeedStaticText",wxStaticText);
  m_R2FSpeedPreloadSpinCtrl = XRCCTRL(*this,"m_R2FSpeedPreloadSpinCtrl",wxSpinCtrlDouble);
  m_R2FSpeedPreloadStaticText = XRCCTRL(*this,"m_R2FSpeedPreloadStaticText",wxStaticText);
  m_R2FSpeedMmSpinCtrl = XRCCTRL(*this,"m_R2FSpeedMmSpinCtrl",wxSpinCtrlDouble);
  m_R2FSpeedMmStaticText = XRCCTRL(*this,"m_R2FSpeedMmStaticText",wxStaticText);
  m_R2FDropBStopStaticText = XRCCTRL(*this,"m_R2FDropBStopStaticText",wxStaticText);
  m_R2FDropBStopSpinCtrl = XRCCTRL(*this,"m_R2FDropBStopSpinCtrl",wxSpinCtrlDouble);
  m_R2FAfterFailureStaticText = XRCCTRL(*this,"m_R2FAfterFailureStaticText",wxStaticText);
  m_R2FAfterFailureRadioBox = XRCCTRL(*this,"m_R2FAfterFailureRadioBox",wxRadioBox);
  m_R2FGoToSpinCtrl = XRCCTRL(*this,"m_R2FGoToSpinCtrl",wxSpinCtrlDouble);
  m_R2FGoToRadioBox = XRCCTRL(*this,"m_R2FGoToRadioBox",wxRadioBox);
  wxID_R2FOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_R2FCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_RelaxationPanel = XRCCTRL(*this,"m_RelaxationPanel",wxPanel);
  m_RelaxationRampStaticText = XRCCTRL(*this,"m_RelaxationRampStaticText",wxStaticText);
  m_RelaxationRampSpinCtrl = XRCCTRL(*this,"m_RelaxationRampSpinCtrl",wxSpinCtrlDouble);
  m_RelaxationRampRadioBox = XRCCTRL(*this,"m_RelaxationRampRadioBox",wxRadioBox);
  m_staticText321 = XRCCTRL(*this,"m_staticText321",wxStaticText);
  m_RelaxationPauseStaticText = XRCCTRL(*this,"m_RelaxationPauseStaticText",wxStaticText);
  m_RelaxationPauseSpinCtrl = XRCCTRL(*this,"m_RelaxationPauseSpinCtrl",wxSpinCtrlDouble);
  m_RelaxationStepsStaticText = XRCCTRL(*this,"m_RelaxationStepsStaticText",wxStaticText);
  m_RelaxationStepsSpinCtrl = XRCCTRL(*this,"m_RelaxationStepsSpinCtrl",wxSpinCtrl);
  wxID_RelaxationOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_RelaxationCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_CreepPanel = XRCCTRL(*this,"m_CreepPanel",wxPanel);
  m_CreepSpeedStaticText = XRCCTRL(*this,"m_CreepSpeedStaticText",wxStaticText);
  m_CreepSpeedPreloadSpinCtrl = XRCCTRL(*this,"m_CreepSpeedPreloadSpinCtrl",wxSpinCtrlDouble);
  m_CreepSpeedPreloadStaticText = XRCCTRL(*this,"m_CreepSpeedPreloadStaticText",wxStaticText);
  m_CreepSpeedMmSpinCtrl = XRCCTRL(*this,"m_CreepSpeedMmSpinCtrl",wxSpinCtrlDouble);
  m_CreepSpeedMmStaticText = XRCCTRL(*this,"m_CreepSpeedMmStaticText",wxStaticText);
  m_CreepHoldForceStressStaticText = XRCCTRL(*this,"m_CreepHoldForceStressStaticText",wxStaticText);
  m_CreepHoldForceStressSpinCtrl = XRCCTRL(*this,"m_CreepHoldForceStressSpinCtrl",wxSpinCtrlDouble);
  m_CreepHoldTimeStaticText = XRCCTRL(*this,"m_CreepHoldTimeStaticText",wxStaticText);
  m_CreepHoldTimeSpinCtrl = XRCCTRL(*this,"m_CreepHoldTimeSpinCtrl",wxSpinCtrlDouble);
  wxID_CreepOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_CreepCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_FatiguePanel = XRCCTRL(*this,"m_FatiguePanel",wxPanel);
  m_FatigueCyclesStaticText = XRCCTRL(*this,"m_FatigueCyclesStaticText",wxStaticText);
  m_FatigueCyclesSpinCtrl = XRCCTRL(*this,"m_FatigueCyclesSpinCtrl",wxSpinCtrl);
  m_FatigueTotalTimeStaticText = XRCCTRL(*this,"m_FatigueTotalTimeStaticText",wxStaticText);
  m_FatigueTotalTimeSpinCtrl = XRCCTRL(*this,"m_FatigueTotalTimeSpinCtrl",wxSpinCtrlDouble);
  m_FatigueAmplitudeTimeStaticText = XRCCTRL(*this,"m_FatigueAmplitudeTimeStaticText",wxStaticText);
  m_FatigueAmplitudeSpinCtrl = XRCCTRL(*this,"m_FatigueAmplitudeSpinCtrl",wxSpinCtrlDouble);
  m_FatigueAmplitudeRadioBox = XRCCTRL(*this,"m_FatigueAmplitudeRadioBox",wxRadioBox);
  m_staticText33 = XRCCTRL(*this,"m_staticText33",wxStaticText);
  m_FatigueRestTimeStaticText = XRCCTRL(*this,"m_FatigueRestTimeStaticText",wxStaticText);
  m_FatigueRestTimeSpinCtrl = XRCCTRL(*this,"m_FatigueRestTimeSpinCtrl",wxSpinCtrlDouble);
  m_FatigueFrequencyStaticText = XRCCTRL(*this,"m_FatigueFrequencyStaticText",wxStaticText);
  m_FatigueFrequencySpinCtrl = XRCCTRL(*this,"m_FatigueFrequencySpinCtrl",wxSpinCtrlDouble);
  m_FatigueLoopCheckBox = XRCCTRL(*this,"m_FatigueLoopCheckBox",wxCheckBox);
  wxID_FatigueOK = XRCCTRL(*this,"wxID_OK",wxButton);
  wxID_FatigueCANCEL = XRCCTRL(*this,"wxID_CANCEL",wxButton);
  m_DecreaseDistanceButton = XRCCTRL(*this,"m_DecreaseDistanceButton",wxButton);
  m_IncreaseDistanceButton = XRCCTRL(*this,"m_IncreaseDistanceButton",wxButton);
  m_StopButton = XRCCTRL(*this,"m_StopButton",wxButton);
  m_ForceStaticText = XRCCTRL(*this,"m_ForceStaticText",wxStaticText);
  m_Protocols = XRCCTRL(*this,"m_Protocols",wxPanel);
  m_ProtocolsListBox = XRCCTRL(*this,"m_ProtocolsListBox",wxListBox);
  m_ProtocolsNavigationPanel = XRCCTRL(*this,"m_ProtocolsNavigationPanel",wxPanel);
  m_ProtocolsExperiment1Button = XRCCTRL(*this,"m_ProtocolsExperiment1Button",wxButton);
  m_staticText32 = XRCCTRL(*this,"m_staticText32",wxStaticText);
  m_ProtocolsExperiment2Button = XRCCTRL(*this,"m_ProtocolsExperiment2Button",wxButton);
  m_ProtocolsUpButton = XRCCTRL(*this,"m_ProtocolsUpButton",wxButton);
  m_ProtocolsExperiment3Button = XRCCTRL(*this,"m_ProtocolsExperiment3Button",wxButton);
  m_ProtocolsXButton = XRCCTRL(*this,"m_ProtocolsXButton",wxButton);
  m_ProtocolsExperiment4Button = XRCCTRL(*this,"m_ProtocolsExperiment4Button",wxButton);
  m_ProtocolsDownButton = XRCCTRL(*this,"m_ProtocolsDownButton",wxButton);
  m_ProtocolsExperiment5Button = XRCCTRL(*this,"m_ProtocolsExperiment5Button",wxButton);
  m_checkBox3 = XRCCTRL(*this,"m_checkBox3",wxCheckBox);
  m_Results = XRCCTRL(*this,"m_Results",wxScrolledWindow);
  m_ResultListCtrl = XRCCTRL(*this,"m_ResultListCtrl",wxListCtrl);
  m_ResultSaveButton = XRCCTRL(*this,"m_ResultSaveButton",wxButton);
  m_Graph = XRCCTRL(*this,"m_Graph",wxPanel);
 }
public:
MyFrame_Base(const wxString& title, wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
