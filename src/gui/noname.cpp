///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

MyFrame_Base::MyFrame_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1000,500 ), wxDefaultSize );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bTopSizer;
	bTopSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* m_TopSizer2;
	m_TopSizer2 = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_TopSizer2->AddGrowableCol( 0 );
	m_TopSizer2->AddGrowableRow( 0 );
	m_TopSizer2->SetFlexibleDirection( wxBOTH );
	m_TopSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_Experiments = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializePanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxFlexGridSizer* m_InitializeSizer;
	m_InitializeSizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	m_InitializeSizer->AddGrowableCol( 0 );
	m_InitializeSizer->AddGrowableRow( 3 );
	m_InitializeSizer->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	
	m_InitializeSizer->Add( 0, 10, 1, wxEXPAND, 5 );
	
	m_InitializeWarningStaticText = new wxStaticText( m_InitializePanel, wxID_ANY, wxT("Be sure that nothing is mounted before hitting the button \"Home motors\"."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_InitializeWarningStaticText->Wrap( -1 );
	m_InitializeSizer->Add( m_InitializeWarningStaticText, 1, wxALL|wxEXPAND, 5 );
	
	m_InitializeHomeMotorsButton = new wxButton( m_InitializePanel, wxID_ANY, wxT("Home motors"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeSizer->Add( m_InitializeHomeMotorsButton, 0, wxEXPAND, 5 );
	
	
	m_InitializeSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_InitializePanel->SetSizer( m_InitializeSizer );
	m_InitializePanel->Layout();
	m_InitializeSizer->Fit( m_InitializePanel );
	m_Experiments->AddPage( m_InitializePanel, wxT("Initialize"), false );
	m_ClampingPosPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ClampingPositionSizer1;
	m_ClampingPositionSizer1 = new wxFlexGridSizer( 2, 0, 0, 0 );
	m_ClampingPositionSizer1->SetFlexibleDirection( wxVERTICAL );
	m_ClampingPositionSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_ClampingPositionSizer1->Add( 0, 10, 1, 0, 5 );
	
	wxBoxSizer* m_ClampingPositionSizer2;
	m_ClampingPositionSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ClampingPositionStaticText = new wxStaticText( m_ClampingPosPanel, wxID_ANY, wxT("Clamping Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ClampingPositionStaticText->Wrap( -1 );
	m_ClampingPositionSizer2->Add( m_ClampingPositionStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ClampingPositionSpinCtrl = new wxSpinCtrl( m_ClampingPosPanel, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 7 );
	m_ClampingPositionSizer2->Add( m_ClampingPositionSpinCtrl, 0, wxALL, 5 );
	
	m_m_ClampingPositionButton = new wxButton( m_ClampingPosPanel, wxID_ANY, wxT("Got to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ClampingPositionSizer2->Add( m_m_ClampingPositionButton, 0, wxALL, 5 );
	
	
	m_ClampingPositionSizer1->Add( m_ClampingPositionSizer2, 1, wxEXPAND, 5 );
	
	
	m_ClampingPosPanel->SetSizer( m_ClampingPositionSizer1 );
	m_ClampingPosPanel->Layout();
	m_ClampingPositionSizer1->Fit( m_ClampingPosPanel );
	m_Experiments->AddPage( m_ClampingPosPanel, wxT("Clamping Position"), false );
	m_PreloadPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_PreloadSizer1;
	m_PreloadSizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_PreloadSizer1->AddGrowableCol( 0 );
	m_PreloadSizer1->SetFlexibleDirection( wxBOTH );
	m_PreloadSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_PreloadSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxString m_PreloadUnitRadioBoxChoices[] = { wxT("Stress"), wxT("Force") };
	int m_PreloadUnitRadioBoxNChoices = sizeof( m_PreloadUnitRadioBoxChoices ) / sizeof( wxString );
	m_PreloadUnitRadioBox = new wxRadioBox( m_PreloadPanel, wxID_ANY, wxT("Unit"), wxDefaultPosition, wxDefaultSize, m_PreloadUnitRadioBoxNChoices, m_PreloadUnitRadioBoxChoices, 2, wxRA_SPECIFY_COLS );
	m_PreloadUnitRadioBox->SetSelection( 0 );
	m_PreloadSizer1->Add( m_PreloadUnitRadioBox, 0, wxEXPAND, 5 );
	
	m_PreloadCalculateDiameterPanel = new wxPanel( m_PreloadPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxBoxSizer* m_PreloadSizer3;
	m_PreloadSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_PreloadCalculateDiameterCheckBox = new wxCheckBox( m_PreloadCalculateDiameterPanel, wxID_ANY, wxT("Calculate diameter"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSizer3->Add( m_PreloadCalculateDiameterCheckBox, 0, wxALL, 5 );
	
	wxBoxSizer* m_PreloadSizer4;
	m_PreloadSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PreloadYStaticText = new wxStaticText( m_PreloadCalculateDiameterPanel, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadYStaticText->Wrap( -1 );
	m_PreloadSizer4->Add( m_PreloadYStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadYSpinCtrl = new wxSpinCtrl( m_PreloadCalculateDiameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_PreloadSizer4->Add( m_PreloadYSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadXStaticText = new wxStaticText( m_PreloadCalculateDiameterPanel, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadXStaticText->Wrap( -1 );
	m_PreloadSizer4->Add( m_PreloadXStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_PreloadXRadioBoxChoices[] = { wxT("% of y"), wxT("define") };
	int m_PreloadXRadioBoxNChoices = sizeof( m_PreloadXRadioBoxChoices ) / sizeof( wxString );
	m_PreloadXRadioBox = new wxRadioBox( m_PreloadCalculateDiameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_PreloadXRadioBoxNChoices, m_PreloadXRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	m_PreloadXRadioBox->SetSelection( 0 );
	m_PreloadSizer4->Add( m_PreloadXRadioBox, 0, wxALL, 5 );
	
	m_PreloadXSpinCtrl = new wxSpinCtrl( m_PreloadCalculateDiameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_PreloadSizer4->Add( m_PreloadXSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_PreloadSizer3->Add( m_PreloadSizer4, 1, wxEXPAND, 5 );
	
	
	m_PreloadCalculateDiameterPanel->SetSizer( m_PreloadSizer3 );
	m_PreloadCalculateDiameterPanel->Layout();
	m_PreloadSizer3->Fit( m_PreloadCalculateDiameterPanel );
	m_PreloadSizer1->Add( m_PreloadCalculateDiameterPanel, 1, wxEXPAND, 0 );
	
	wxBoxSizer* m_PreloadSizer2;
	m_PreloadSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PreloadLimitStaticText = new wxStaticText( m_PreloadPanel, wxID_ANY, wxT("Force / Stress Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadLimitStaticText->Wrap( -1 );
	m_PreloadSizer2->Add( m_PreloadLimitStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadLimitSpinCtrl = new wxSpinCtrl( m_PreloadPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_PreloadSizer2->Add( m_PreloadLimitSpinCtrl, 0, wxALL, 5 );
	
	m_PreloadSpeedStaticText = new wxStaticText( m_PreloadPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSpeedStaticText->Wrap( -1 );
	m_PreloadSizer2->Add( m_PreloadSpeedStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadSpeedSpinCtrl = new wxSpinCtrl( m_PreloadPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_PreloadSizer2->Add( m_PreloadSpeedSpinCtrl, 0, wxALL, 5 );
	
	
	m_PreloadSizer1->Add( m_PreloadSizer2, 1, wxEXPAND, 5 );
	
	
	m_PreloadSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_PreloadSdbSizer = new wxStdDialogButtonSizer();
	m_PreloadSdbSizerOK = new wxButton( m_PreloadPanel, wxID_OK );
	m_PreloadSdbSizer->AddButton( m_PreloadSdbSizerOK );
	m_PreloadSdbSizerCancel = new wxButton( m_PreloadPanel, wxID_CANCEL );
	m_PreloadSdbSizer->AddButton( m_PreloadSdbSizerCancel );
	m_PreloadSdbSizer->Realize();
	
	m_PreloadSizer1->Add( m_PreloadSdbSizer, 1, wxEXPAND, 5 );
	
	
	m_PreloadPanel->SetSizer( m_PreloadSizer1 );
	m_PreloadPanel->Layout();
	m_PreloadSizer1->Fit( m_PreloadPanel );
	m_Experiments->AddPage( m_PreloadPanel, wxT("Preload"), false );
	m_ConditioningPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ConditioningSizer1;
	m_ConditioningSizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_ConditioningSizer1->AddGrowableCol( 0 );
	m_ConditioningSizer1->AddGrowableRow( 5 );
	m_ConditioningSizer1->SetFlexibleDirection( wxBOTH );
	m_ConditioningSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_ConditioningSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_ConditioningSizer21;
	m_ConditioningSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ConditioningCyclesStaticText = new wxStaticText( m_ConditioningPanel, wxID_ANY, wxT("N cycles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ConditioningCyclesStaticText->Wrap( -1 );
	m_ConditioningSizer21->Add( m_ConditioningCyclesStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ConditioningCyclesSpinCtrl = new wxSpinCtrl( m_ConditioningPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 5 );
	m_ConditioningSizer21->Add( m_ConditioningCyclesSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ConditioningSpeedStaticText = new wxStaticText( m_ConditioningPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ConditioningSpeedStaticText->Wrap( -1 );
	m_ConditioningSizer21->Add( m_ConditioningSpeedStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxBoxSizer* m_ConditioningSizer3;
	m_ConditioningSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_ConditioningSpeedPreloadSpinCtrl = new wxSpinCtrl( m_ConditioningPanel, wxID_ANY, wxT("% preload/sec"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 7 );
	m_ConditioningSizer3->Add( m_ConditioningSpeedPreloadSpinCtrl, 0, wxALL, 5 );
	
	m_ConditioningSpeedMmSpinCtrl = new wxSpinCtrl( m_ConditioningPanel, wxID_ANY, wxT("mm/sec"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_ConditioningSizer3->Add( m_ConditioningSpeedMmSpinCtrl, 0, wxALL, 5 );
	
	
	m_ConditioningSizer21->Add( m_ConditioningSizer3, 1, wxEXPAND, 5 );
	
	
	m_ConditioningSizer1->Add( m_ConditioningSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_ConditioningSizer22;
	m_ConditioningSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ConditioningStressForceLimitStaticText = new wxStaticText( m_ConditioningPanel, wxID_ANY, wxT("Stress / Force Limit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ConditioningStressForceLimitStaticText->Wrap( -1 );
	m_ConditioningSizer22->Add( m_ConditioningStressForceLimitStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ConditioningStressForceLimitSpinCtrl = new wxSpinCtrl( m_ConditioningPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_ConditioningSizer22->Add( m_ConditioningStressForceLimitSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_ConditioningLimitRadioBoxChoices[] = { wxT(" "), wxT(" ") };
	int m_ConditioningLimitRadioBoxNChoices = sizeof( m_ConditioningLimitRadioBoxChoices ) / sizeof( wxString );
	m_ConditioningLimitRadioBox = new wxRadioBox( m_ConditioningPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_ConditioningLimitRadioBoxNChoices, m_ConditioningLimitRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	m_ConditioningLimitRadioBox->SetSelection( 0 );
	m_ConditioningSizer22->Add( m_ConditioningLimitRadioBox, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ConditioningDistanceLimitStaticText = new wxStaticText( m_ConditioningPanel, wxID_ANY, wxT("Distance Limit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ConditioningDistanceLimitStaticText->Wrap( -1 );
	m_ConditioningSizer22->Add( m_ConditioningDistanceLimitStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ConditioningDistanceLimitSpinCtrl = new wxSpinCtrl( m_ConditioningPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_ConditioningSizer22->Add( m_ConditioningDistanceLimitSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_ConditioningDisctanceLimitRadioBoxChoices[] = { wxT("mm/sec"), wxT("%") };
	int m_ConditioningDisctanceLimitRadioBoxNChoices = sizeof( m_ConditioningDisctanceLimitRadioBoxChoices ) / sizeof( wxString );
	m_ConditioningDisctanceLimitRadioBox = new wxRadioBox( m_ConditioningPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_ConditioningDisctanceLimitRadioBoxNChoices, m_ConditioningDisctanceLimitRadioBoxChoices, 1, wxRA_SPECIFY_COLS );
	m_ConditioningDisctanceLimitRadioBox->SetSelection( 0 );
	m_ConditioningSizer22->Add( m_ConditioningDisctanceLimitRadioBox, 0, wxALL, 5 );
	
	
	m_ConditioningSizer1->Add( m_ConditioningSizer22, 1, wxEXPAND, 5 );
	
	
	m_ConditioningSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_ConditioningSdbSizer = new wxStdDialogButtonSizer();
	m_ConditioningSdbSizerOK = new wxButton( m_ConditioningPanel, wxID_OK );
	m_ConditioningSdbSizer->AddButton( m_ConditioningSdbSizerOK );
	m_ConditioningSdbSizerCancel = new wxButton( m_ConditioningPanel, wxID_CANCEL );
	m_ConditioningSdbSizer->AddButton( m_ConditioningSdbSizerCancel );
	m_ConditioningSdbSizer->Realize();
	
	m_ConditioningSizer1->Add( m_ConditioningSdbSizer, 1, wxEXPAND, 5 );
	
	
	m_ConditioningSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_ConditioningPanel->SetSizer( m_ConditioningSizer1 );
	m_ConditioningPanel->Layout();
	m_ConditioningSizer1->Fit( m_ConditioningPanel );
	m_Experiments->AddPage( m_ConditioningPanel, wxT("Conditioning"), true );
	m_Ramp2FailurePanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_R2FSizer1;
	m_R2FSizer1 = new wxFlexGridSizer( 6, 0, 0, 0 );
	m_R2FSizer1->AddGrowableCol( 0 );
	m_R2FSizer1->AddGrowableRow( 5 );
	m_R2FSizer1->SetFlexibleDirection( wxBOTH );
	m_R2FSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_R2FSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_R2FSizer21;
	m_R2FSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_R2FSpeedStaticText = new wxStaticText( m_Ramp2FailurePanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_R2FSpeedStaticText->Wrap( -1 );
	m_R2FSizer21->Add( m_R2FSpeedStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_R2FSpeedSpinCtrl = new wxSpinCtrl( m_Ramp2FailurePanel, wxID_ANY, wxT("%preload/s"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 6 );
	m_R2FSizer21->Add( m_R2FSpeedSpinCtrl, 0, wxALL, 5 );
	
	m_R2FDropBStopStaticText = new wxStaticText( m_Ramp2FailurePanel, wxID_ANY, wxT("Drop before Stop:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_R2FDropBStopStaticText->Wrap( -1 );
	m_R2FSizer21->Add( m_R2FDropBStopStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_R2FDropBStopSpinCtrl = new wxSpinCtrl( m_Ramp2FailurePanel, wxID_ANY, wxT("%max. force"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_R2FSizer21->Add( m_R2FDropBStopSpinCtrl, 0, wxALL, 5 );
	
	
	m_R2FSizer1->Add( m_R2FSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_R2FSizer22;
	m_R2FSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_R2FAfterFailureStaticText = new wxStaticText( m_Ramp2FailurePanel, wxID_ANY, wxT("After Failure:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_R2FAfterFailureStaticText->Wrap( -1 );
	m_R2FSizer22->Add( m_R2FAfterFailureStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_R2FAfterFailureRadioBoxChoices[] = { wxT("Preload pos."), wxT("Stop"), wxT("Go to") };
	int m_R2FAfterFailureRadioBoxNChoices = sizeof( m_R2FAfterFailureRadioBoxChoices ) / sizeof( wxString );
	m_R2FAfterFailureRadioBox = new wxRadioBox( m_Ramp2FailurePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_R2FAfterFailureRadioBoxNChoices, m_R2FAfterFailureRadioBoxChoices, 1, wxRA_SPECIFY_COLS );
	m_R2FAfterFailureRadioBox->SetSelection( 1 );
	m_R2FSizer22->Add( m_R2FAfterFailureRadioBox, 0, wxALL, 5 );
	
	m_R2FGoToSpinCtrl = new wxSpinCtrl( m_Ramp2FailurePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_R2FSizer22->Add( m_R2FGoToSpinCtrl, 0, wxALIGN_BOTTOM|wxALL, 5 );
	
	wxString m_R2FGoToRadioBoxChoices[] = { wxT("mm"), wxT("% preload") };
	int m_R2FGoToRadioBoxNChoices = sizeof( m_R2FGoToRadioBoxChoices ) / sizeof( wxString );
	m_R2FGoToRadioBox = new wxRadioBox( m_Ramp2FailurePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_R2FGoToRadioBoxNChoices, m_R2FGoToRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	m_R2FGoToRadioBox->SetSelection( 0 );
	m_R2FSizer22->Add( m_R2FGoToRadioBox, 0, wxALIGN_BOTTOM, 5 );
	
	
	m_R2FSizer1->Add( m_R2FSizer22, 1, wxALL|wxEXPAND, 5 );
	
	
	m_R2FSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_R2FSdbSizer = new wxStdDialogButtonSizer();
	m_R2FSdbSizerOK = new wxButton( m_Ramp2FailurePanel, wxID_OK );
	m_R2FSdbSizer->AddButton( m_R2FSdbSizerOK );
	m_R2FSdbSizerCancel = new wxButton( m_Ramp2FailurePanel, wxID_CANCEL );
	m_R2FSdbSizer->AddButton( m_R2FSdbSizerCancel );
	m_R2FSdbSizer->Realize();
	
	m_R2FSizer1->Add( m_R2FSdbSizer, 1, wxEXPAND, 5 );
	
	
	m_R2FSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_Ramp2FailurePanel->SetSizer( m_R2FSizer1 );
	m_Ramp2FailurePanel->Layout();
	m_R2FSizer1->Fit( m_Ramp2FailurePanel );
	m_Experiments->AddPage( m_Ramp2FailurePanel, wxT("E:Ramp to Failure"), false );
	m_RelaxationPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_RelaxationSizer1;
	m_RelaxationSizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_RelaxationSizer1->AddGrowableCol( 0 );
	m_RelaxationSizer1->AddGrowableRow( 5 );
	m_RelaxationSizer1->SetFlexibleDirection( wxBOTH );
	m_RelaxationSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_RelaxationSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_RelaxationSizer21;
	m_RelaxationSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_RelaxationRampStaticText = new wxStaticText( m_RelaxationPanel, wxID_ANY, wxT("Ramp:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RelaxationRampStaticText->Wrap( -1 );
	m_RelaxationSizer21->Add( m_RelaxationRampStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_RelaxationRampSpinCtrl = new wxSpinCtrl( m_RelaxationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_RelaxationSizer21->Add( m_RelaxationRampSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_RelaxationRampRadioBoxChoices[] = { wxT("distance"), wxT("expension") };
	int m_RelaxationRampRadioBoxNChoices = sizeof( m_RelaxationRampRadioBoxChoices ) / sizeof( wxString );
	m_RelaxationRampRadioBox = new wxRadioBox( m_RelaxationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_RelaxationRampRadioBoxNChoices, m_RelaxationRampRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	m_RelaxationRampRadioBox->SetSelection( 0 );
	m_RelaxationSizer21->Add( m_RelaxationRampRadioBox, 0, wxBOTTOM, 5 );
	
	
	m_RelaxationSizer1->Add( m_RelaxationSizer21, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_RelaxationSizer22;
	m_RelaxationSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_RelaxationPauseStaticText = new wxStaticText( m_RelaxationPanel, wxID_ANY, wxT("Pause:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RelaxationPauseStaticText->Wrap( -1 );
	m_RelaxationSizer22->Add( m_RelaxationPauseStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_RelaxationPauseSpinCtrl = new wxSpinCtrl( m_RelaxationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_RelaxationSizer22->Add( m_RelaxationPauseSpinCtrl, 0, wxALL, 5 );
	
	m_RelaxationStepsStaticText = new wxStaticText( m_RelaxationPanel, wxID_ANY, wxT("N steps:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RelaxationStepsStaticText->Wrap( -1 );
	m_RelaxationSizer22->Add( m_RelaxationStepsStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_RelaxationStepsSpinCtrl = new wxSpinCtrl( m_RelaxationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_RelaxationSizer22->Add( m_RelaxationStepsSpinCtrl, 0, wxALL, 5 );
	
	
	m_RelaxationSizer1->Add( m_RelaxationSizer22, 1, wxEXPAND, 5 );
	
	
	m_RelaxationSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_RelaxationSdbSizer = new wxStdDialogButtonSizer();
	m_RelaxationSdbSizerOK = new wxButton( m_RelaxationPanel, wxID_OK );
	m_RelaxationSdbSizer->AddButton( m_RelaxationSdbSizerOK );
	m_RelaxationSdbSizerCancel = new wxButton( m_RelaxationPanel, wxID_CANCEL );
	m_RelaxationSdbSizer->AddButton( m_RelaxationSdbSizerCancel );
	m_RelaxationSdbSizer->Realize();
	
	m_RelaxationSizer1->Add( m_RelaxationSdbSizer, 1, wxEXPAND, 5 );
	
	
	m_RelaxationSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_RelaxationPanel->SetSizer( m_RelaxationSizer1 );
	m_RelaxationPanel->Layout();
	m_RelaxationSizer1->Fit( m_RelaxationPanel );
	m_Experiments->AddPage( m_RelaxationPanel, wxT("E:Relaxation"), false );
	m_CreepPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_CreepSizer1;
	m_CreepSizer1 = new wxFlexGridSizer( 6, 0, 0, 0 );
	m_CreepSizer1->AddGrowableCol( 0 );
	m_CreepSizer1->AddGrowableRow( 5 );
	m_CreepSizer1->SetFlexibleDirection( wxBOTH );
	m_CreepSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_CreepSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_CreepSizer21;
	m_CreepSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_CreepSpeedStaticText = new wxStaticText( m_CreepPanel, wxID_ANY, wxT("Speed for Ramp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CreepSpeedStaticText->Wrap( -1 );
	m_CreepSizer21->Add( m_CreepSpeedStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_CreepSpeedSpinCtrl = new wxSpinCtrl( m_CreepPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_CreepSizer21->Add( m_CreepSpeedSpinCtrl, 0, wxALL, 5 );
	
	m_CreepHoldForceStressStaticText = new wxStaticText( m_CreepPanel, wxID_ANY, wxT("Hold Force/Stress"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CreepHoldForceStressStaticText->Wrap( -1 );
	m_CreepSizer21->Add( m_CreepHoldForceStressStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_CreepHoldForceStressSpinCtrl = new wxSpinCtrl( m_CreepPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_CreepSizer21->Add( m_CreepHoldForceStressSpinCtrl, 0, wxALL, 5 );
	
	
	m_CreepSizer1->Add( m_CreepSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_CreepSizer22;
	m_CreepSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_CreepHoldTimeStaticText = new wxStaticText( m_CreepPanel, wxID_ANY, wxT("Hold Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_CreepHoldTimeStaticText->Wrap( -1 );
	m_CreepSizer22->Add( m_CreepHoldTimeStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_CreepHoldTimeSpinCtrl = new wxSpinCtrl( m_CreepPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_CreepSizer22->Add( m_CreepHoldTimeSpinCtrl, 0, wxALL, 5 );
	
	
	m_CreepSizer1->Add( m_CreepSizer22, 1, wxEXPAND, 5 );
	
	
	m_CreepSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_CreepSdbSizer = new wxStdDialogButtonSizer();
	m_CreepSdbSizerOK = new wxButton( m_CreepPanel, wxID_OK );
	m_CreepSdbSizer->AddButton( m_CreepSdbSizerOK );
	m_CreepSdbSizerCancel = new wxButton( m_CreepPanel, wxID_CANCEL );
	m_CreepSdbSizer->AddButton( m_CreepSdbSizerCancel );
	m_CreepSdbSizer->Realize();
	
	m_CreepSizer1->Add( m_CreepSdbSizer, 1, wxEXPAND, 5 );
	
	
	m_CreepSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_CreepPanel->SetSizer( m_CreepSizer1 );
	m_CreepPanel->Layout();
	m_CreepSizer1->Fit( m_CreepPanel );
	m_Experiments->AddPage( m_CreepPanel, wxT("E:Creep"), false );
	m_FatiguePanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_FatigueSizer1;
	m_FatigueSizer1 = new wxFlexGridSizer( 8, 1, 0, 0 );
	m_FatigueSizer1->AddGrowableCol( 0 );
	m_FatigueSizer1->AddGrowableRow( 7 );
	m_FatigueSizer1->SetFlexibleDirection( wxBOTH );
	m_FatigueSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_FatigueSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_FatigueSizer21;
	m_FatigueSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_FatigueCyclesStaticText = new wxStaticText( m_FatiguePanel, wxID_ANY, wxT("N of cycles"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FatigueCyclesStaticText->Wrap( -1 );
	m_FatigueSizer21->Add( m_FatigueCyclesStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_FatigueCyclesSpinCtrl = new wxSpinCtrl( m_FatiguePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_FatigueSizer21->Add( m_FatigueCyclesSpinCtrl, 0, wxALL, 5 );
	
	m_FatigueTotalTimeStaticText = new wxStaticText( m_FatiguePanel, wxID_ANY, wxT("Total time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FatigueTotalTimeStaticText->Wrap( -1 );
	m_FatigueSizer21->Add( m_FatigueTotalTimeStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_FatigueTotalTimeSpinCtrl = new wxSpinCtrl( m_FatiguePanel, wxID_ANY, wxT("min."), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_FatigueSizer21->Add( m_FatigueTotalTimeSpinCtrl, 0, wxALL, 5 );
	
	
	m_FatigueSizer1->Add( m_FatigueSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_FatigueSizer22;
	m_FatigueSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_FatigueAmplitudeTimeStaticText = new wxStaticText( m_FatiguePanel, wxID_ANY, wxT("Amplitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FatigueAmplitudeTimeStaticText->Wrap( -1 );
	m_FatigueSizer22->Add( m_FatigueAmplitudeTimeStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_FatigueAmplitudeSpinCtrl = new wxSpinCtrl( m_FatiguePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_FatigueSizer22->Add( m_FatigueAmplitudeSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_AmplitudeRadioBoxChoices[] = { wxT("distance"), wxT("%preload") };
	int m_AmplitudeRadioBoxNChoices = sizeof( m_AmplitudeRadioBoxChoices ) / sizeof( wxString );
	m_AmplitudeRadioBox = new wxRadioBox( m_FatiguePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_AmplitudeRadioBoxNChoices, m_AmplitudeRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	m_AmplitudeRadioBox->SetSelection( 0 );
	m_FatigueSizer22->Add( m_AmplitudeRadioBox, 0, wxEXPAND, 5 );
	
	
	m_FatigueSizer1->Add( m_FatigueSizer22, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_FatigueSizer23;
	m_FatigueSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_RestTimeStaticText = new wxStaticText( m_FatiguePanel, wxID_ANY, wxT("Rest time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_RestTimeStaticText->Wrap( -1 );
	m_FatigueSizer23->Add( m_RestTimeStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_RestTimeSpinCtrl = new wxSpinCtrl( m_FatiguePanel, wxID_ANY, wxT("sec."), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_FatigueSizer23->Add( m_RestTimeSpinCtrl, 0, wxALL, 5 );
	
	m_FrequencyStaticText = new wxStaticText( m_FatiguePanel, wxID_ANY, wxT("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FrequencyStaticText->Wrap( -1 );
	m_FatigueSizer23->Add( m_FrequencyStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_spinCtrl24 = new wxSpinCtrl( m_FatiguePanel, wxID_ANY, wxT("cycles/sec."), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_FatigueSizer23->Add( m_spinCtrl24, 0, wxALL, 5 );
	
	
	m_FatigueSizer1->Add( m_FatigueSizer23, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_FatigueSizer24;
	m_FatigueSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBox2 = new wxCheckBox( m_FatiguePanel, wxID_ANY, wxT("Loop + nr."), wxDefaultPosition, wxDefaultSize, 0 );
	m_FatigueSizer24->Add( m_checkBox2, 0, wxALL, 5 );
	
	
	m_FatigueSizer1->Add( m_FatigueSizer24, 1, wxEXPAND, 5 );
	
	
	m_FatiguePanel->SetSizer( m_FatigueSizer1 );
	m_FatiguePanel->Layout();
	m_FatigueSizer1->Fit( m_FatiguePanel );
	m_Experiments->AddPage( m_FatiguePanel, wxT("E:Fatigue Testing"), false );
	
	m_TopSizer2->Add( m_Experiments, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_StretcherDistanceSizer;
	m_StretcherDistanceSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_DecreaseDistanceButton = new wxButton( this, wxID_ANY, wxT("--> <--"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StretcherDistanceSizer->Add( m_DecreaseDistanceButton, 1, wxALIGN_BOTTOM|wxALL, 5 );
	
	m_IncreaseDistanceButton = new wxButton( this, wxID_ANY, wxT("<-- -->"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StretcherDistanceSizer->Add( m_IncreaseDistanceButton, 1, wxALIGN_BOTTOM|wxALL, 5 );
	
	m_StopButton = new wxButton( this, wxID_ANY, wxT("STOP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StretcherDistanceSizer->Add( m_StopButton, 1, wxALIGN_BOTTOM|wxALL, 5 );
	
	m_ForceStaticText = new wxStaticText( this, wxID_ANY, wxT("Force"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_ForceStaticText->Wrap( -1 );
	m_StretcherDistanceSizer->Add( m_ForceStaticText, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_TopSizer2->Add( m_StretcherDistanceSizer, 1, wxEXPAND, 5 );
	
	
	bTopSizer->Add( m_TopSizer2, 1, wxEXPAND, 5 );
	
	m_Protocols = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ProtocolsSizer1;
	m_ProtocolsSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
	m_ProtocolsSizer1->AddGrowableCol( 0 );
	m_ProtocolsSizer1->AddGrowableRow( 0 );
	m_ProtocolsSizer1->SetFlexibleDirection( wxBOTH );
	m_ProtocolsSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ProtocolsListBox = new wxListBox( m_Protocols, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_ProtocolsSizer1->Add( m_ProtocolsListBox, 0, wxALL|wxEXPAND, 5 );
	
	m_ProtocolsNavigationPanel = new wxPanel( m_Protocols, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ProtocolsSizer2;
	m_ProtocolsSizer2 = new wxFlexGridSizer( 5, 2, 0, 0 );
	m_ProtocolsSizer2->AddGrowableCol( 0 );
	m_ProtocolsSizer2->AddGrowableCol( 1 );
	m_ProtocolsSizer2->AddGrowableRow( 0 );
	m_ProtocolsSizer2->AddGrowableRow( 1 );
	m_ProtocolsSizer2->AddGrowableRow( 2 );
	m_ProtocolsSizer2->AddGrowableRow( 3 );
	m_ProtocolsSizer2->AddGrowableRow( 4 );
	m_ProtocolsSizer2->SetFlexibleDirection( wxBOTH );
	m_ProtocolsSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ProtocolsExperiment1Button = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsExperiment1Button, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText32 = new wxStaticText( m_ProtocolsNavigationPanel, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	m_ProtocolsSizer2->Add( m_staticText32, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsExperiment2Button = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsExperiment2Button, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsUpButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("↑"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsUpButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsExperiment3Button = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsExperiment3Button, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsXButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsXButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsExperiment4Button = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsExperiment4Button, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsDownButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("↓"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsDownButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsExperiment5Button = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsExperiment5Button, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_checkBox3 = new wxCheckBox( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Loop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_checkBox3, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_ProtocolsNavigationPanel->SetSizer( m_ProtocolsSizer2 );
	m_ProtocolsNavigationPanel->Layout();
	m_ProtocolsSizer2->Fit( m_ProtocolsNavigationPanel );
	m_ProtocolsSizer1->Add( m_ProtocolsNavigationPanel, 1, wxALL|wxEXPAND, 5 );
	
	
	m_Protocols->SetSizer( m_ProtocolsSizer1 );
	m_Protocols->Layout();
	m_ProtocolsSizer1->Fit( m_Protocols );
	bTopSizer->Add( m_Protocols, 1, wxEXPAND | wxALL, 5 );
	
	
	bMainSizer->Add( bTopSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bBottomSizer;
	bBottomSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_Results = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_Results->SetScrollRate( 5, 5 );
	wxBoxSizer* m_ResultSizer1;
	m_ResultSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_ResultSizer21;
	m_ResultSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ResultListCtrl = new wxListCtrl( m_Results, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	m_ResultSizer21->Add( m_ResultListCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	m_ResultSizer1->Add( m_ResultSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_ResultSizer22;
	m_ResultSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ResultSaveButton = new wxButton( m_Results, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ResultSizer22->Add( m_ResultSaveButton, 0, wxALL, 5 );
	
	
	m_ResultSizer1->Add( m_ResultSizer22, 0, wxBOTTOM, 5 );
	
	
	m_Results->SetSizer( m_ResultSizer1 );
	m_Results->Layout();
	m_ResultSizer1->Fit( m_Results );
	bBottomSizer->Add( m_Results, 1, wxEXPAND | wxALL, 5 );
	
	m_Graph = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_GraphSizer;
	m_GraphSizer = new wxBoxSizer( wxVERTICAL );
	
	
	m_Graph->SetSizer( m_GraphSizer );
	m_Graph->Layout();
	m_GraphSizer->Fit( m_Graph );
	bBottomSizer->Add( m_Graph, 1, wxEXPAND | wxALL, 5 );
	
	
	bMainSizer->Add( bBottomSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	m_MenuBar = new wxMenuBar( 0 );
	m_SettingsMenu = new wxMenu();
	m_SamplingFrequencyMenu = new wxMenu();
	m_SettingsMenu->Append( -1, wxT("MyMenu"), m_SamplingFrequencyMenu );
	
	m_PortsMenu = new wxMenu();
	m_SettingsMenu->Append( -1, wxT("MyMenu"), m_PortsMenu );
	
	m_FileOutputMenu = new wxMenu();
	m_SettingsMenu->Append( -1, wxT("MyMenu"), m_FileOutputMenu );
	
	m_MenuBar->Append( m_SettingsMenu, wxT("Settings") ); 
	
	this->SetMenuBar( m_MenuBar );
	
	
	this->Centre( wxBOTH );
}

MyFrame_Base::~MyFrame_Base()
{
}

MySamplingFrequency_Base::MySamplingFrequency_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_SamplingFrequencySizer1;
	m_SamplingFrequencySizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_SamplingFrequencySizer1->AddGrowableCol( 0 );
	m_SamplingFrequencySizer1->AddGrowableRow( 5 );
	m_SamplingFrequencySizer1->SetFlexibleDirection( wxBOTH );
	m_SamplingFrequencySizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_SamplingFrequencySizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_SamplingFrequencySizer21;
	m_SamplingFrequencySizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_SamplingFrequencyDistanceStaticText = new wxStaticText( this, wxID_ANY, wxT("Measuring Frequency for Distance:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SamplingFrequencyDistanceStaticText->Wrap( -1 );
	m_SamplingFrequencySizer21->Add( m_SamplingFrequencyDistanceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SamplingFrequencyDistanceSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_SamplingFrequencySizer21->Add( m_SamplingFrequencyDistanceSpinCtrl, 0, wxALL, 5 );
	
	
	m_SamplingFrequencySizer1->Add( m_SamplingFrequencySizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_SamplingFrequencySizer22;
	m_SamplingFrequencySizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_SamplingFrequencyForceStaticText = new wxStaticText( this, wxID_ANY, wxT("Measuring Frequency for Force:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SamplingFrequencyForceStaticText->Wrap( -1 );
	m_SamplingFrequencySizer22->Add( m_SamplingFrequencyForceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SamplingFrequencyForceSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_SamplingFrequencySizer22->Add( m_SamplingFrequencyForceSpinCtrl, 0, wxALL, 5 );
	
	
	m_SamplingFrequencySizer1->Add( m_SamplingFrequencySizer22, 1, wxEXPAND, 5 );
	
	
	m_SamplingFrequencySizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_SamplingFrequencySdbSizer = new wxStdDialogButtonSizer();
	m_SamplingFrequencySdbSizerOK = new wxButton( this, wxID_OK );
	m_SamplingFrequencySdbSizer->AddButton( m_SamplingFrequencySdbSizerOK );
	m_SamplingFrequencySdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_SamplingFrequencySdbSizer->AddButton( m_SamplingFrequencySdbSizerCancel );
	m_SamplingFrequencySdbSizer->Realize();
	
	m_SamplingFrequencySizer1->Add( m_SamplingFrequencySdbSizer, 1, wxEXPAND, 5 );
	
	
	m_SamplingFrequencySizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_SamplingFrequencySizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MySamplingFrequency_Base::~MySamplingFrequency_Base()
{
}

MyPorts_Base::MyPorts_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_PortsSizer1;
	m_PortsSizer1 = new wxFlexGridSizer( 7, 0, 0, 0 );
	m_PortsSizer1->AddGrowableCol( 0 );
	m_PortsSizer1->AddGrowableRow( 5 );
	m_PortsSizer1->SetFlexibleDirection( wxBOTH );
	m_PortsSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_PortsSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_PortsSizer21;
	m_PortsSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PortsMotor1StaticText = new wxStaticText( this, wxID_ANY, wxT("Motor 1:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsMotor1StaticText->Wrap( -1 );
	m_PortsSizer21->Add( m_PortsMotor1StaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxArrayString m_PortsMotor1ChoiceChoices;
	m_PortsMotor1Choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsMotor1ChoiceChoices, 0 );
	m_PortsMotor1Choice->SetSelection( 0 );
	m_PortsSizer21->Add( m_PortsMotor1Choice, 0, wxALL, 5 );
	
	
	m_PortsSizer1->Add( m_PortsSizer21, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_PortsSizer22;
	m_PortsSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PortsMotor2StaticText = new wxStaticText( this, wxID_ANY, wxT("Motor 2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsMotor2StaticText->Wrap( -1 );
	m_PortsSizer22->Add( m_PortsMotor2StaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxArrayString m_PortsMotor2ChoiceChoices;
	m_PortsMotor2Choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsMotor2ChoiceChoices, 0 );
	m_PortsMotor2Choice->SetSelection( 0 );
	m_PortsSizer22->Add( m_PortsMotor2Choice, 0, wxALL, 5 );
	
	
	m_PortsSizer1->Add( m_PortsSizer22, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_PortsSizer23;
	m_PortsSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PortsForceStaticText = new wxStaticText( this, wxID_ANY, wxT("Force Sensor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsForceStaticText->Wrap( -1 );
	m_PortsSizer23->Add( m_PortsForceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxArrayString m_PortsForceChoiceChoices;
	m_PortsForceChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsForceChoiceChoices, 0 );
	m_PortsForceChoice->SetSelection( 0 );
	m_PortsSizer23->Add( m_PortsForceChoice, 0, wxALL, 5 );
	
	
	m_PortsSizer1->Add( m_PortsSizer23, 1, wxEXPAND, 5 );
	
	
	m_PortsSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_PortsSdbSizer = new wxStdDialogButtonSizer();
	m_PortsSdbSizerOK = new wxButton( this, wxID_OK );
	m_PortsSdbSizer->AddButton( m_PortsSdbSizerOK );
	m_PortsSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_PortsSdbSizer->AddButton( m_PortsSdbSizerCancel );
	m_PortsSdbSizer->Realize();
	
	m_PortsSizer1->Add( m_PortsSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_PortsSizer1 );
	this->Layout();
	m_PortsSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

MyPorts_Base::~MyPorts_Base()
{
}

MyFileOutput_Vase::MyFileOutput_Vase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_FileOutputSizer1;
	m_FileOutputSizer1 = new wxFlexGridSizer( 5, 0, 0, 0 );
	m_FileOutputSizer1->AddGrowableCol( 0 );
	m_FileOutputSizer1->AddGrowableRow( 4 );
	m_FileOutputSizer1->SetFlexibleDirection( wxBOTH );
	m_FileOutputSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_FileOutputSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* m_FileOutputSizer2;
	m_FileOutputSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_FileOutputNameStaticText = new wxStaticText( this, wxID_ANY, wxT("Filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FileOutputNameStaticText->Wrap( -1 );
	m_FileOutputSizer2->Add( m_FileOutputNameStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_FileOutputNameFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_FileOutputSizer2->Add( m_FileOutputNameFilePicker, 0, wxALL, 5 );
	
	
	m_FileOutputSizer1->Add( m_FileOutputSizer2, 1, wxEXPAND, 5 );
	
	
	m_FileOutputSizer1->Add( 0, 5, 1, wxEXPAND, 5 );
	
	m_FileOutputSdbSizer = new wxStdDialogButtonSizer();
	m_FileOutputSdbSizerOK = new wxButton( this, wxID_OK );
	m_FileOutputSdbSizer->AddButton( m_FileOutputSdbSizerOK );
	m_FileOutputSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_FileOutputSdbSizer->AddButton( m_FileOutputSdbSizerCancel );
	m_FileOutputSdbSizer->Realize();
	
	m_FileOutputSizer1->Add( m_FileOutputSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_FileOutputSizer1 );
	this->Layout();
	m_FileOutputSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}

MyFileOutput_Vase::~MyFileOutput_Vase()
{
}
