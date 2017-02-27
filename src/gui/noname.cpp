///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 24 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

MyFrame_Base::MyFrame_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1000,500 ), wxDefaultSize );
	
	m_MenuBar = new wxMenuBar( 0 );
	m_SettingsMenu = new wxMenu();
	wxMenuItem* m_PortsMenuMenuItem;
	m_PortsMenuMenuItem = new wxMenuItem( m_SettingsMenu, wxANY_ID, wxString( wxT("Ports") ) , wxEmptyString, wxITEM_NORMAL );
	m_SettingsMenu->Append( m_PortsMenuMenuItem );
	
	wxMenuItem* m_FileOutputMenuItem;
	m_FileOutputMenuItem = new wxMenuItem( m_SettingsMenu, wxANY_ID, wxString( wxT("File Output") ) , wxEmptyString, wxITEM_NORMAL );
	m_SettingsMenu->Append( m_FileOutputMenuItem );
	
	m_MenuBar->Append( m_SettingsMenu, wxT("Settings") ); 
	
	m_AdvancedMenu = new wxMenu();
	wxMenuItem* m_StartUpDialogMenuItem;
	m_StartUpDialogMenuItem = new wxMenuItem( m_AdvancedMenu, wxID_ANY, wxString( wxT("Start up dialog") ) , wxEmptyString, wxITEM_NORMAL );
	m_AdvancedMenu->Append( m_StartUpDialogMenuItem );
	
	wxMenuItem* m_LoadStoredPositionsMenuItem;
	m_LoadStoredPositionsMenuItem = new wxMenuItem( m_AdvancedMenu, wxID_ANY, wxString( wxT("Load stored positions") ) , wxEmptyString, wxITEM_NORMAL );
	m_AdvancedMenu->Append( m_LoadStoredPositionsMenuItem );
	
	wxMenuItem* m_HomeStagesMenuItem;
	m_HomeStagesMenuItem = new wxMenuItem( m_AdvancedMenu, wxID_ANY, wxString( wxT("Home Stages") ) , wxEmptyString, wxITEM_NORMAL );
	m_AdvancedMenu->Append( m_HomeStagesMenuItem );
	
	m_MenuBar->Append( m_AdvancedMenu, wxT("Advanced") ); 
	
	this->SetMenuBar( m_MenuBar );
	
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
	wxFlexGridSizer* m_InitializeSizer1;
	m_InitializeSizer1 = new wxFlexGridSizer( 7, 1, 0, 0 );
	m_InitializeSizer1->AddGrowableCol( 0 );
	m_InitializeSizer1->AddGrowableRow( 6 );
	m_InitializeSizer1->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_InitializePanel21 = new wxPanel( m_InitializePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxFlexGridSizer* m_InitializeSizer21;
	m_InitializeSizer21 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_InitializeSizer21->AddGrowableCol( 0 );
	m_InitializeSizer21->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_InitializePresetStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Please load a preset if you want to load defined settings."), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializePresetStaticText->Wrap( -1 );
	m_InitializeSizer21->Add( m_InitializePresetStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializePresetFilePicker = new wxFilePickerCtrl( m_InitializePanel21, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_InitializeSizer21->Add( m_InitializePresetFilePicker, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* m_InitializeSizer31;
	m_InitializeSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_InitializeSizer31->AddGrowableCol( 0 );
	m_InitializeSizer31->AddGrowableCol( 1 );
	m_InitializeSizer31->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_InitializeApplyPresetButton = new wxButton( m_InitializePanel21, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeSizer31->Add( m_InitializeApplyPresetButton, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializeSavePresetButton = new wxButton( m_InitializePanel21, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeSizer31->Add( m_InitializeSavePresetButton, 0, wxALL|wxEXPAND, 5 );
	
	
	m_InitializeSizer21->Add( m_InitializeSizer31, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_InitializeSizer32;
	m_InitializeSizer32 = new wxFlexGridSizer( 1, 4, 0, 0 );
	m_InitializeSizer32->AddGrowableCol( 0 );
	m_InitializeSizer32->AddGrowableCol( 1 );
	m_InitializeSizer32->AddGrowableCol( 2 );
	m_InitializeSizer32->AddGrowableCol( 3 );
	m_InitializeSizer32->AddGrowableRow( 0 );
	m_InitializeSizer32->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer32->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_InitializeMaxPosStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Distance at max. positions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMaxPosStaticText->Wrap( -1 );
	m_InitializeSizer32->Add( m_InitializeMaxPosStaticText, 0, wxALL, 5 );
	
	m_InitializeMaxPosShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMaxPosShowStaticText->Wrap( -1 );
	m_InitializeSizer32->Add( m_InitializeMaxPosShowStaticText, 0, wxALL, 5 );
	
	m_InitializeMountingLengthStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Mounting length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMountingLengthStaticText->Wrap( -1 );
	m_InitializeSizer32->Add( m_InitializeMountingLengthStaticText, 0, wxALL, 5 );
	
	m_InitializeMountingLengthShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMountingLengthShowStaticText->Wrap( -1 );
	m_InitializeSizer32->Add( m_InitializeMountingLengthShowStaticText, 0, wxALL, 5 );
	
	
	m_InitializeSizer21->Add( m_InitializeSizer32, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_InitializeSizer331;
	m_InitializeSizer331 = new wxFlexGridSizer( 1, 8, 0, 0 );
	m_InitializeSizer331->AddGrowableCol( 0 );
	m_InitializeSizer331->AddGrowableCol( 1 );
	m_InitializeSizer331->AddGrowableCol( 2 );
	m_InitializeSizer331->AddGrowableCol( 3 );
	m_InitializeSizer331->AddGrowableCol( 4 );
	m_InitializeSizer331->AddGrowableCol( 5 );
	m_InitializeSizer331->AddGrowableCol( 6 );
	m_InitializeSizer331->AddGrowableCol( 7 );
	m_InitializeSizer331->AddGrowableRow( 0 );
	m_InitializeSizer331->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer331->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_InitializeMinDistanceStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Min. distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMinDistanceStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMinDistanceStaticText, 0, wxALL, 5 );
	
	m_InitializeMinDistanceShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxSUNKEN_BORDER );
	m_InitializeMinDistanceShowStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMinDistanceShowStaticText, 0, wxALIGN_LEFT|wxALL, 5 );
	
	m_InitializeMaxDistanceStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Max. distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMaxDistanceStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMaxDistanceStaticText, 0, wxALL, 5 );
	
	m_InitializeMaxDistanceShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_InitializeMaxDistanceShowStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMaxDistanceShowStaticText, 0, wxALIGN_LEFT|wxALL, 5 );
	
	m_InitializeMinForceStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Min. force [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMinForceStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMinForceStaticText, 0, wxALL, 5 );
	
	m_InitializeMinForceShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_InitializeMinForceShowStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMinForceShowStaticText, 0, wxALIGN_LEFT|wxALL, 5 );
	
	m_InitializeMaxForceStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Max. force [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeMaxForceStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMaxForceStaticText, 0, wxALL, 5 );
	
	m_InitializeMaxForceShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_InitializeMaxForceShowStaticText->Wrap( -1 );
	m_InitializeSizer331->Add( m_InitializeMaxForceShowStaticText, 0, wxALIGN_LEFT|wxALL, 5 );
	
	
	m_InitializeSizer21->Add( m_InitializeSizer331, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_InitializeSizer34;
	m_InitializeSizer34 = new wxFlexGridSizer( 0, 4, 0, 0 );
	m_InitializeSizer34->AddGrowableCol( 0 );
	m_InitializeSizer34->AddGrowableCol( 1 );
	m_InitializeSizer34->AddGrowableCol( 2 );
	m_InitializeSizer34->AddGrowableCol( 3 );
	m_InitializeSizer34->SetFlexibleDirection( wxBOTH );
	m_InitializeSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_InitializeDistanceSensitivityStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Distance sensitivity [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeDistanceSensitivityStaticText->Wrap( -1 );
	m_InitializeSizer34->Add( m_InitializeDistanceSensitivityStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializeDistanceSensitivityShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeDistanceSensitivityShowStaticText->Wrap( -1 );
	m_InitializeSizer34->Add( m_InitializeDistanceSensitivityShowStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializeForceStressSensitivityStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxT("Force sensitivity [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeForceStressSensitivityStaticText->Wrap( -1 );
	m_InitializeSizer34->Add( m_InitializeForceStressSensitivityStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializeForceStressSensitivityShowStaticText = new wxStaticText( m_InitializePanel21, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeForceStressSensitivityShowStaticText->Wrap( -1 );
	m_InitializeSizer34->Add( m_InitializeForceStressSensitivityShowStaticText, 0, wxALL, 5 );
	
	
	m_InitializeSizer21->Add( m_InitializeSizer34, 1, wxEXPAND, 5 );
	
	
	m_InitializePanel21->SetSizer( m_InitializeSizer21 );
	m_InitializePanel21->Layout();
	m_InitializeSizer21->Fit( m_InitializePanel21 );
	m_InitializeSizer1->Add( m_InitializePanel21, 1, wxEXPAND | wxALL, 5 );
	
	wxString m_InitializeUnitRadioBoxChoices[] = { wxT("Stress"), wxT("Force") };
	int m_InitializeUnitRadioBoxNChoices = sizeof( m_InitializeUnitRadioBoxChoices ) / sizeof( wxString );
	m_InitializeUnitRadioBox = new wxRadioBox( m_InitializePanel, wxID_ANY, wxT("Unit"), wxDefaultPosition, wxDefaultSize, m_InitializeUnitRadioBoxNChoices, m_InitializeUnitRadioBoxChoices, 2, wxRA_SPECIFY_COLS );
	m_InitializeUnitRadioBox->SetSelection( 1 );
	m_InitializeSizer1->Add( m_InitializeUnitRadioBox, 0, wxALL|wxEXPAND, 5 );
	
	m_InitializeCalculateDiameterPanel = new wxPanel( m_InitializePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxBoxSizer* m_InitializeSizer33;
	m_InitializeSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_InitializeCrossSectionStaticText = new wxStaticText( m_InitializeCalculateDiameterPanel, wxID_ANY, wxT("Cross section area [mm2]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_InitializeCrossSectionStaticText->Wrap( -1 );
	m_InitializeSizer33->Add( m_InitializeCrossSectionStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_InitializeCrossSectionSpinCtrl = new wxSpinCtrl( m_InitializeCalculateDiameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 10000, 0 );
	m_InitializeSizer33->Add( m_InitializeCrossSectionSpinCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_InitializeCalculateDiameterPanel->SetSizer( m_InitializeSizer33 );
	m_InitializeCalculateDiameterPanel->Layout();
	m_InitializeSizer33->Fit( m_InitializeCalculateDiameterPanel );
	m_InitializeSizer1->Add( m_InitializeCalculateDiameterPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	m_InitializePanel->SetSizer( m_InitializeSizer1 );
	m_InitializePanel->Layout();
	m_InitializeSizer1->Fit( m_InitializePanel );
	m_Experiments->AddPage( m_InitializePanel, wxT("Initialize"), true );
	m_LimitsPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_LimitsSizer1;
	m_LimitsSizer1 = new wxFlexGridSizer( 4, 1, 0, 0 );
	m_LimitsSizer1->AddGrowableCol( 0 );
	m_LimitsSizer1->AddGrowableRow( 3 );
	m_LimitsSizer1->SetFlexibleDirection( wxVERTICAL );
	m_LimitsSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LimitsLimitPanel1 = new wxPanel( m_LimitsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxFlexGridSizer* m_LimitsLimitSizer2;
	m_LimitsLimitSizer2 = new wxFlexGridSizer( 1, 2, 0, 0 );
	m_LimitsLimitSizer2->AddGrowableCol( 0 );
	m_LimitsLimitSizer2->SetFlexibleDirection( wxBOTH );
	m_LimitsLimitSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* m_LimitsLimitSizer3;
	m_LimitsLimitSizer3 = new wxFlexGridSizer( 6, 2, 0, 0 );
	m_LimitsLimitSizer3->SetFlexibleDirection( wxBOTH );
	m_LimitsLimitSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LimitsLimitStaticText60 = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxT("Limits:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitStaticText60->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitStaticText60, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_staticText61, 0, wxALL, 5 );
	
	m_LimitsLimitMinDistanceStaticText = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxT("Minimal distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitMinDistanceStaticText->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMinDistanceStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_LimitsLimitMinDistanceSpinCtrl = new wxSpinCtrl( m_LimitsLimitPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 153, 0 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMinDistanceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_LimitsLimitMaxDistanceStaticText = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxT("Maximal distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitMaxDistanceStaticText->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMaxDistanceStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_LimitsLimitMaxDistanceSpinCtrl = new wxSpinCtrl( m_LimitsLimitPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 153, 0 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMaxDistanceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_LimitsLimitMinForceStaticText = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxT("Minimal force [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitMinForceStaticText->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMinForceStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_LimitsLimitMinForceSpinCtrl = new wxSpinCtrl( m_LimitsLimitPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMinForceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_LimitsLimitMaxForceStaticText = new wxStaticText( m_LimitsLimitPanel1, wxID_ANY, wxT("Maximal force [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitMaxForceStaticText->Wrap( -1 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMaxForceStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_LimitsLimitMaxForceSpinCtrl = new wxSpinCtrl( m_LimitsLimitPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitMaxForceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_LimitsLimitSetButton = new wxButton( m_LimitsLimitPanel1, wxID_ANY, wxT("Set limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitSizer3->Add( m_LimitsLimitSetButton, 0, wxALL, 5 );
	
	
	m_LimitsLimitSizer2->Add( m_LimitsLimitSizer3, 1, wxEXPAND, 5 );
	
	m_LimitsLimitPanel2 = new wxPanel( m_LimitsLimitPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_LimitsLimitSizer4;
	m_LimitsLimitSizer4 = new wxFlexGridSizer( 4, 1, 0, 0 );
	m_LimitsLimitSizer4->AddGrowableRow( 0 );
	m_LimitsLimitSizer4->AddGrowableRow( 1 );
	m_LimitsLimitSizer4->AddGrowableRow( 2 );
	m_LimitsLimitSizer4->AddGrowableRow( 3 );
	m_LimitsLimitSizer4->SetFlexibleDirection( wxBOTH );
	m_LimitsLimitSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LimitsLimitSet1Button = new wxButton( m_LimitsLimitPanel2, wxID_ANY, wxT("Load \"Set 1\" limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitSizer4->Add( m_LimitsLimitSet1Button, 0, wxALL, 5 );
	
	m_LimitsLimitSet2Button = new wxButton( m_LimitsLimitPanel2, wxID_ANY, wxT("Load \"Set 2\" limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitSizer4->Add( m_LimitsLimitSet2Button, 0, wxALL, 5 );
	
	m_LimitsLimitSet3Button = new wxButton( m_LimitsLimitPanel2, wxID_ANY, wxT("Load \"Set 3\" limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitSizer4->Add( m_LimitsLimitSet3Button, 0, wxALL, 5 );
	
	m_LimitsLimitSet4Button = new wxButton( m_LimitsLimitPanel2, wxID_ANY, wxT("Load \"Set 4\" limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LimitsLimitSizer4->Add( m_LimitsLimitSet4Button, 0, wxALL, 5 );
	
	
	m_LimitsLimitPanel2->SetSizer( m_LimitsLimitSizer4 );
	m_LimitsLimitPanel2->Layout();
	m_LimitsLimitSizer4->Fit( m_LimitsLimitPanel2 );
	m_LimitsLimitSizer2->Add( m_LimitsLimitPanel2, 1, wxEXPAND | wxALL, 5 );
	
	
	m_LimitsLimitPanel1->SetSizer( m_LimitsLimitSizer2 );
	m_LimitsLimitPanel1->Layout();
	m_LimitsLimitSizer2->Fit( m_LimitsLimitPanel1 );
	m_LimitsSizer1->Add( m_LimitsLimitPanel1, 1, wxEXPAND | wxALL, 5 );
	
	
	m_LimitsSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_LimitsPanel->SetSizer( m_LimitsSizer1 );
	m_LimitsPanel->Layout();
	m_LimitsSizer1->Fit( m_LimitsPanel );
	m_Experiments->AddPage( m_LimitsPanel, wxT("Limits"), false );
	m_LengthsPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_LengthSizer1;
	m_LengthSizer1 = new wxFlexGridSizer( 3, 1, 0, 0 );
	m_LengthSizer1->SetFlexibleDirection( wxBOTH );
	m_LengthSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsPanel21 = new wxPanel( m_LengthsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxFlexGridSizer* m_LengthsSizer22;
	m_LengthsSizer22 = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_LengthsSizer22->AddGrowableCol( 0 );
	m_LengthsSizer22->SetFlexibleDirection( wxBOTH );
	m_LengthsSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsMoveStaticText = new wxStaticText( m_LengthsPanel21, wxID_ANY, wxT("Move the stages to the mounting length manually and/or use the distance input and \"Go to\" button,\nmount the clamps and push the button \"Set mounting lenght\"."), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsMoveStaticText->Wrap( -1 );
	m_LengthsSizer22->Add( m_LengthsMoveStaticText, 0, wxALL, 5 );
	
	wxBoxSizer* m_LengthsSizer31;
	m_LengthsSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_LengthsGoToStaticText = new wxStaticText( m_LengthsPanel21, wxID_ANY, wxT("Distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsGoToStaticText->Wrap( -1 );
	m_LengthsSizer31->Add( m_LengthsGoToStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_LengthsGoToSpinCtrl = new wxSpinCtrl( m_LengthsPanel21, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0, 0, 200, 150 );
	m_LengthsSizer31->Add( m_LengthsGoToSpinCtrl, 0, wxALL, 5 );
	
	m_LengthsGoToButton = new wxButton( m_LengthsPanel21, wxID_ANY, wxT("Go to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSizer31->Add( m_LengthsGoToButton, 0, wxALL, 5 );
	
	m_LengthsMountingLengthButton = new wxButton( m_LengthsPanel21, wxID_ANY, wxT("Set mounting length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSizer31->Add( m_LengthsMountingLengthButton, 0, wxALL, 5 );
	
	
	m_LengthsSizer22->Add( m_LengthsSizer31, 1, wxEXPAND, 5 );
	
	
	m_LengthsPanel21->SetSizer( m_LengthsSizer22 );
	m_LengthsPanel21->Layout();
	m_LengthsSizer22->Fit( m_LengthsPanel21 );
	m_LengthSizer1->Add( m_LengthsPanel21, 1, wxEXPAND | wxALL, 5 );
	
	m_LengthsPanel22 = new wxPanel( m_LengthsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_LengthsSizer23;
	m_LengthsSizer23 = new wxFlexGridSizer( 3, 1, 0, 0 );
	m_LengthsSizer23->AddGrowableCol( 0 );
	m_LengthsSizer23->AddGrowableRow( 0 );
	m_LengthsSizer23->AddGrowableRow( 1 );
	m_LengthsSizer23->AddGrowableRow( 2 );
	m_LengthsSizer23->SetFlexibleDirection( wxBOTH );
	m_LengthsSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsSensitivityStaticText = new wxStaticText( m_LengthsPanel22, wxID_ANY, wxT("Please select the sensitivity for the force/stress and the distance according to the velocity."), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSensitivityStaticText->Wrap( -1 );
	m_LengthsSizer23->Add( m_LengthsSensitivityStaticText, 0, wxALL, 5 );
	
	wxFlexGridSizer* m_LengthsSizer32;
	m_LengthsSizer32 = new wxFlexGridSizer( 0, 4, 0, 0 );
	m_LengthsSizer32->AddGrowableCol( 0 );
	m_LengthsSizer32->AddGrowableCol( 1 );
	m_LengthsSizer32->AddGrowableCol( 2 );
	m_LengthsSizer32->AddGrowableCol( 3 );
	m_LengthsSizer32->SetFlexibleDirection( wxBOTH );
	m_LengthsSizer32->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsForceStressSensitivityStaticText = new wxStaticText( m_LengthsPanel22, wxID_ANY, wxT("Force [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsForceStressSensitivityStaticText->Wrap( -1 );
	m_LengthsSizer32->Add( m_LengthsForceStressSensitivityStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_LengthsForceStressSensitivitySpinCtrl = new wxSpinCtrl( m_LengthsPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 10, 0 );
	m_LengthsSizer32->Add( m_LengthsForceStressSensitivitySpinCtrl, 0, wxEXPAND, 5 );
	
	m_LengthsDistanceSensitivityStaticText = new wxStaticText( m_LengthsPanel22, wxID_ANY, wxT("Distance [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsDistanceSensitivityStaticText->Wrap( -1 );
	m_LengthsSizer32->Add( m_LengthsDistanceSensitivityStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_LengthsDistanceSensitivitySpinCtrl = new wxSpinCtrl( m_LengthsPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 10, 0 );
	m_LengthsSizer32->Add( m_LengthsDistanceSensitivitySpinCtrl, 0, wxEXPAND, 5 );
	
	
	m_LengthsSizer23->Add( m_LengthsSizer32, 1, wxEXPAND, 5 );
	
	m_LengthsSensitivityButton = new wxButton( m_LengthsPanel22, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSizer23->Add( m_LengthsSensitivityButton, 0, wxALL|wxEXPAND, 5 );
	
	
	m_LengthsPanel22->SetSizer( m_LengthsSizer23 );
	m_LengthsPanel22->Layout();
	m_LengthsSizer23->Fit( m_LengthsPanel22 );
	m_LengthSizer1->Add( m_LengthsPanel22, 1, wxEXPAND | wxALL, 5 );
	
	m_LengthsPanel23 = new wxPanel( m_LengthsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_LengthsSizer24;
	m_LengthsSizer24 = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_LengthsSizer24->AddGrowableCol( 0 );
	m_LengthsSizer24->SetFlexibleDirection( wxBOTH );
	m_LengthsSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsZeroStaticText = new wxStaticText( m_LengthsPanel23, wxID_ANY, wxT("Set distance and/or stress/force to zero if you wish. Limits will be relative to this values."), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsZeroStaticText->Wrap( -1 );
	m_LengthsSizer24->Add( m_LengthsZeroStaticText, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* m_LengthsSizer33;
	m_LengthsSizer33 = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_LengthsSizer33->AddGrowableCol( 0 );
	m_LengthsSizer33->AddGrowableCol( 1 );
	m_LengthsSizer33->SetFlexibleDirection( wxBOTH );
	m_LengthsSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_LengthsSetDistanceZeroButton = new wxButton( m_LengthsPanel23, wxID_ANY, wxT("Zero distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSizer33->Add( m_LengthsSetDistanceZeroButton, 0, wxALL|wxEXPAND, 5 );
	
	m_LengthsSetForceZeroButton = new wxButton( m_LengthsPanel23, wxID_ANY, wxT("Zero force"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LengthsSizer33->Add( m_LengthsSetForceZeroButton, 0, wxALL|wxEXPAND, 5 );
	
	
	m_LengthsSizer24->Add( m_LengthsSizer33, 1, wxEXPAND, 5 );
	
	
	m_LengthsPanel23->SetSizer( m_LengthsSizer24 );
	m_LengthsPanel23->Layout();
	m_LengthsSizer24->Fit( m_LengthsPanel23 );
	m_LengthSizer1->Add( m_LengthsPanel23, 1, wxEXPAND | wxALL, 5 );
	
	
	m_LengthsPanel->SetSizer( m_LengthSizer1 );
	m_LengthsPanel->Layout();
	m_LengthSizer1->Fit( m_LengthsPanel );
	m_Experiments->AddPage( m_LengthsPanel, wxT("Lengths"), false );
	m_PreloadPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_PreloadSizer;
	m_PreloadSizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	m_PreloadSizer->AddGrowableCol( 0 );
	m_PreloadSizer->SetFlexibleDirection( wxBOTH );
	m_PreloadSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_PreloadCalculateDiameterPanel = new wxPanel( m_PreloadPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxBoxSizer* m_PreloadSizer2;
	m_PreloadSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_PreloadLimitStaticText = new wxStaticText( m_PreloadCalculateDiameterPanel, wxID_ANY, wxT("Force Limit [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadLimitStaticText->Wrap( -1 );
	m_PreloadSizer2->Add( m_PreloadLimitStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadLimitSpinCtrl = new wxSpinCtrl( m_PreloadCalculateDiameterPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_PreloadSizer2->Add( m_PreloadLimitSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_PreloadSpeedStaticText = new wxStaticText( m_PreloadCalculateDiameterPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSpeedStaticText->Wrap( -1 );
	m_PreloadSizer2->Add( m_PreloadSpeedStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_PreloadSpeedPanel = new wxPanel( m_PreloadCalculateDiameterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_PreloadSpeedPreloadSpinCtrl = new wxSpinCtrl( m_PreloadSpeedPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	fgSizer25->Add( m_PreloadSpeedPreloadSpinCtrl, 0, wxALL, 5 );
	
	m_PreloadSpeedPreloadStaticText = new wxStaticText( m_PreloadSpeedPanel, wxID_ANY, wxT("% mounting length/sec."), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSpeedPreloadStaticText->Wrap( -1 );
	fgSizer25->Add( m_PreloadSpeedPreloadStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_PreloadSpeedMmSpinCtrl = new wxSpinCtrl( m_PreloadSpeedPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 53, 0 );
	fgSizer25->Add( m_PreloadSpeedMmSpinCtrl, 0, wxALL, 5 );
	
	m_PreloadSpeedMmStaticText = new wxStaticText( m_PreloadSpeedPanel, wxID_ANY, wxT("mm/sec."), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSpeedMmStaticText->Wrap( -1 );
	fgSizer25->Add( m_PreloadSpeedMmStaticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	m_PreloadSpeedPanel->SetSizer( fgSizer25 );
	m_PreloadSpeedPanel->Layout();
	fgSizer25->Fit( m_PreloadSpeedPanel );
	m_PreloadSizer2->Add( m_PreloadSpeedPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	m_PreloadCalculateDiameterPanel->SetSizer( m_PreloadSizer2 );
	m_PreloadCalculateDiameterPanel->Layout();
	m_PreloadSizer2->Fit( m_PreloadCalculateDiameterPanel );
	m_PreloadSizer->Add( m_PreloadCalculateDiameterPanel, 1, wxEXPAND, 0 );
	
	wxFlexGridSizer* m_PreloadSdbSizer;
	m_PreloadSdbSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_PreloadSdbSizer->SetFlexibleDirection( wxBOTH );
	m_PreloadSdbSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_PreloadCancelButton = new wxButton( m_PreloadPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSdbSizer->Add( m_PreloadCancelButton, 0, wxALL, 5 );
	
	m_PreloadSendButton = new wxButton( m_PreloadPanel, wxID_ANY, wxT("Send to protocol"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PreloadSdbSizer->Add( m_PreloadSendButton, 0, wxALL, 5 );
	
	
	m_PreloadSizer->Add( m_PreloadSdbSizer, 1, wxALIGN_RIGHT, 5 );
	
	
	m_PreloadPanel->SetSizer( m_PreloadSizer );
	m_PreloadPanel->Layout();
	m_PreloadSizer->Fit( m_PreloadPanel );
	m_Experiments->AddPage( m_PreloadPanel, wxT("Preload"), false );
	m_OneStepPanel1 = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_OneStepSizer1;
	m_OneStepSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_OneStepSizer1->AddGrowableCol( 0 );
	m_OneStepSizer1->AddGrowableCol( 1 );
	m_OneStepSizer1->AddGrowableRow( 0 );
	m_OneStepSizer1->SetFlexibleDirection( wxBOTH );
	m_OneStepSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_OneStepPanel21 = new wxPanel( m_OneStepPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_OneStepSizer21;
	m_OneStepSizer21 = new wxFlexGridSizer( 3, 1, 0, 0 );
	m_OneStepSizer21->AddGrowableCol( 0 );
	m_OneStepSizer21->AddGrowableRow( 1 );
	m_OneStepSizer21->SetFlexibleDirection( wxBOTH );
	m_OneStepSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* m_OneStepTypeSizer;
	m_OneStepTypeSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_OneStepTypeSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OneStepStressForceRadioBtn = new wxRadioButton( m_OneStepPanel21, wxID_ANY, wxT("Stress/Force"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepTypeSizer->Add( m_OneStepStressForceRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_OneStepDistanceRadioBtn = new wxRadioButton( m_OneStepPanel21, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepTypeSizer->Add( m_OneStepDistanceRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_OneStepTypeSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_OneStepSizer21->Add( m_OneStepTypeSizer, 1, wxEXPAND, 5 );
	
	m_OneStepStressForcePanel = new wxPanel( m_OneStepPanel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_OneStepStressForceSizer;
	m_OneStepStressForceSizer = new wxFlexGridSizer( 6, 3, 0, 0 );
	m_OneStepStressForceSizer->AddGrowableRow( 5 );
	m_OneStepStressForceSizer->SetFlexibleDirection( wxBOTH );
	m_OneStepStressForceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_OneStepStressForceVelocityStaticText = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxT(" Velocity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceVelocityStaticText->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceVelocityStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepStressForceVelocitySpinCtrl = new wxSpinCtrl( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceVelocitySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	wxBoxSizer* m_OneStepStressForceVelocitySizer;
	m_OneStepStressForceVelocitySizer = new wxBoxSizer( wxVERTICAL );
	
	m_OneStepStressForceVelocityMmRadioBtn = new wxRadioButton( m_OneStepStressForcePanel, wxID_ANY, wxT("mm/s"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepStressForceVelocitySizer->Add( m_OneStepStressForceVelocityMmRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_OneStepStressForceVelocityPercentRadioBtn = new wxRadioButton( m_OneStepStressForcePanel, wxID_ANY, wxT("%L0/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceVelocitySizer->Add( m_OneStepStressForceVelocityPercentRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_OneStepStressForceSizer->Add( m_OneStepStressForceVelocitySizer, 1, wxALIGN_CENTER, 5 );
	
	m_OneStepStressForceDelayStaticText = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxT(" Delay [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceDelayStaticText->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceDelayStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepStressForceDelaySpinCtrl = new wxSpinCtrl( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceDelaySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText78 = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_staticText78, 0, wxALL, 5 );
	
	m_OneStepStressForceLimitStaticText = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxT(" Limit [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceLimitStaticText->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceLimitStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepStressForceLimitSpinCtrl = new wxSpinCtrl( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceLimitSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_OneStepStressForceHoldLimitCheckBox = new wxCheckBox( m_OneStepStressForcePanel, wxID_ANY, wxT("Hold"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceHoldLimitCheckBox, 0, wxALL, 5 );
	
	m_OneStepStressForceDwellStaticText = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxT(" Dwell [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceDwellStaticText->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceDwellStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepStressForceDwellSpinCtrl = new wxSpinCtrl( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForceDwellSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText82 = new wxStaticText( m_OneStepStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText82->Wrap( -1 );
	m_OneStepStressForceSizer->Add( m_staticText82, 0, wxALL, 5 );
	
	m_OneStepStressForcePhotoCheckBox = new wxCheckBox( m_OneStepStressForcePanel, wxID_ANY, wxT("Photo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepStressForceSizer->Add( m_OneStepStressForcePhotoCheckBox, 0, wxALIGN_CENTER, 5 );
	
	
	m_OneStepStressForceSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_OneStepStressForcePanel->SetSizer( m_OneStepStressForceSizer );
	m_OneStepStressForcePanel->Layout();
	m_OneStepStressForceSizer->Fit( m_OneStepStressForcePanel );
	m_OneStepSizer21->Add( m_OneStepStressForcePanel, 1, wxEXPAND | wxALL, 5 );
	
	m_OneStepDistancePanel = new wxPanel( m_OneStepPanel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	m_OneStepDistancePanel->Hide();
	
	wxFlexGridSizer* m_OneStepDistanceSizer;
	m_OneStepDistanceSizer = new wxFlexGridSizer( 6, 3, 0, 0 );
	m_OneStepDistanceSizer->AddGrowableRow( 0 );
	m_OneStepDistanceSizer->SetFlexibleDirection( wxBOTH );
	m_OneStepDistanceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_OneStepDistanceVelocityStaticText = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxT(" Velocity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceVelocityStaticText->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceVelocityStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepDistanceVelocitySpinCtrl = new wxSpinCtrl( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceVelocitySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	wxBoxSizer* m_OneStepDistanceVelocitySizer;
	m_OneStepDistanceVelocitySizer = new wxBoxSizer( wxVERTICAL );
	
	m_OneStepDistanceVelocityMmRadioBtn = new wxRadioButton( m_OneStepDistancePanel, wxID_ANY, wxT("mm/s"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepDistanceVelocitySizer->Add( m_OneStepDistanceVelocityMmRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_OneStepDistanceVelocityPercentRadioBtn = new wxRadioButton( m_OneStepDistancePanel, wxID_ANY, wxT("%L0/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceVelocitySizer->Add( m_OneStepDistanceVelocityPercentRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_OneStepDistanceSizer->Add( m_OneStepDistanceVelocitySizer, 1, wxALIGN_CENTER|wxEXPAND, 5 );
	
	m_OneStepDistanceDelayStaticText = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxT(" Delay [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceDelayStaticText->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceDelayStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepDistanceDelaySpinCtrl = new wxSpinCtrl( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceDelaySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText781 = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText781->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_staticText781, 0, wxALL, 5 );
	
	m_OneStepDistanceUpperLimitStaticText = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxT(" Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceUpperLimitStaticText->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceUpperLimitStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepDistanceLimitSpinCtrl = new wxSpinCtrl( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceLimitSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	wxBoxSizer* m_OneStepDistanceUpperLimitSizer;
	m_OneStepDistanceUpperLimitSizer = new wxBoxSizer( wxVERTICAL );
	
	m_OneStepDistanceLimitMmRelRadioBtn = new wxRadioButton( m_OneStepDistancePanel, wxID_ANY, wxT("mm rel."), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepDistanceUpperLimitSizer->Add( m_OneStepDistanceLimitMmRelRadioBtn, 0, wxALIGN_LEFT, 5 );
	
	m_OneStepDistanceLimitMmRadioBtn = new wxRadioButton( m_OneStepDistancePanel, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceUpperLimitSizer->Add( m_OneStepDistanceLimitMmRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_OneStepDistanceLimitPercentRadioBtn = new wxRadioButton( m_OneStepDistancePanel, wxID_ANY, wxT("%L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceUpperLimitSizer->Add( m_OneStepDistanceLimitPercentRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	
	m_OneStepDistanceSizer->Add( m_OneStepDistanceUpperLimitSizer, 1, wxEXPAND, 5 );
	
	m_OneStepDistanceDwellStaticText = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxT(" Dwell [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceDwellStaticText->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceDwellStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepDistanceDwellSpinCtrl = new wxSpinCtrl( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepDistanceSizer->Add( m_OneStepDistanceDwellSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText821 = new wxStaticText( m_OneStepDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText821->Wrap( -1 );
	m_OneStepDistanceSizer->Add( m_staticText821, 0, wxALL, 5 );
	
	m_OneStepDistancePhotoCheckBox = new wxCheckBox( m_OneStepDistancePanel, wxID_ANY, wxT("Photo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepDistanceSizer->Add( m_OneStepDistancePhotoCheckBox, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	m_OneStepDistancePanel->SetSizer( m_OneStepDistanceSizer );
	m_OneStepDistancePanel->Layout();
	m_OneStepDistanceSizer->Fit( m_OneStepDistancePanel );
	m_OneStepSizer21->Add( m_OneStepDistancePanel, 1, wxEXPAND | wxALL, 5 );
	
	
	m_OneStepPanel21->SetSizer( m_OneStepSizer21 );
	m_OneStepPanel21->Layout();
	m_OneStepSizer21->Fit( m_OneStepPanel21 );
	m_OneStepSizer1->Add( m_OneStepPanel21, 1, wxEXPAND | wxALL, 5 );
	
	m_OneStepPanel22 = new wxPanel( m_OneStepPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_OneStepSizer221;
	m_OneStepSizer221 = new wxFlexGridSizer( 6, 3, 0, 0 );
	m_OneStepSizer221->SetFlexibleDirection( wxBOTH );
	m_OneStepSizer221->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_OneStepEndOfEventRepeatCheckBox = new wxCheckBox( m_OneStepPanel22, wxID_ANY, wxT("Repeat [cycles]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventRepeatCheckBox, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepEndOfEventRepeatSpinCtrl = new wxSpinCtrl( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 1, 1000, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventRepeatSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText75 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText75, 0, 0, 5 );
	
	m_OneStepEndOfEventStaticText = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxT("End of event:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepEndOfEventStaticText->Wrap( -1 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText711 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText711, 0, wxALL, 5 );
	
	m_staticText72 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText72, 0, wxALL, 5 );
	
	m_OneStepEndOfEventStopRadioBtn = new wxRadioButton( m_OneStepPanel22, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepSizer221->Add( m_OneStepEndOfEventStopRadioBtn, 0, 0, 5 );
	
	m_staticText84 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText84->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText84, 0, wxALL, 5 );
	
	m_staticText85 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText85->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText85, 0, wxALL, 5 );
	
	m_OneStepEndOfEventHoldRadioBtn = new wxRadioButton( m_OneStepPanel22, wxID_ANY, wxT("Hold a distance:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventHoldRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepEndOfEventHoldSpinCtrl = new wxSpinCtrl( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventHoldSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_OneStepPanel3 = new wxPanel( m_OneStepPanel22, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_OneStepHoldSizer;
	m_OneStepHoldSizer = new wxBoxSizer( wxVERTICAL );
	
	m_OneStepEndOfEventHoldMmRelRadioBtn = new wxRadioButton( m_OneStepPanel3, wxID_ANY, wxT("mm rel."), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepHoldSizer->Add( m_OneStepEndOfEventHoldMmRelRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_OneStepEndOfEventHoldMmRadioBtn = new wxRadioButton( m_OneStepPanel3, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepHoldSizer->Add( m_OneStepEndOfEventHoldMmRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_OneStepEndOfEventHoldPercentRadioBtn = new wxRadioButton( m_OneStepPanel3, wxID_ANY, wxT("%L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepHoldSizer->Add( m_OneStepEndOfEventHoldPercentRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	
	m_OneStepPanel3->SetSizer( m_OneStepHoldSizer );
	m_OneStepPanel3->Layout();
	m_OneStepHoldSizer->Fit( m_OneStepPanel3 );
	m_OneStepSizer221->Add( m_OneStepPanel3, 1, 0, 5 );
	
	m_OneStepEndOfEventL0RadioBtn = new wxRadioButton( m_OneStepPanel22, wxID_ANY, wxT("Go to L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventL0RadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText803 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText803->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText803, 0, wxALL, 5 );
	
	m_staticText81 = new wxStaticText( m_OneStepPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	m_OneStepSizer221->Add( m_staticText81, 0, wxALL, 5 );
	
	m_OneStepEndOfEventMLRadioBtn = new wxRadioButton( m_OneStepPanel22, wxID_ANY, wxT("Go to mounting l."), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer221->Add( m_OneStepEndOfEventMLRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_OneStepSizer221->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_OneStepPanel22->SetSizer( m_OneStepSizer221 );
	m_OneStepPanel22->Layout();
	m_OneStepSizer221->Fit( m_OneStepPanel22 );
	m_OneStepSizer1->Add( m_OneStepPanel22, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText142 = new wxStaticText( m_OneStepPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText142->Wrap( -1 );
	m_OneStepSizer1->Add( m_staticText142, 0, wxALL, 5 );
	
	wxFlexGridSizer* m_OneStepSizer2;
	m_OneStepSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	m_OneStepSizer2->AddGrowableCol( 0 );
	m_OneStepSizer2->SetFlexibleDirection( wxBOTH );
	m_OneStepSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_OneStepSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OneStepCancelButton = new wxButton( m_OneStepPanel1, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer2->Add( m_OneStepCancelButton, 0, wxALL, 5 );
	
	m_OneStepSendButton = new wxButton( m_OneStepPanel1, wxID_ANY, wxT("Send to protocol"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer2->Add( m_OneStepSendButton, 0, wxALL, 5 );
	
	
	m_OneStepSizer1->Add( m_OneStepSizer2, 1, wxEXPAND, 5 );
	
	
	m_OneStepPanel1->SetSizer( m_OneStepSizer1 );
	m_OneStepPanel1->Layout();
	m_OneStepSizer1->Fit( m_OneStepPanel1 );
	m_Experiments->AddPage( m_OneStepPanel1, wxT("One Step Event"), false );
	m_ContinuosPanel = new wxPanel( m_Experiments, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ContinuousSizer1;
	m_ContinuousSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_ContinuousSizer1->AddGrowableCol( 0 );
	m_ContinuousSizer1->AddGrowableCol( 1 );
	m_ContinuousSizer1->AddGrowableRow( 0 );
	m_ContinuousSizer1->SetFlexibleDirection( wxBOTH );
	m_ContinuousSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ContinuousPanel21 = new wxPanel( m_ContinuosPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ContinuousSizer21;
	m_ContinuousSizer21 = new wxFlexGridSizer( 3, 1, 0, 0 );
	m_ContinuousSizer21->AddGrowableCol( 0 );
	m_ContinuousSizer21->AddGrowableRow( 1 );
	m_ContinuousSizer21->AddGrowableRow( 2 );
	m_ContinuousSizer21->SetFlexibleDirection( wxBOTH );
	m_ContinuousSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* m_ContinuousTypeSizer;
	m_ContinuousTypeSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	m_ContinuousTypeSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_ContinuousStressForceRadioBtn = new wxRadioButton( m_ContinuousPanel21, wxID_ANY, wxT("Stress/Force"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousTypeSizer->Add( m_ContinuousStressForceRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_ContinuousDistanceRadioBtn = new wxRadioButton( m_ContinuousPanel21, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousTypeSizer->Add( m_ContinuousDistanceRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousTypeSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_ContinuousSizer21->Add( m_ContinuousTypeSizer, 1, wxEXPAND, 5 );
	
	m_ContinuousStressForcePanel = new wxPanel( m_ContinuousPanel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_ContinuousStressForceSizer;
	m_ContinuousStressForceSizer = new wxFlexGridSizer( 7, 3, 0, 0 );
	m_ContinuousStressForceSizer->SetFlexibleDirection( wxBOTH );
	m_ContinuousStressForceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ContinuousStressForceVelocityStaticText = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxT(" Velocity:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_ContinuousStressForceVelocityStaticText->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceVelocityStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceVelocitySpinCtrl = new wxSpinCtrl( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceVelocitySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	wxBoxSizer* m_ContinuousStressForceVelocitySizer;
	m_ContinuousStressForceVelocitySizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousStressForceVelocityMmRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("mm/s"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousStressForceVelocitySizer->Add( m_ContinuousStressForceVelocityMmRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_ContinuousStressForceVelocityPercentRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("%L0/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceVelocitySizer->Add( m_ContinuousStressForceVelocityPercentRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceVelocitySizer, 1, wxALIGN_CENTER, 5 );
	
	m_ContinuousStressForceHoldTimeStaticText = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxT(" Hold time [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceHoldTimeStaticText->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceHoldTimeStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceHoldTimeSpinCtrl = new wxSpinCtrl( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceHoldTimeSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText782 = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText782->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_staticText782, 0, wxALL, 5 );
	
	m_ContinuousStressForceIncrementStaticText = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxT(" Increment [dN]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceIncrementStaticText->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceIncrementStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceIncrementSpinCtrl = new wxSpinCtrl( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceIncrementSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText802 = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText802->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_staticText802, 0, wxALL, 5 );
	
	
	m_ContinuousStressForceSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_ContinuousStressForceMaxValueRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("Max. value"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceMaxValueRadioBtn, 0, 0, 5 );
	
	m_ContinuousStressForceStepsRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("Steps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceStepsRadioBtn, 0, 0, 5 );
	
	m_ContinuousStressForceMaxValueStaticText = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxT(" Max. value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceMaxValueStaticText->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceMaxValueStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceMaxValueSpinCtrl = new wxSpinCtrl( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -50000, 50000, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceMaxValueSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* m_ContinuousStressForceMaxValueSizer;
	m_ContinuousStressForceMaxValueSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousStressForceMaxValueValueRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("N"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousStressForceMaxValueSizer->Add( m_ContinuousStressForceMaxValueValueRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceMaxValuePercentRadioBtn = new wxRadioButton( m_ContinuousStressForcePanel, wxID_ANY, wxT("%Fmax."), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceMaxValueSizer->Add( m_ContinuousStressForceMaxValuePercentRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceMaxValueSizer, 1, wxEXPAND, 5 );
	
	m_ContinuousStressForceStepsStaticText = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxT(" Steps:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceStepsStaticText->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceStepsStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousStressForceStepsSpinCtrl = new wxSpinCtrl( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 10, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForceStepsSpinCtrl, 0, 0, 5 );
	
	m_staticText76 = new wxStaticText( m_ContinuousStressForcePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	m_ContinuousStressForceSizer->Add( m_staticText76, 0, wxALL, 5 );
	
	m_ContinuousStressForcePhotoCheckBox = new wxCheckBox( m_ContinuousStressForcePanel, wxID_ANY, wxT("Photo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousStressForceSizer->Add( m_ContinuousStressForcePhotoCheckBox, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousStressForceSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_ContinuousStressForcePanel->SetSizer( m_ContinuousStressForceSizer );
	m_ContinuousStressForcePanel->Layout();
	m_ContinuousStressForceSizer->Fit( m_ContinuousStressForcePanel );
	m_ContinuousSizer21->Add( m_ContinuousStressForcePanel, 1, wxEXPAND, 5 );
	
	m_ContinuousDistancePanel = new wxPanel( m_ContinuousPanel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	m_ContinuousDistancePanel->Hide();
	
	wxFlexGridSizer* m_ContinuousDistanceSizer;
	m_ContinuousDistanceSizer = new wxFlexGridSizer( 7, 3, 0, 0 );
	m_ContinuousDistanceSizer->AddGrowableRow( 6 );
	m_ContinuousDistanceSizer->SetFlexibleDirection( wxBOTH );
	m_ContinuousDistanceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ContinuousDistanceVelocityStaticText = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxT(" Velocity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceVelocityStaticText->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceVelocityStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistanceVelocitySpinCtrl = new wxSpinCtrl( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceVelocitySpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_ContinuousDistancePanel21 = new wxPanel( m_ContinuousDistancePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_ContinuousDistanceVelocitySizer;
	m_ContinuousDistanceVelocitySizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousDistanceVelocityMmRadioBtn = new wxRadioButton( m_ContinuousDistancePanel21, wxID_ANY, wxT("mm/s"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousDistanceVelocitySizer->Add( m_ContinuousDistanceVelocityMmRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	m_ContinuousDistanceVelocityPercentRadioBtn = new wxRadioButton( m_ContinuousDistancePanel21, wxID_ANY, wxT("%L0/s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceVelocitySizer->Add( m_ContinuousDistanceVelocityPercentRadioBtn, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousDistancePanel21->SetSizer( m_ContinuousDistanceVelocitySizer );
	m_ContinuousDistancePanel21->Layout();
	m_ContinuousDistanceVelocitySizer->Fit( m_ContinuousDistancePanel21 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistancePanel21, 1, wxEXPAND, 5 );
	
	m_ContinuousDistanceHoldTimeStaticText = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxT(" Hold time [s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceHoldTimeStaticText->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceHoldTimeStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistanceHoldTimeSpinCtrl = new wxSpinCtrl( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceHoldTimeSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_staticText7811 = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7811->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_staticText7811, 0, wxALL, 5 );
	
	m_ContinuousDistanceIncrementStaticText = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxT(" Increment [dmm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceIncrementStaticText->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceIncrementStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistanceIncrementSpinCtrl = new wxSpinCtrl( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, -20, 20, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceIncrementSpinCtrl, 0, wxALIGN_CENTER, 5 );
	
	m_ContinuousDistancePanel22 = new wxPanel( m_ContinuousDistancePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_ContinuousDistanceIncrementSizer;
	m_ContinuousDistanceIncrementSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousDistanceIncrementMmRadioBtn = new wxRadioButton( m_ContinuousDistancePanel22, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceIncrementSizer->Add( m_ContinuousDistanceIncrementMmRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_ContinuousDistanceIncrementPercentRadioBtn = new wxRadioButton( m_ContinuousDistancePanel22, wxID_ANY, wxT("%L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceIncrementSizer->Add( m_ContinuousDistanceIncrementPercentRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	
	m_ContinuousDistancePanel22->SetSizer( m_ContinuousDistanceIncrementSizer );
	m_ContinuousDistancePanel22->Layout();
	m_ContinuousDistanceIncrementSizer->Fit( m_ContinuousDistancePanel22 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistancePanel22, 1, wxEXPAND | wxALL, 5 );
	
	
	m_ContinuousDistanceSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_ContinuousDistanceMaxValueRadioBtn = new wxRadioButton( m_ContinuousDistancePanel, wxID_ANY, wxT("Max. value"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceMaxValueRadioBtn, 0, 0, 5 );
	
	m_ContinuousDistanceStepsRadioBtn = new wxRadioButton( m_ContinuousDistancePanel, wxID_ANY, wxT("Steps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceStepsRadioBtn, 0, 0, 5 );
	
	m_ContinuousDistanceMaxValueStaticText = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxT(" Max. Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceMaxValueStaticText->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceMaxValueStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistanceMaxValueSpinCtrl = new wxSpinCtrl( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceMaxValueSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistancePanel23 = new wxPanel( m_ContinuousDistancePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_ContinuousDistanceMaxValueSizer;
	m_ContinuousDistanceMaxValueSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousDistanceMaxValueMmRelRadioBtn = new wxRadioButton( m_ContinuousDistancePanel23, wxID_ANY, wxT("mm rel"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousDistanceMaxValueSizer->Add( m_ContinuousDistanceMaxValueMmRelRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_ContinuousDistanceMaxValueMmRadioBtn = new wxRadioButton( m_ContinuousDistancePanel23, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceMaxValueSizer->Add( m_ContinuousDistanceMaxValueMmRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_ContinuousDistanceMaxValuePercentRadioBtn = new wxRadioButton( m_ContinuousDistancePanel23, wxID_ANY, wxT("%L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceMaxValueSizer->Add( m_ContinuousDistanceMaxValuePercentRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	
	m_ContinuousDistancePanel23->SetSizer( m_ContinuousDistanceMaxValueSizer );
	m_ContinuousDistancePanel23->Layout();
	m_ContinuousDistanceMaxValueSizer->Fit( m_ContinuousDistancePanel23 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistancePanel23, 1, wxEXPAND, 5 );
	
	m_ContinuousDistanceStepsStaticText = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxT(" Steps:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceStepsStaticText->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceStepsStaticText, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousDistanceStepsSpinCtrl = new wxSpinCtrl( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 10, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistanceStepsSpinCtrl, 0, 0, 5 );
	
	m_staticText801 = new wxStaticText( m_ContinuousDistancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText801->Wrap( -1 );
	m_ContinuousDistanceSizer->Add( m_staticText801, 0, wxALL, 5 );
	
	m_ContinuousDistancePhotoCheckBox = new wxCheckBox( m_ContinuousDistancePanel, wxID_ANY, wxT("Photo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousDistanceSizer->Add( m_ContinuousDistancePhotoCheckBox, 0, wxALIGN_CENTER, 5 );
	
	
	m_ContinuousDistancePanel->SetSizer( m_ContinuousDistanceSizer );
	m_ContinuousDistancePanel->Layout();
	m_ContinuousDistanceSizer->Fit( m_ContinuousDistancePanel );
	m_ContinuousSizer21->Add( m_ContinuousDistancePanel, 1, wxEXPAND, 5 );
	
	
	m_ContinuousPanel21->SetSizer( m_ContinuousSizer21 );
	m_ContinuousPanel21->Layout();
	m_ContinuousSizer21->Fit( m_ContinuousPanel21 );
	m_ContinuousSizer1->Add( m_ContinuousPanel21, 1, wxEXPAND | wxALL, 5 );
	
	m_ContinuousPanel22 = new wxPanel( m_ContinuosPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_OneStepSizer22;
	m_OneStepSizer22 = new wxFlexGridSizer( 7, 3, 0, 0 );
	m_OneStepSizer22->SetFlexibleDirection( wxBOTH );
	m_OneStepSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ContinuousEndOfEventRepeatCheckBox = new wxCheckBox( m_ContinuousPanel22, wxID_ANY, wxT("Repeat [cycles]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventRepeatCheckBox, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousEndOfEventRepeatSpinCtrl = new wxSpinCtrl( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventRepeatSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText94 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText94, 0, wxALL, 5 );
	
	m_OneStepEndOfEventStaticText1 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxT("End of event:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepEndOfEventStaticText1->Wrap( -1 );
	m_OneStepSizer22->Add( m_OneStepEndOfEventStaticText1, 0, wxALL, 5 );
	
	m_staticText7111 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7111->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText7111, 0, wxALL, 5 );
	
	m_staticText95 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText95->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText95, 0, wxALL, 5 );
	
	m_ContinuousEndOfEventStopRadioBtn = new wxRadioButton( m_ContinuousPanel22, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventStopRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText140 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText140->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText140, 0, wxALL, 5 );
	
	m_staticText96 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText96, 0, wxALL, 5 );
	
	m_ContinuousEndOfEventHoldDistanceRadioBtn = new wxRadioButton( m_ContinuousPanel22, wxID_ANY, wxT("Hold a distance:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventHoldDistanceRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousEndOfEventHoldDistanceSpinCtrl = new wxSpinCtrl( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventHoldDistanceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousPanel31 = new wxPanel( m_ContinuousPanel22, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_ContinuousHoldSizer;
	m_ContinuousHoldSizer = new wxBoxSizer( wxVERTICAL );
	
	m_ContinuousEndOfEventHoldMmRelRadioBtn = new wxRadioButton( m_ContinuousPanel31, wxID_ANY, wxT("mm rel."), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_ContinuousHoldSizer->Add( m_ContinuousEndOfEventHoldMmRelRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousEndOfEventHoldMmRadioBtn = new wxRadioButton( m_ContinuousPanel31, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousHoldSizer->Add( m_ContinuousEndOfEventHoldMmRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	m_ContinuousEndOfEventHoldPercentRadioBtn = new wxRadioButton( m_ContinuousPanel31, wxID_ANY, wxT("%L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousHoldSizer->Add( m_ContinuousEndOfEventHoldPercentRadioBtn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT, 5 );
	
	
	m_ContinuousPanel31->SetSizer( m_ContinuousHoldSizer );
	m_ContinuousPanel31->Layout();
	m_ContinuousHoldSizer->Fit( m_ContinuousPanel31 );
	m_OneStepSizer22->Add( m_ContinuousPanel31, 1, 0, 5 );
	
	m_ContinuousEndOfEventL0RadioBtn = new wxRadioButton( m_ContinuousPanel22, wxID_ANY, wxT("Go to L0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventL0RadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText83 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText83->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText83, 0, wxALL, 5 );
	
	m_staticText97 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText97, 0, wxALL, 5 );
	
	m_ContinuousEndOfEventMLRadioBtn = new wxRadioButton( m_ContinuousPanel22, wxID_ANY, wxT("Go to mounting length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventMLRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText86 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText86->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText86, 0, wxALL, 5 );
	
	m_staticText98 = new wxStaticText( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	m_OneStepSizer22->Add( m_staticText98, 0, wxALL, 5 );
	
	m_ContinuousEndOfEventHoldStressForceRadioBtn = new wxRadioButton( m_ContinuousPanel22, wxID_ANY, wxT("Stop at [N]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventHoldStressForceRadioBtn, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_ContinuousEndOfEventHoldStressForceSpinCtrl = new wxSpinCtrl( m_ContinuousPanel22, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_OneStepSizer22->Add( m_ContinuousEndOfEventHoldStressForceSpinCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	m_ContinuousPanel22->SetSizer( m_OneStepSizer22 );
	m_ContinuousPanel22->Layout();
	m_OneStepSizer22->Fit( m_ContinuousPanel22 );
	m_ContinuousSizer1->Add( m_ContinuousPanel22, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText143 = new wxStaticText( m_ContinuosPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText143->Wrap( -1 );
	m_ContinuousSizer1->Add( m_staticText143, 0, wxALL, 5 );
	
	wxFlexGridSizer* m_ContinuousSizer2;
	m_ContinuousSizer2 = new wxFlexGridSizer( 0, 3, 0, 0 );
	m_ContinuousSizer2->AddGrowableCol( 0 );
	m_ContinuousSizer2->SetFlexibleDirection( wxBOTH );
	m_ContinuousSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_ContinuousSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_ContinuousCancelButton = new wxButton( m_ContinuosPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousSizer2->Add( m_ContinuousCancelButton, 0, wxALL, 5 );
	
	m_ContinuousSendButton = new wxButton( m_ContinuosPanel, wxID_ANY, wxT("Send to protocol"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ContinuousSizer2->Add( m_ContinuousSendButton, 0, wxALL, 5 );
	
	
	m_ContinuousSizer1->Add( m_ContinuousSizer2, 1, wxEXPAND, 5 );
	
	
	m_ContinuosPanel->SetSizer( m_ContinuousSizer1 );
	m_ContinuosPanel->Layout();
	m_ContinuousSizer1->Fit( m_ContinuosPanel );
	m_Experiments->AddPage( m_ContinuosPanel, wxT("Continuous Event"), false );
	
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
	
	m_DistanceStaticText = new wxStaticText( this, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_DistanceStaticText->Wrap( -1 );
	m_StretcherDistanceSizer->Add( m_DistanceStaticText, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
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
	m_ProtocolsSizer2 = new wxFlexGridSizer( 8, 2, 0, 0 );
	m_ProtocolsSizer2->AddGrowableCol( 0 );
	m_ProtocolsSizer2->AddGrowableCol( 1 );
	m_ProtocolsSizer2->AddGrowableRow( 0 );
	m_ProtocolsSizer2->AddGrowableRow( 1 );
	m_ProtocolsSizer2->AddGrowableRow( 2 );
	m_ProtocolsSizer2->AddGrowableRow( 3 );
	m_ProtocolsSizer2->AddGrowableRow( 4 );
	m_ProtocolsSizer2->AddGrowableRow( 5 );
	m_ProtocolsSizer2->AddGrowableRow( 6 );
	m_ProtocolsSizer2->SetFlexibleDirection( wxBOTH );
	m_ProtocolsSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ProtocolsUpButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("↑"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsUpButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsTriggerButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Trigger"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsTriggerButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_ProtocolsXButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsXButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsPauseButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsPauseButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_ProtocolsDownButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("↓"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsDownButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_ProtocolsPauseResumeButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Pause/Resume"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsPauseResumeButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_ProtocolsLoopCheckBox = new wxCheckBox( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Loop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsLoopCheckBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_ProtocolsLoopSpinCtrl = new wxSpinCtrl( m_ProtocolsNavigationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsLoopSpinCtrl, 0, wxALL, 5 );
	
	m_ProtocolsCurrentLoopStaticText = new wxStaticText( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Current Loop:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsCurrentLoopStaticText->Wrap( -1 );
	m_ProtocolsSizer2->Add( m_ProtocolsCurrentLoopStaticText, 0, wxALL, 5 );
	
	m_ProtocolsCurrentLoopNrStaticText = new wxStaticText( m_ProtocolsNavigationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsCurrentLoopNrStaticText->Wrap( -1 );
	m_ProtocolsSizer2->Add( m_ProtocolsCurrentLoopNrStaticText, 0, wxALL, 5 );
	
	m_ProtocolsPreviewButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsPreviewButton, 0, wxALL, 5 );
	
	m_staticText91 = new wxStaticText( m_ProtocolsNavigationPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	m_ProtocolsSizer2->Add( m_staticText91, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_ProtocolsRunButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Run"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsRunButton, 0, wxALL, 5 );
	
	m_ProtocolsStopButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsStopButton, 0, wxALL, 5 );
	
	m_ProtocolsSaveButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsSaveButton, 0, wxALL, 5 );
	
	m_ProtocolsLoadButton = new wxButton( m_ProtocolsNavigationPanel, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ProtocolsSizer2->Add( m_ProtocolsLoadButton, 0, wxALL, 5 );
	
	
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
	
	m_LogTextCtrl = new wxTextCtrl( m_Results, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_ResultSizer1->Add( m_LogTextCtrl, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* m_ResultSizer2;
	m_ResultSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_LogClearButton = new wxButton( m_Results, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ResultSizer2->Add( m_LogClearButton, 0, wxALL, 5 );
	
	m_LogSaveButton = new wxButton( m_Results, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ResultSizer2->Add( m_LogSaveButton, 0, wxALL, 5 );
	
	
	m_ResultSizer1->Add( m_ResultSizer2, 1, wxEXPAND, 5 );
	
	
	m_Results->SetSizer( m_ResultSizer1 );
	m_Results->Layout();
	m_ResultSizer1->Fit( m_Results );
	bBottomSizer->Add( m_Results, 1, wxEXPAND | wxALL, 5 );
	
	m_GraphPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_GraphSizer1;
	m_GraphSizer1 = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_GraphSizer1->AddGrowableCol( 0 );
	m_GraphSizer1->AddGrowableRow( 0 );
	m_GraphSizer1->SetFlexibleDirection( wxBOTH );
	m_GraphSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* m_GraphSizer2;
	m_GraphSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_GraphExportCSVButton = new wxButton( m_GraphPanel, wxID_ANY, wxT("Export CSV"), wxDefaultPosition, wxDefaultSize, 0 );
	m_GraphSizer2->Add( m_GraphExportCSVButton, 0, wxALL, 5 );
	
	m_GraphExportPNGButton = new wxButton( m_GraphPanel, wxID_ANY, wxT("Export PNG"), wxDefaultPosition, wxDefaultSize, 0 );
	m_GraphSizer2->Add( m_GraphExportPNGButton, 0, wxALL, 5 );
	
	m_GraphClearButton = new wxButton( m_GraphPanel, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	m_GraphSizer2->Add( m_GraphClearButton, 0, wxALL, 5 );
	
	m_GraphTypeComboBox = new wxComboBox( m_GraphPanel, wxID_ANY, wxT("Distance"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_GraphTypeComboBox->Append( wxT("Distance") );
	m_GraphTypeComboBox->Append( wxT("Displacement") );
	m_GraphTypeComboBox->Append( wxT("Both") );
	m_GraphTypeComboBox->SetSelection( 0 );
	m_GraphSizer2->Add( m_GraphTypeComboBox, 0, wxALL, 5 );
	
	m_GraphLimitsCheckBox = new wxCheckBox( m_GraphPanel, wxID_ANY, wxT("Show limits"), wxDefaultPosition, wxDefaultSize, 0 );
	m_GraphSizer2->Add( m_GraphLimitsCheckBox, 0, wxALL, 5 );
	
	
	m_GraphSizer1->Add( m_GraphSizer2, 1, wxEXPAND, 5 );
	
	
	m_GraphPanel->SetSizer( m_GraphSizer1 );
	m_GraphPanel->Layout();
	m_GraphSizer1->Fit( m_GraphPanel );
	bBottomSizer->Add( m_GraphPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	bMainSizer->Add( bBottomSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_PortsMenuMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame_Base::2 ) );
	this->Connect( m_FileOutputMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame_Base::3 ) );
}

MyFrame_Base::~MyFrame_Base()
{
	// Disconnect Events
	this->Disconnect( wxANY_ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame_Base::2 ) );
	this->Disconnect( wxANY_ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame_Base::3 ) );
	
}

MySamplingFrequency_Base::MySamplingFrequency_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_SamplingFrequencySizer1;
	m_SamplingFrequencySizer1 = new wxFlexGridSizer( 5, 1, 0, 0 );
	m_SamplingFrequencySizer1->AddGrowableCol( 0 );
	m_SamplingFrequencySizer1->AddGrowableRow( 4 );
	m_SamplingFrequencySizer1->SetFlexibleDirection( wxBOTH );
	m_SamplingFrequencySizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_SamplingFrequencySizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_SamplingFrequencySizer2;
	m_SamplingFrequencySizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	m_SamplingFrequencySizer2->SetFlexibleDirection( wxBOTH );
	m_SamplingFrequencySizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_SamplingFrequencyDistanceStaticText = new wxStaticText( this, wxID_ANY, wxT("Measuring Frequency for Distance:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SamplingFrequencyDistanceStaticText->Wrap( -1 );
	m_SamplingFrequencySizer2->Add( m_SamplingFrequencyDistanceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SamplingFrequencyDistanceSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_SamplingFrequencySizer2->Add( m_SamplingFrequencyDistanceSpinCtrl, 0, wxALL, 5 );
	
	m_SamplingFrequencyForceStaticText = new wxStaticText( this, wxID_ANY, wxT("Measuring Frequency for Force:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SamplingFrequencyForceStaticText->Wrap( -1 );
	m_SamplingFrequencySizer2->Add( m_SamplingFrequencyForceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_SamplingFrequencyForceSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_SamplingFrequencySizer2->Add( m_SamplingFrequencyForceSpinCtrl, 0, wxALL, 5 );
	
	
	m_SamplingFrequencySizer1->Add( m_SamplingFrequencySizer2, 1, wxEXPAND, 5 );
	
	
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
	m_PortsSizer1 = new wxFlexGridSizer( 4, 0, 0, 0 );
	m_PortsSizer1->AddGrowableCol( 0 );
	m_PortsSizer1->AddGrowableRow( 3 );
	m_PortsSizer1->SetFlexibleDirection( wxBOTH );
	m_PortsSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_PortsSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_PortsSizer2;
	m_PortsSizer2 = new wxFlexGridSizer( 3, 2, 0, 0 );
	m_PortsSizer2->SetFlexibleDirection( wxBOTH );
	m_PortsSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_PortsMotor1StaticText = new wxStaticText( this, wxID_ANY, wxT("Motor 1:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsMotor1StaticText->Wrap( -1 );
	m_PortsSizer2->Add( m_PortsMotor1StaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_PortsMotor1ChoiceChoices[] = { wxT("ttyUSB0"), wxT("ttyUSB1"), wxT("ttyUSB2"), wxT("ttyUSB3"), wxT("ttyUSB4"), wxT("ttyUSB5") };
	int m_PortsMotor1ChoiceNChoices = sizeof( m_PortsMotor1ChoiceChoices ) / sizeof( wxString );
	m_PortsMotor1Choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsMotor1ChoiceNChoices, m_PortsMotor1ChoiceChoices, 0 );
	m_PortsMotor1Choice->SetSelection( 0 );
	m_PortsSizer2->Add( m_PortsMotor1Choice, 0, wxALL, 5 );
	
	m_PortsMotor2StaticText = new wxStaticText( this, wxID_ANY, wxT("Motor 2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsMotor2StaticText->Wrap( -1 );
	m_PortsSizer2->Add( m_PortsMotor2StaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_PortsMotor2ChoiceChoices[] = { wxT("ttyUSB0"), wxT("ttyUSB1"), wxT("ttyUSB2"), wxT("ttyUSB3"), wxT("ttyUSB4"), wxT("ttyUSB5") };
	int m_PortsMotor2ChoiceNChoices = sizeof( m_PortsMotor2ChoiceChoices ) / sizeof( wxString );
	m_PortsMotor2Choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsMotor2ChoiceNChoices, m_PortsMotor2ChoiceChoices, 0 );
	m_PortsMotor2Choice->SetSelection( 1 );
	m_PortsSizer2->Add( m_PortsMotor2Choice, 0, wxALL, 5 );
	
	m_PortsForceStaticText = new wxStaticText( this, wxID_ANY, wxT("Force Sensor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_PortsForceStaticText->Wrap( -1 );
	m_PortsSizer2->Add( m_PortsForceStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString m_PortsForceChoiceChoices[] = { wxT("ttyUSB0"), wxT("ttyUSB1"), wxT("ttyUSB2"), wxT("ttyUSB3"), wxT("ttyUSB4"), wxT("ttyUSB5") };
	int m_PortsForceChoiceNChoices = sizeof( m_PortsForceChoiceChoices ) / sizeof( wxString );
	m_PortsForceChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_PortsForceChoiceNChoices, m_PortsForceChoiceChoices, 0 );
	m_PortsForceChoice->SetSelection( 2 );
	m_PortsSizer2->Add( m_PortsForceChoice, 0, wxALL, 5 );
	
	
	m_PortsSizer1->Add( m_PortsSizer2, 1, wxEXPAND, 5 );
	
	
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

MyFileOutput_Base::MyFileOutput_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_FileOutputSizer1;
	m_FileOutputSizer1 = new wxFlexGridSizer( 4, 0, 0, 0 );
	m_FileOutputSizer1->AddGrowableCol( 0 );
	m_FileOutputSizer1->AddGrowableRow( 3 );
	m_FileOutputSizer1->SetFlexibleDirection( wxBOTH );
	m_FileOutputSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_FileOutputSizer1->Add( 0, 10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* m_FileOutputSizer2;
	m_FileOutputSizer2 = new wxFlexGridSizer( 1, 2, 0, 0 );
	m_FileOutputSizer2->SetFlexibleDirection( wxBOTH );
	m_FileOutputSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_FileOutputNameStaticText = new wxStaticText( this, wxID_ANY, wxT("Path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_FileOutputNameStaticText->Wrap( -1 );
	m_FileOutputSizer2->Add( m_FileOutputNameStaticText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_FileOutputNameDirPicker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_CHANGE_DIR|wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	m_FileOutputSizer2->Add( m_FileOutputNameDirPicker, 0, wxALL, 5 );
	
	
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
	
	this->Centre( wxBOTH );
}

MyFileOutput_Base::~MyFileOutput_Base()
{
}

MyHomeStages_Base::MyHomeStages_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_HomeStagesSizer1;
	m_HomeStagesSizer1 = new wxFlexGridSizer( 4, 0, 0, 0 );
	m_HomeStagesSizer1->SetFlexibleDirection( wxBOTH );
	m_HomeStagesSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("Are you sure that nothing is mounted and/or will be destroyed when the stages go to the home position."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	m_HomeStagesSizer1->Add( m_staticText62, 0, wxALL, 5 );
	
	m_HomeStagesSdbSizer = new wxStdDialogButtonSizer();
	m_HomeStagesSdbSizerOK = new wxButton( this, wxID_OK );
	m_HomeStagesSdbSizer->AddButton( m_HomeStagesSdbSizerOK );
	m_HomeStagesSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_HomeStagesSdbSizer->AddButton( m_HomeStagesSdbSizerCancel );
	m_HomeStagesSdbSizer->Realize();
	
	m_HomeStagesSizer1->Add( m_HomeStagesSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_HomeStagesSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MyHomeStages_Base::~MyHomeStages_Base()
{
}

MyExportDialog_Base::MyExportDialog_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_ExportDialogSizer1;
	m_ExportDialogSizer1 = new wxFlexGridSizer( 5, 1, 0, 0 );
	m_ExportDialogSizer1->AddGrowableCol( 0 );
	m_ExportDialogSizer1->AddGrowableRow( 4 );
	m_ExportDialogSizer1->SetFlexibleDirection( wxBOTH );
	m_ExportDialogSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_ExportDialog1StaticText = new wxStaticText( this, wxID_ANY, wxT("Please select a folder and filename."), wxDefaultPosition, wxDefaultSize, 0 );
	m_ExportDialog1StaticText->Wrap( -1 );
	m_ExportDialogSizer1->Add( m_ExportDialog1StaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_ExportDialogFilePicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Choose a folder and insert a filename."), wxT("*.txt"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE );
	m_ExportDialogSizer1->Add( m_ExportDialogFilePicker, 0, wxALL, 5 );
	
	m_ExportDialog2StaticText = new wxStaticText( this, wxID_ANY, wxT("Please select the experiments from which you don't want to export the measurement values."), wxDefaultPosition, wxDefaultSize, 0 );
	m_ExportDialog2StaticText->Wrap( -1 );
	m_ExportDialogSizer1->Add( m_ExportDialog2StaticText, 0, wxALL, 5 );
	
	m_ExportDialogListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( 25,450 ), 0, NULL, wxLB_MULTIPLE ); 
	m_ExportDialogSizer1->Add( m_ExportDialogListBox, 0, wxALL|wxEXPAND, 5 );
	
	m_ExportDialogSdbSizer = new wxStdDialogButtonSizer();
	m_ExportDialogSdbSizerOK = new wxButton( this, wxID_OK );
	m_ExportDialogSdbSizer->AddButton( m_ExportDialogSdbSizerOK );
	m_ExportDialogSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_ExportDialogSdbSizer->AddButton( m_ExportDialogSdbSizerCancel );
	m_ExportDialogSdbSizer->Realize();
	
	m_ExportDialogSizer1->Add( m_ExportDialogSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_ExportDialogSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MyExportDialog_Base::~MyExportDialog_Base()
{
}

MyPauseDialog_Base::MyPauseDialog_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* m_PauseDialogSizer1;
	m_PauseDialogSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText69 = new wxStaticText( this, wxID_ANY, wxT("Please insert the waiting time [sec]."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	m_PauseDialogSizer1->Add( m_staticText69, 0, wxALL|wxEXPAND, 5 );
	
	m_PauseDialogWaitingTimeSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000000, 0 );
	m_PauseDialogSizer1->Add( m_PauseDialogWaitingTimeSpinCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_PauseDialogSdbSizer = new wxStdDialogButtonSizer();
	m_PauseDialogSdbSizerOK = new wxButton( this, wxID_OK );
	m_PauseDialogSdbSizer->AddButton( m_PauseDialogSdbSizerOK );
	m_PauseDialogSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_PauseDialogSdbSizer->AddButton( m_PauseDialogSdbSizerCancel );
	m_PauseDialogSdbSizer->Realize();
	
	m_PauseDialogSizer1->Add( m_PauseDialogSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_PauseDialogSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MyPauseDialog_Base::~MyPauseDialog_Base()
{
}

MyStartUpDialog_Base::MyStartUpDialog_Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* m_StartUpDialogSizer1;
	m_StartUpDialogSizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	m_StartUpDialogSizer1->AddGrowableCol( 0 );
	m_StartUpDialogSizer1->SetFlexibleDirection( wxBOTH );
	m_StartUpDialogSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_StartUpDialogPanel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* m_StartUpDialogSizer21;
	m_StartUpDialogSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_StartUpHomingStaticText = new wxStaticText( m_StartUpDialogPanel1, wxID_ANY, wxT("If the stages were moved outside this software or the software\ncrashed, the stages needs to be homed."), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_StartUpHomingStaticText->Wrap( -1 );
	m_StartUpDialogSizer21->Add( m_StartUpHomingStaticText, 0, wxALL|wxEXPAND, 5 );
	
	
	m_StartUpDialogPanel1->SetSizer( m_StartUpDialogSizer21 );
	m_StartUpDialogPanel1->Layout();
	m_StartUpDialogSizer21->Fit( m_StartUpDialogPanel1 );
	m_StartUpDialogSizer1->Add( m_StartUpDialogPanel1, 1, wxEXPAND | wxALL, 5 );
	
	m_StartUpLEStaticText = new wxStaticText( this, wxID_ANY, wxT("Move the stages to the distance with actuator collision, insert the\ndistance [mm] and push the button \"Set\"."), wxDefaultPosition, wxDefaultSize, 0 );
	m_StartUpLEStaticText->Wrap( -1 );
	m_StartUpDialogSizer1->Add( m_StartUpLEStaticText, 0, wxALL|wxEXPAND, 5 );
	
	m_StartUpDialogPanel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* m_StartUpDialogSizer22;
	m_StartUpDialogSizer22 = new wxFlexGridSizer( 1, 2, 0, 0 );
	m_StartUpDialogSizer22->AddGrowableCol( 0 );
	m_StartUpDialogSizer22->AddGrowableCol( 1 );
	m_StartUpDialogSizer22->AddGrowableRow( 0 );
	m_StartUpDialogSizer22->SetFlexibleDirection( wxBOTH );
	m_StartUpDialogSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_StartUpDecreaseDistanceButton = new wxButton( m_StartUpDialogPanel2, wxID_ANY, wxT("--> <--"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StartUpDialogSizer22->Add( m_StartUpDecreaseDistanceButton, 0, wxALL|wxEXPAND, 5 );
	
	m_StartUpIncreaseDistanceButton = new wxButton( m_StartUpDialogPanel2, wxID_ANY, wxT("<-- -->"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StartUpDialogSizer22->Add( m_StartUpIncreaseDistanceButton, 0, wxALL|wxEXPAND, 5 );
	
	
	m_StartUpDialogPanel2->SetSizer( m_StartUpDialogSizer22 );
	m_StartUpDialogPanel2->Layout();
	m_StartUpDialogSizer22->Fit( m_StartUpDialogPanel2 );
	m_StartUpDialogSizer1->Add( m_StartUpDialogPanel2, 1, wxEXPAND | wxALL, 5 );
	
	m_StartUpInputSpinCtrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 1000, 0 );
	m_StartUpDialogSizer1->Add( m_StartUpInputSpinCtrl, 0, wxALL|wxEXPAND, 5 );
	
	m_StartUpSetButton = new wxButton( this, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_StartUpDialogSizer1->Add( m_StartUpSetButton, 0, wxALL|wxEXPAND, 5 );
	
	m_StartUpDialogSdbSizer = new wxStdDialogButtonSizer();
	m_StartUpDialogSdbSizerOK = new wxButton( this, wxID_OK );
	m_StartUpDialogSdbSizer->AddButton( m_StartUpDialogSdbSizerOK );
	m_StartUpDialogSdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_StartUpDialogSdbSizer->AddButton( m_StartUpDialogSdbSizerCancel );
	m_StartUpDialogSdbSizer->Realize();
	
	m_StartUpDialogSizer1->Add( m_StartUpDialogSdbSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_StartUpDialogSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MyStartUpDialog_Base::~MyStartUpDialog_Base()
{
}
