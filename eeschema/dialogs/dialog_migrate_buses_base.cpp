///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 30 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dialog_migrate_buses_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_MIGRATE_BUSES_BASE::DIALOG_MIGRATE_BUSES_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* main_sizer;
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("This schematic has one or more buses with more than one label.  This was allowed in previous KiCad versions but is no longer permitted."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( 480 );
	main_sizer->Add( m_staticText5, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Please select a new name for each of the buses below.\nA name has been suggested for you based on the labels attached\nto the bus."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( 480 );
	m_staticText7->SetMinSize( wxSize( -1,60 ) );
	
	main_sizer->Add( m_staticText7, 0, wxALL, 5 );
	
	m_migration_list = new wxListView( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	m_migration_list->SetMinSize( wxSize( 460,100 ) );
	
	main_sizer->Add( m_migration_list, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Proposed new name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	main_sizer->Add( m_staticText6, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cb_new_name = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_cb_new_name->SetMinSize( wxSize( 300,-1 ) );
	m_cb_new_name->SetMaxSize( wxSize( 460,-1 ) );
	
	bSizer7->Add( m_cb_new_name, 0, wxALL, 5 );
	
	m_btn_accept = new wxButton( this, wxID_ANY, wxT("Accept Name"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_btn_accept, 0, wxALL, 5 );
	
	
	main_sizer->Add( bSizer7, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1->Realize();
	
	main_sizer->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );
}

DIALOG_MIGRATE_BUSES_BASE::~DIALOG_MIGRATE_BUSES_BASE()
{
}
