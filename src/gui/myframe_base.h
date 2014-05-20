//
// This file was automatically generated by wxrc, do not edit by hand.
//

#ifndef __myframe_base_h__
#define __myframe_base_h__

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "mybutton.h"
#include <wx/spinctrl.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/scrolwin.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <mathplot.h>
#include <wx/xrc/xmlres.h>

class MyFrame_Base : public wxFrame {
protected:
 wxNotebook* m_Experiments;
 wxPanel* m_InitializePanel1;
 wxPanel* m_InitializePanel21;
 wxStaticText* m_InitializeStaticText1;
 wxButton* m_InitializeLoadStoredPositionButton;
 wxStaticText* m_InitializeStaticText2;
 wxStaticText* m_InitializeWarningStaticText;
 wxButton* m_InitializeHomeMotorsButton;
 wxPanel* m_InitializePanel22;
 wxStaticText* m_InitializeMoveStaticText;
 wxButton* m_InitializeMountingLengthButton;
 wxRadioBox* m_InitializeUnitRadioBox;
 wxPanel* m_InitializeCalculateDiameterPanel;
 wxStaticText* m_InitializeCrossSectionStaticText;
 wxSpinCtrlDouble* m_InitializeCrossSectionSpinCtrl;
 wxPanel* m_LimitsPanel;
 wxPanel* m_LimitsLimitPanel;
 wxStaticText* m_LimitsLimitStaticText60;
 wxStaticText* m_staticText61;
 wxStaticText* m_staticText71;
 wxStaticText* m_LimitsLimitMaxDistanceStaticText;
 wxSpinCtrlDouble* m_LimitsLimitMaxDistanceSpinCtrl;
 wxButton* m_LimitsLimitSet1Button;
 wxStaticText* m_LimitsLimitMinDistanceStaticText;
 wxSpinCtrlDouble* m_LimitsLimitMinDistanceSpinCtrl;
 wxButton* m_LimitsLimitSet2Button;
 wxStaticText* m_LimitsLimitMaxForceStaticText;
 wxSpinCtrlDouble* m_LimitsLimitMaxForceSpinCtrl;
 wxButton* m_LimitsLimitSet3Button;
 wxStaticText* m_LimitsLimitMinForceStaticText;
 wxSpinCtrlDouble* m_LimitsLimitMinForceSpinCtrl;
 wxButton* m_LimitsLimitSet4Button;
 wxButton* m_LimitsLimitSetButton;
 wxPanel* m_LimitsL0Panel;
 wxStaticText* m_LimitsGoToStaticText;
 wxSpinCtrlDouble* m_LimitsGoToSpinCtrl;
 wxButton* m_LimitsGoToButton;
 wxButton* m_LimitsSetL0Button;
 wxPanel* m_PreloadPanel;
 wxPanel* m_PreloadCalculateDiameterPanel;
 wxStaticText* m_PreloadLimitStaticText;
 wxSpinCtrlDouble* m_PreloadLimitSpinCtrl;
 wxStaticText* m_PreloadSpeedStaticText;
 wxPanel* m_PreloadSpeedPanel;
 wxSpinCtrlDouble* m_PreloadSpeedPreloadSpinCtrl;
 wxStaticText* m_PreloadSpeedPreloadStaticText;
 wxSpinCtrlDouble* m_PreloadSpeedMmSpinCtrl;
 wxStaticText* m_PreloadSpeedMmStaticText;
 wxButton* m_PreloadCancelButton;
 wxButton* m_PreloadSendButton;
 wxPanel* m_OneStepPanel1;
 wxPanel* m_OneStepPanel21;
 wxRadioButton* m_OneStepStressForceRadioBtn;
 wxRadioButton* m_OneStepDistanceRadioBtn;
 wxPanel* m_OneStepStressForcePanel;
 wxStaticText* m_OneStepStressForceVelocityStaticText;
 wxSpinCtrlDouble* m_OneStepStressForceVelocitySpinCtrl;
 wxRadioButton* m_OneStepStressForceVelocityMmRadioBtn;
 wxRadioButton* m_OneStepStressForceVelocityPercentRadioBtn;
 wxStaticText* m_OneStepStressForceHoldTime1StaticText;
 wxSpinCtrlDouble* m_OneStepStressForceHoldTime1SpinCtrl;
 wxStaticText* m_staticText78;
 wxStaticText* m_OneStepStressForceUpperLimitStaticText;
 wxSpinCtrlDouble* m_OneStepStressForceUpperLimitSpinCtrl;
 wxStaticText* m_staticText80;
 wxStaticText* m_OneStepStressForceHoldTime2StaticText;
 wxSpinCtrlDouble* m_OneStepStressForceHoldTime2SpinCtrl;
 wxStaticText* m_staticText82;
 wxStaticText* m_OneStepStressForceLowerLimitStaticText;
 wxSpinCtrlDouble* m_OneStepStressForceLowerLimitSpinCtrl;
 wxCheckBox* m_OneStepStressForcePhotoCheckBox;
 wxPanel* m_OneStepDistancePanel;
 wxStaticText* m_OneStepDistanceVelocityStaticText;
 wxSpinCtrlDouble* m_OneStepDistanceVelocitySpinCtrl;
 wxRadioButton* m_OneStepDistanceVelocityMmRadioBtn;
 wxRadioButton* m_OneStepDistanceVelocityPercentRadioBtn;
 wxStaticText* m_OneStepDistanceHoldTime1StaticText;
 wxSpinCtrlDouble* m_OneStepDistanceHoldTime1SpinCtrl;
 wxStaticText* m_staticText781;
 wxStaticText* m_OneStepDistanceUpperLimitStaticText;
 wxSpinCtrlDouble* m_OneStepDistanceUpperLimitSpinCtrl;
 wxRadioButton* m_OneStepDistanceUpperLimitMmRadioBtn;
 wxRadioButton* m_OneStepDistanceUpperLimitPercentRadioBtn;
 wxStaticText* m_OneStepDistanceHoldTime2StaticText;
 wxSpinCtrlDouble* m_OneStepDistanceHoldTime2SpinCtrl;
 wxStaticText* m_staticText821;
 wxStaticText* m_OneStepDistanceLowerLimitStaticText;
 wxSpinCtrlDouble* m_OneStepDistanceLowerLimitSpinCtrl;
 wxRadioButton* m_OneStepDistanceLowerLimitMmRadioBtn;
 wxRadioButton* m_OneStepDistanceLowerLimitPercentRadioBtn;
 wxCheckBox* m_OneStepDistancePhotoCheckBox;
 wxPanel* m_OneStepPanel22;
 wxCheckBox* m_OneStepEndOfEventRepeatCheckBox;
 wxSpinCtrl* m_OneStepEndOfEventRepeatSpinCtrl;
 wxStaticText* m_staticText75;
 wxStaticText* m_OneStepEndOfEventStaticText;
 wxStaticText* m_staticText711;
 wxStaticText* m_staticText72;
 wxRadioButton* m_OneStepEndOfEventHoldRadioBtn;
 wxSpinCtrlDouble* m_OneStepEndOfEventHoldSpinCtrl;
 wxRadioButton* m_OneStepEndOfEventHoldMmRadioBtn;
 wxRadioButton* m_OneStepEndOfEventHoldPercentRadioBtn;
 wxRadioButton* m_OneStepEndOfEventL0RadioBtn;
 wxStaticText* m_staticText142;
 wxButton* m_OneStepCancelButton;
 wxButton* m_OneStepSendButton;
 wxPanel* m_ContinuosPanel;
 wxPanel* m_ContinuousPanel21;
 wxRadioButton* m_ContinuousStressForceRadioBtn;
 wxRadioButton* m_ContinuousDistanceRadioBtn;
 wxPanel* m_ContinuousStressForcePanel;
 wxStaticText* m_ContinuousStressForceVelocityStaticText;
 wxSpinCtrlDouble* m_ContinuousStressForceVelocitySpinCtrl;
 wxRadioButton* m_ContinuousStressForceVelocityMmRadioBtn;
 wxRadioButton* m_ContinuousVelocityPercentRadioBtn;
 wxStaticText* m_ContinuousStressForceHoldTime1StaticText;
 wxSpinCtrlDouble* m_ContinuousStressForceHoldTime1SpinCtrl;
 wxStaticText* m_staticText782;
 wxStaticText* m_ContinuousStressForceIncrementStaticText;
 wxSpinCtrlDouble* m_ContinuousStressForceIncrementSpinCtrl;
 wxStaticText* m_staticText802;
 wxStaticText* m_ContinuousStressForceMaxValueStaticText;
 wxSpinCtrlDouble* m_ContinuousStressForceMaxValueSpinCtrl;
 wxCheckBox* m_ContinuousStressForcePhotoCheckBox;
 wxPanel* m_ContinuousDistancePanel;
 wxStaticText* m_ContinuousDistanceVelocityStaticText;
 wxSpinCtrlDouble* m_ContinuousDistanceVelocitySpinCtrl;
 wxRadioButton* m_ContinuousDistanceVelocityMmRadioBtn;
 wxRadioButton* m_ContinuousDistanceVelocityPercentRadioBtn;
 wxStaticText* m_ContinuousDistanceHoldTime1StaticText;
 wxSpinCtrlDouble* m_ContinuousDistanceHoldTime1SpinCtrl;
 wxStaticText* m_staticText7811;
 wxStaticText* m_ContinuousDistanceIncrementStaticText;
 wxSpinCtrlDouble* m_ContinuousDistanceIncrementSpinCtrl;
 wxStaticText* m_staticText8011;
 wxStaticText* m_ContinuousDistanceMaxValueStaticText;
 wxSpinCtrlDouble* m_ContinuousDistanceMaxValueSpinCtrl;
 wxRadioButton* m_ContinuousDistanceMaxValueMmRadioBtn1;
 wxRadioButton* m_ContinuousDistanceMaxValuePercentRadioBtn1;
 wxCheckBox* m_ContinuousDistancePhotoCheckBox;
 wxPanel* m_ContinuousPanel22;
 wxCheckBox* m_ContinuousEndOfEventRepeatCheckBox;
 wxSpinCtrl* m_ContinuousEndOfEventRepeatSpinCtrl;
 wxStaticText* m_OneStepEndOfEventStaticText1;
 wxStaticText* m_staticText7111;
 wxRadioButton* m_ContinuousEndOfEventHoldRadioBtn;
 wxStaticText* m_staticText140;
 wxRadioButton* m_ContinuousEndOfEventL0RadioBtn;
 wxStaticText* m_staticText143;
 wxButton* m_ContinuousCancelButton;
 wxButton* m_ContinuousSendButton;
 wxPanel* m_ConditioningPanel;
 wxStaticText* m_ConditioningCyclesStaticText;
 wxSpinCtrl* m_ConditioningCyclesSpinCtrl;
 wxStaticText* m_ConditioningSpeedStaticText;
 wxPanel* m_ConditioningSpeedPanel;
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
 wxButton* m_ConditioningCancelButton;
 wxButton* m_ConditioningSendButton;
 wxPanel* m_Ramp2FailurePanel;
 wxStaticText* m_R2FSpeedStaticText;
 wxPanel* m_R2FSpeedPanel;
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
 wxButton* m_R2FCancelButton;
 wxButton* m_R2FSendButton;
 wxMyButton* m_DecreaseDistanceButton;
 wxMyButton* m_IncreaseDistanceButton;
 wxButton* m_StopButton;
 wxStaticText* m_ForceStaticText;
 wxStaticText* m_DistanceStaticText;
 wxPanel* m_Protocols;
 wxListBox* m_ProtocolsListBox;
 wxPanel* m_ProtocolsNavigationPanel;
 wxButton* m_ProtocolsUpButton;
 wxButton* m_ProtocolsPhotoButton;
 wxButton* m_ProtocolsXButton;
 wxButton* m_ProtocolsPauseButton;
 wxButton* m_ProtocolsDownButton;
 wxStaticText* m_staticText325;
 wxCheckBox* m_ProtocolsLoopCheckBox;
 wxButton* m_ProtocolsPreviewButton;
 wxButton* m_ProtocolsRunButton;
 wxButton* m_ProtocolsStopButton;
 wxButton* m_ProtocolsSaveButton;
 wxButton* m_ProtocolsLoadButton;
 wxScrolledWindow* m_Results;
 wxListCtrl* m_ResultListCtrl;
 wxButton* m_ResultSaveButton;
 wxPanel* m_GraphPanel;
 wxBoxSizer* m_GraphSizer1;
 wxBoxSizer* m_GraphSizer2;
 wxButton* m_GraphExportCSVButton;
 wxButton* m_GraphExportPNGButton;
 wxButton* m_GraphClearButton;

private:
 void InitWidgetsFromXRC(wxWindow *parent){
  wxXmlResource::Get()->LoadObject(this,parent,wxT("MyFrame_Base"), wxT("wxFrame"));
  m_Experiments = XRCCTRL(*this,"m_Experiments",wxNotebook);
  m_InitializePanel1 = XRCCTRL(*this,"m_InitializePanel1",wxPanel);
  m_InitializePanel21 = XRCCTRL(*this,"m_InitializePanel21",wxPanel);
  m_InitializeStaticText1 = XRCCTRL(*this,"m_InitializeStaticText1",wxStaticText);
  m_InitializeLoadStoredPositionButton = XRCCTRL(*this,"m_InitializeLoadStoredPositionButton",wxButton);
  m_InitializeStaticText2 = XRCCTRL(*this,"m_InitializeStaticText2",wxStaticText);
  m_InitializeWarningStaticText = XRCCTRL(*this,"m_InitializeWarningStaticText",wxStaticText);
  m_InitializeHomeMotorsButton = XRCCTRL(*this,"m_InitializeHomeMotorsButton",wxButton);
  m_InitializePanel22 = XRCCTRL(*this,"m_InitializePanel22",wxPanel);
  m_InitializeMoveStaticText = XRCCTRL(*this,"m_InitializeMoveStaticText",wxStaticText);
  m_InitializeMountingLengthButton = XRCCTRL(*this,"m_InitializeMountingLengthButton",wxButton);
  m_InitializeUnitRadioBox = XRCCTRL(*this,"m_InitializeUnitRadioBox",wxRadioBox);
  m_InitializeCalculateDiameterPanel = XRCCTRL(*this,"m_InitializeCalculateDiameterPanel",wxPanel);
  m_InitializeCrossSectionStaticText = XRCCTRL(*this,"m_InitializeCrossSectionStaticText",wxStaticText);
  m_InitializeCrossSectionSpinCtrl = XRCCTRL(*this,"m_InitializeCrossSectionSpinCtrl",wxSpinCtrlDouble);
  m_LimitsPanel = XRCCTRL(*this,"m_LimitsPanel",wxPanel);
  m_LimitsLimitPanel = XRCCTRL(*this,"m_LimitsLimitPanel",wxPanel);
  m_LimitsLimitStaticText60 = XRCCTRL(*this,"m_LimitsLimitStaticText60",wxStaticText);
  m_staticText61 = XRCCTRL(*this,"m_staticText61",wxStaticText);
  m_staticText71 = XRCCTRL(*this,"m_staticText71",wxStaticText);
  m_LimitsLimitMaxDistanceStaticText = XRCCTRL(*this,"m_LimitsLimitMaxDistanceStaticText",wxStaticText);
  m_LimitsLimitMaxDistanceSpinCtrl = XRCCTRL(*this,"m_LimitsLimitMaxDistanceSpinCtrl",wxSpinCtrlDouble);
  m_LimitsLimitSet1Button = XRCCTRL(*this,"m_LimitsLimitSet1Button",wxButton);
  m_LimitsLimitMinDistanceStaticText = XRCCTRL(*this,"m_LimitsLimitMinDistanceStaticText",wxStaticText);
  m_LimitsLimitMinDistanceSpinCtrl = XRCCTRL(*this,"m_LimitsLimitMinDistanceSpinCtrl",wxSpinCtrlDouble);
  m_LimitsLimitSet2Button = XRCCTRL(*this,"m_LimitsLimitSet2Button",wxButton);
  m_LimitsLimitMaxForceStaticText = XRCCTRL(*this,"m_LimitsLimitMaxForceStaticText",wxStaticText);
  m_LimitsLimitMaxForceSpinCtrl = XRCCTRL(*this,"m_LimitsLimitMaxForceSpinCtrl",wxSpinCtrlDouble);
  m_LimitsLimitSet3Button = XRCCTRL(*this,"m_LimitsLimitSet3Button",wxButton);
  m_LimitsLimitMinForceStaticText = XRCCTRL(*this,"m_LimitsLimitMinForceStaticText",wxStaticText);
  m_LimitsLimitMinForceSpinCtrl = XRCCTRL(*this,"m_LimitsLimitMinForceSpinCtrl",wxSpinCtrlDouble);
  m_LimitsLimitSet4Button = XRCCTRL(*this,"m_LimitsLimitSet4Button",wxButton);
  m_LimitsLimitSetButton = XRCCTRL(*this,"m_LimitsLimitSetButton",wxButton);
  m_LimitsL0Panel = XRCCTRL(*this,"m_LimitsL0Panel",wxPanel);
  m_LimitsGoToStaticText = XRCCTRL(*this,"m_LimitsGoToStaticText",wxStaticText);
  m_LimitsGoToSpinCtrl = XRCCTRL(*this,"m_LimitsGoToSpinCtrl",wxSpinCtrlDouble);
  m_LimitsGoToButton = XRCCTRL(*this,"m_LimitsGoToButton",wxButton);
  m_LimitsSetL0Button = XRCCTRL(*this,"m_LimitsSetL0Button",wxButton);
  m_PreloadPanel = XRCCTRL(*this,"m_PreloadPanel",wxPanel);
  m_PreloadCalculateDiameterPanel = XRCCTRL(*this,"m_PreloadCalculateDiameterPanel",wxPanel);
  m_PreloadLimitStaticText = XRCCTRL(*this,"m_PreloadLimitStaticText",wxStaticText);
  m_PreloadLimitSpinCtrl = XRCCTRL(*this,"m_PreloadLimitSpinCtrl",wxSpinCtrlDouble);
  m_PreloadSpeedStaticText = XRCCTRL(*this,"m_PreloadSpeedStaticText",wxStaticText);
  m_PreloadSpeedPanel = XRCCTRL(*this,"m_PreloadSpeedPanel",wxPanel);
  m_PreloadSpeedPreloadSpinCtrl = XRCCTRL(*this,"m_PreloadSpeedPreloadSpinCtrl",wxSpinCtrlDouble);
  m_PreloadSpeedPreloadStaticText = XRCCTRL(*this,"m_PreloadSpeedPreloadStaticText",wxStaticText);
  m_PreloadSpeedMmSpinCtrl = XRCCTRL(*this,"m_PreloadSpeedMmSpinCtrl",wxSpinCtrlDouble);
  m_PreloadSpeedMmStaticText = XRCCTRL(*this,"m_PreloadSpeedMmStaticText",wxStaticText);
  m_PreloadCancelButton = XRCCTRL(*this,"m_PreloadCancelButton",wxButton);
  m_PreloadSendButton = XRCCTRL(*this,"m_PreloadSendButton",wxButton);
  m_OneStepPanel1 = XRCCTRL(*this,"m_OneStepPanel1",wxPanel);
  m_OneStepPanel21 = XRCCTRL(*this,"m_OneStepPanel21",wxPanel);
  m_OneStepStressForceRadioBtn = XRCCTRL(*this,"m_OneStepStressForceRadioBtn",wxRadioButton);
  m_OneStepDistanceRadioBtn = XRCCTRL(*this,"m_OneStepDistanceRadioBtn",wxRadioButton);
  m_OneStepStressForcePanel = XRCCTRL(*this,"m_OneStepStressForcePanel",wxPanel);
  m_OneStepStressForceVelocityStaticText = XRCCTRL(*this,"m_OneStepStressForceVelocityStaticText",wxStaticText);
  m_OneStepStressForceVelocitySpinCtrl = XRCCTRL(*this,"m_OneStepStressForceVelocitySpinCtrl",wxSpinCtrlDouble);
  m_OneStepStressForceVelocityMmRadioBtn = XRCCTRL(*this,"m_OneStepStressForceVelocityMmRadioBtn",wxRadioButton);
  m_OneStepStressForceVelocityPercentRadioBtn = XRCCTRL(*this,"m_OneStepStressForceVelocityPercentRadioBtn",wxRadioButton);
  m_OneStepStressForceHoldTime1StaticText = XRCCTRL(*this,"m_OneStepStressForceHoldTime1StaticText",wxStaticText);
  m_OneStepStressForceHoldTime1SpinCtrl = XRCCTRL(*this,"m_OneStepStressForceHoldTime1SpinCtrl",wxSpinCtrlDouble);
  m_staticText78 = XRCCTRL(*this,"m_staticText78",wxStaticText);
  m_OneStepStressForceUpperLimitStaticText = XRCCTRL(*this,"m_OneStepStressForceUpperLimitStaticText",wxStaticText);
  m_OneStepStressForceUpperLimitSpinCtrl = XRCCTRL(*this,"m_OneStepStressForceUpperLimitSpinCtrl",wxSpinCtrlDouble);
  m_staticText80 = XRCCTRL(*this,"m_staticText80",wxStaticText);
  m_OneStepStressForceHoldTime2StaticText = XRCCTRL(*this,"m_OneStepStressForceHoldTime2StaticText",wxStaticText);
  m_OneStepStressForceHoldTime2SpinCtrl = XRCCTRL(*this,"m_OneStepStressForceHoldTime2SpinCtrl",wxSpinCtrlDouble);
  m_staticText82 = XRCCTRL(*this,"m_staticText82",wxStaticText);
  m_OneStepStressForceLowerLimitStaticText = XRCCTRL(*this,"m_OneStepStressForceLowerLimitStaticText",wxStaticText);
  m_OneStepStressForceLowerLimitSpinCtrl = XRCCTRL(*this,"m_OneStepStressForceLowerLimitSpinCtrl",wxSpinCtrlDouble);
  m_OneStepStressForcePhotoCheckBox = XRCCTRL(*this,"m_OneStepStressForcePhotoCheckBox",wxCheckBox);
  m_OneStepDistancePanel = XRCCTRL(*this,"m_OneStepDistancePanel",wxPanel);
  m_OneStepDistanceVelocityStaticText = XRCCTRL(*this,"m_OneStepDistanceVelocityStaticText",wxStaticText);
  m_OneStepDistanceVelocitySpinCtrl = XRCCTRL(*this,"m_OneStepDistanceVelocitySpinCtrl",wxSpinCtrlDouble);
  m_OneStepDistanceVelocityMmRadioBtn = XRCCTRL(*this,"m_OneStepDistanceVelocityMmRadioBtn",wxRadioButton);
  m_OneStepDistanceVelocityPercentRadioBtn = XRCCTRL(*this,"m_OneStepDistanceVelocityPercentRadioBtn",wxRadioButton);
  m_OneStepDistanceHoldTime1StaticText = XRCCTRL(*this,"m_OneStepDistanceHoldTime1StaticText",wxStaticText);
  m_OneStepDistanceHoldTime1SpinCtrl = XRCCTRL(*this,"m_OneStepDistanceHoldTime1SpinCtrl",wxSpinCtrlDouble);
  m_staticText781 = XRCCTRL(*this,"m_staticText781",wxStaticText);
  m_OneStepDistanceUpperLimitStaticText = XRCCTRL(*this,"m_OneStepDistanceUpperLimitStaticText",wxStaticText);
  m_OneStepDistanceUpperLimitSpinCtrl = XRCCTRL(*this,"m_OneStepDistanceUpperLimitSpinCtrl",wxSpinCtrlDouble);
  m_OneStepDistanceUpperLimitMmRadioBtn = XRCCTRL(*this,"m_OneStepDistanceUpperLimitMmRadioBtn",wxRadioButton);
  m_OneStepDistanceUpperLimitPercentRadioBtn = XRCCTRL(*this,"m_OneStepDistanceUpperLimitPercentRadioBtn",wxRadioButton);
  m_OneStepDistanceHoldTime2StaticText = XRCCTRL(*this,"m_OneStepDistanceHoldTime2StaticText",wxStaticText);
  m_OneStepDistanceHoldTime2SpinCtrl = XRCCTRL(*this,"m_OneStepDistanceHoldTime2SpinCtrl",wxSpinCtrlDouble);
  m_staticText821 = XRCCTRL(*this,"m_staticText821",wxStaticText);
  m_OneStepDistanceLowerLimitStaticText = XRCCTRL(*this,"m_OneStepDistanceLowerLimitStaticText",wxStaticText);
  m_OneStepDistanceLowerLimitSpinCtrl = XRCCTRL(*this,"m_OneStepDistanceLowerLimitSpinCtrl",wxSpinCtrlDouble);
  m_OneStepDistanceLowerLimitMmRadioBtn = XRCCTRL(*this,"m_OneStepDistanceLowerLimitMmRadioBtn",wxRadioButton);
  m_OneStepDistanceLowerLimitPercentRadioBtn = XRCCTRL(*this,"m_OneStepDistanceLowerLimitPercentRadioBtn",wxRadioButton);
  m_OneStepDistancePhotoCheckBox = XRCCTRL(*this,"m_OneStepDistancePhotoCheckBox",wxCheckBox);
  m_OneStepPanel22 = XRCCTRL(*this,"m_OneStepPanel22",wxPanel);
  m_OneStepEndOfEventRepeatCheckBox = XRCCTRL(*this,"m_OneStepEndOfEventRepeatCheckBox",wxCheckBox);
  m_OneStepEndOfEventRepeatSpinCtrl = XRCCTRL(*this,"m_OneStepEndOfEventRepeatSpinCtrl",wxSpinCtrl);
  m_staticText75 = XRCCTRL(*this,"m_staticText75",wxStaticText);
  m_OneStepEndOfEventStaticText = XRCCTRL(*this,"m_OneStepEndOfEventStaticText",wxStaticText);
  m_staticText711 = XRCCTRL(*this,"m_staticText711",wxStaticText);
  m_staticText72 = XRCCTRL(*this,"m_staticText72",wxStaticText);
  m_OneStepEndOfEventHoldRadioBtn = XRCCTRL(*this,"m_OneStepEndOfEventHoldRadioBtn",wxRadioButton);
  m_OneStepEndOfEventHoldSpinCtrl = XRCCTRL(*this,"m_OneStepEndOfEventHoldSpinCtrl",wxSpinCtrlDouble);
  m_OneStepEndOfEventHoldMmRadioBtn = XRCCTRL(*this,"m_OneStepEndOfEventHoldMmRadioBtn",wxRadioButton);
  m_OneStepEndOfEventHoldPercentRadioBtn = XRCCTRL(*this,"m_OneStepEndOfEventHoldPercentRadioBtn",wxRadioButton);
  m_OneStepEndOfEventL0RadioBtn = XRCCTRL(*this,"m_OneStepEndOfEventL0RadioBtn",wxRadioButton);
  m_staticText142 = XRCCTRL(*this,"m_staticText142",wxStaticText);
  m_OneStepCancelButton = XRCCTRL(*this,"m_OneStepCancelButton",wxButton);
  m_OneStepSendButton = XRCCTRL(*this,"m_OneStepSendButton",wxButton);
  m_ContinuosPanel = XRCCTRL(*this,"m_ContinuosPanel",wxPanel);
  m_ContinuousPanel21 = XRCCTRL(*this,"m_ContinuousPanel21",wxPanel);
  m_ContinuousStressForceRadioBtn = XRCCTRL(*this,"m_ContinuousStressForceRadioBtn",wxRadioButton);
  m_ContinuousDistanceRadioBtn = XRCCTRL(*this,"m_ContinuousDistanceRadioBtn",wxRadioButton);
  m_ContinuousStressForcePanel = XRCCTRL(*this,"m_ContinuousStressForcePanel",wxPanel);
  m_ContinuousStressForceVelocityStaticText = XRCCTRL(*this,"m_ContinuousStressForceVelocityStaticText",wxStaticText);
  m_ContinuousStressForceVelocitySpinCtrl = XRCCTRL(*this,"m_ContinuousStressForceVelocitySpinCtrl",wxSpinCtrlDouble);
  m_ContinuousStressForceVelocityMmRadioBtn = XRCCTRL(*this,"m_ContinuousStressForceVelocityMmRadioBtn",wxRadioButton);
  m_ContinuousVelocityPercentRadioBtn = XRCCTRL(*this,"m_ContinuousVelocityPercentRadioBtn",wxRadioButton);
  m_ContinuousStressForceHoldTime1StaticText = XRCCTRL(*this,"m_ContinuousStressForceHoldTime1StaticText",wxStaticText);
  m_ContinuousStressForceHoldTime1SpinCtrl = XRCCTRL(*this,"m_ContinuousStressForceHoldTime1SpinCtrl",wxSpinCtrlDouble);
  m_staticText782 = XRCCTRL(*this,"m_staticText782",wxStaticText);
  m_ContinuousStressForceIncrementStaticText = XRCCTRL(*this,"m_ContinuousStressForceIncrementStaticText",wxStaticText);
  m_ContinuousStressForceIncrementSpinCtrl = XRCCTRL(*this,"m_ContinuousStressForceIncrementSpinCtrl",wxSpinCtrlDouble);
  m_staticText802 = XRCCTRL(*this,"m_staticText802",wxStaticText);
  m_ContinuousStressForceMaxValueStaticText = XRCCTRL(*this,"m_ContinuousStressForceMaxValueStaticText",wxStaticText);
  m_ContinuousStressForceMaxValueSpinCtrl = XRCCTRL(*this,"m_ContinuousStressForceMaxValueSpinCtrl",wxSpinCtrlDouble);
  m_ContinuousStressForcePhotoCheckBox = XRCCTRL(*this,"m_ContinuousStressForcePhotoCheckBox",wxCheckBox);
  m_ContinuousDistancePanel = XRCCTRL(*this,"m_ContinuousDistancePanel",wxPanel);
  m_ContinuousDistanceVelocityStaticText = XRCCTRL(*this,"m_ContinuousDistanceVelocityStaticText",wxStaticText);
  m_ContinuousDistanceVelocitySpinCtrl = XRCCTRL(*this,"m_ContinuousDistanceVelocitySpinCtrl",wxSpinCtrlDouble);
  m_ContinuousDistanceVelocityMmRadioBtn = XRCCTRL(*this,"m_ContinuousDistanceVelocityMmRadioBtn",wxRadioButton);
  m_ContinuousDistanceVelocityPercentRadioBtn = XRCCTRL(*this,"m_ContinuousDistanceVelocityPercentRadioBtn",wxRadioButton);
  m_ContinuousDistanceHoldTime1StaticText = XRCCTRL(*this,"m_ContinuousDistanceHoldTime1StaticText",wxStaticText);
  m_ContinuousDistanceHoldTime1SpinCtrl = XRCCTRL(*this,"m_ContinuousDistanceHoldTime1SpinCtrl",wxSpinCtrlDouble);
  m_staticText7811 = XRCCTRL(*this,"m_staticText7811",wxStaticText);
  m_ContinuousDistanceIncrementStaticText = XRCCTRL(*this,"m_ContinuousDistanceIncrementStaticText",wxStaticText);
  m_ContinuousDistanceIncrementSpinCtrl = XRCCTRL(*this,"m_ContinuousDistanceIncrementSpinCtrl",wxSpinCtrlDouble);
  m_staticText8011 = XRCCTRL(*this,"m_staticText8011",wxStaticText);
  m_ContinuousDistanceMaxValueStaticText = XRCCTRL(*this,"m_ContinuousDistanceMaxValueStaticText",wxStaticText);
  m_ContinuousDistanceMaxValueSpinCtrl = XRCCTRL(*this,"m_ContinuousDistanceMaxValueSpinCtrl",wxSpinCtrlDouble);
  m_ContinuousDistanceMaxValueMmRadioBtn1 = XRCCTRL(*this,"m_ContinuousDistanceMaxValueMmRadioBtn1",wxRadioButton);
  m_ContinuousDistanceMaxValuePercentRadioBtn1 = XRCCTRL(*this,"m_ContinuousDistanceMaxValuePercentRadioBtn1",wxRadioButton);
  m_ContinuousDistancePhotoCheckBox = XRCCTRL(*this,"m_ContinuousDistancePhotoCheckBox",wxCheckBox);
  m_ContinuousPanel22 = XRCCTRL(*this,"m_ContinuousPanel22",wxPanel);
  m_ContinuousEndOfEventRepeatCheckBox = XRCCTRL(*this,"m_ContinuousEndOfEventRepeatCheckBox",wxCheckBox);
  m_ContinuousEndOfEventRepeatSpinCtrl = XRCCTRL(*this,"m_ContinuousEndOfEventRepeatSpinCtrl",wxSpinCtrl);
  m_OneStepEndOfEventStaticText1 = XRCCTRL(*this,"m_OneStepEndOfEventStaticText1",wxStaticText);
  m_staticText7111 = XRCCTRL(*this,"m_staticText7111",wxStaticText);
  m_ContinuousEndOfEventHoldRadioBtn = XRCCTRL(*this,"m_ContinuousEndOfEventHoldRadioBtn",wxRadioButton);
  m_staticText140 = XRCCTRL(*this,"m_staticText140",wxStaticText);
  m_ContinuousEndOfEventL0RadioBtn = XRCCTRL(*this,"m_ContinuousEndOfEventL0RadioBtn",wxRadioButton);
  m_staticText143 = XRCCTRL(*this,"m_staticText143",wxStaticText);
  m_ContinuousCancelButton = XRCCTRL(*this,"m_ContinuousCancelButton",wxButton);
  m_ContinuousSendButton = XRCCTRL(*this,"m_ContinuousSendButton",wxButton);
  m_ConditioningPanel = XRCCTRL(*this,"m_ConditioningPanel",wxPanel);
  m_ConditioningCyclesStaticText = XRCCTRL(*this,"m_ConditioningCyclesStaticText",wxStaticText);
  m_ConditioningCyclesSpinCtrl = XRCCTRL(*this,"m_ConditioningCyclesSpinCtrl",wxSpinCtrl);
  m_ConditioningSpeedStaticText = XRCCTRL(*this,"m_ConditioningSpeedStaticText",wxStaticText);
  m_ConditioningSpeedPanel = XRCCTRL(*this,"m_ConditioningSpeedPanel",wxPanel);
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
  m_ConditioningCancelButton = XRCCTRL(*this,"m_ConditioningCancelButton",wxButton);
  m_ConditioningSendButton = XRCCTRL(*this,"m_ConditioningSendButton",wxButton);
  m_Ramp2FailurePanel = XRCCTRL(*this,"m_Ramp2FailurePanel",wxPanel);
  m_R2FSpeedStaticText = XRCCTRL(*this,"m_R2FSpeedStaticText",wxStaticText);
  m_R2FSpeedPanel = XRCCTRL(*this,"m_R2FSpeedPanel",wxPanel);
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
  m_R2FCancelButton = XRCCTRL(*this,"m_R2FCancelButton",wxButton);
  m_R2FSendButton = XRCCTRL(*this,"m_R2FSendButton",wxButton);
  m_DecreaseDistanceButton = XRCCTRL(*this,"m_DecreaseDistanceButton",wxMyButton);
  m_IncreaseDistanceButton = XRCCTRL(*this,"m_IncreaseDistanceButton",wxMyButton);
  m_StopButton = XRCCTRL(*this,"m_StopButton",wxButton);
  m_ForceStaticText = XRCCTRL(*this,"m_ForceStaticText",wxStaticText);
  m_DistanceStaticText = XRCCTRL(*this,"m_DistanceStaticText",wxStaticText);
  m_Protocols = XRCCTRL(*this,"m_Protocols",wxPanel);
  m_ProtocolsListBox = XRCCTRL(*this,"m_ProtocolsListBox",wxListBox);
  m_ProtocolsNavigationPanel = XRCCTRL(*this,"m_ProtocolsNavigationPanel",wxPanel);
  m_ProtocolsUpButton = XRCCTRL(*this,"m_ProtocolsUpButton",wxButton);
  m_ProtocolsPhotoButton = XRCCTRL(*this,"m_ProtocolsPhotoButton",wxButton);
  m_ProtocolsXButton = XRCCTRL(*this,"m_ProtocolsXButton",wxButton);
  m_ProtocolsPauseButton = XRCCTRL(*this,"m_ProtocolsPauseButton",wxButton);
  m_ProtocolsDownButton = XRCCTRL(*this,"m_ProtocolsDownButton",wxButton);
  m_staticText325 = XRCCTRL(*this,"m_staticText325",wxStaticText);
  m_ProtocolsLoopCheckBox = XRCCTRL(*this,"m_ProtocolsLoopCheckBox",wxCheckBox);
  m_ProtocolsPreviewButton = XRCCTRL(*this,"m_ProtocolsPreviewButton",wxButton);
  m_ProtocolsRunButton = XRCCTRL(*this,"m_ProtocolsRunButton",wxButton);
  m_ProtocolsStopButton = XRCCTRL(*this,"m_ProtocolsStopButton",wxButton);
  m_ProtocolsSaveButton = XRCCTRL(*this,"m_ProtocolsSaveButton",wxButton);
  m_ProtocolsLoadButton = XRCCTRL(*this,"m_ProtocolsLoadButton",wxButton);
  m_Results = XRCCTRL(*this,"m_Results",wxScrolledWindow);
  m_ResultListCtrl = XRCCTRL(*this,"m_ResultListCtrl",wxListCtrl);
  m_ResultSaveButton = XRCCTRL(*this,"m_ResultSaveButton",wxButton);
  m_GraphPanel = XRCCTRL(*this,"m_GraphPanel",wxPanel);
  m_GraphExportCSVButton = XRCCTRL(*this,"m_GraphExportCSVButton",wxButton);
  m_GraphExportPNGButton = XRCCTRL(*this,"m_GraphExportPNGButton",wxButton);
  m_GraphClearButton = XRCCTRL(*this,"m_GraphClearButton",wxButton);
  m_GraphSizer2 = static_cast<wxBoxSizer*>(m_GraphExportCSVButton->GetContainingSizer());
  m_GraphSizer1 = static_cast<wxBoxSizer*>(m_GraphPanel->GetSizer());
 }
public:
MyFrame_Base(const wxString& title, wxWindow *parent=NULL){
  InitWidgetsFromXRC((wxWindow *)parent);
 }
};

void 
InitXmlResource();
#endif
