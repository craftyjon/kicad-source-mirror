///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 22 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_PCB_TEXT_PROPERTIES_BASE_H__
#define __DIALOG_PCB_TEXT_PROPERTIES_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class PCB_LAYER_BOX_SELECTOR;
class TEXT_CTRL_EVAL;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/bmpcbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_PCB_TEXT_PROPERTIES_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_PCB_TEXT_PROPERTIES_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_TextLabel;
		wxTextCtrl* m_TextContentCtrl;
		wxStaticText* m_SizeXLabel;
		wxStaticText* m_PositionXLabel;
		wxStaticText* m_LayerLabel;
		wxStaticText* m_staticText10;
		TEXT_CTRL_EVAL* m_SizeXCtrl;
		TEXT_CTRL_EVAL* m_PositionXCtrl;
		PCB_LAYER_BOX_SELECTOR* m_LayerSelectionCtrl;
		wxChoice* m_DisplayCtrl;
		wxStaticText* m_SizeYLabel;
		wxStaticText* m_PositionYLabel;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText11;
		TEXT_CTRL_EVAL* m_SizeYCtrl;
		TEXT_CTRL_EVAL* m_PositionYCtrl;
		wxChoice* m_StyleCtrl;
		wxChoice* m_justifyChoice;
		wxStaticText* m_ThicknessLabel;
		wxStaticText* m_orientationLabel;
		TEXT_CTRL_EVAL* m_ThicknessCtrl;
		TEXT_CTRL_EVAL* m_OrientCtrl;
		wxStaticLine* m_staticline;
		wxStdDialogButtonSizer* m_StandardSizer;
		wxButton* m_StandardSizerOK;
		wxButton* m_StandardSizerCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDlg( wxInitDialogEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_PCB_TEXT_PROPERTIES_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Text Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~DIALOG_PCB_TEXT_PROPERTIES_BASE();
	
};

#endif //__DIALOG_PCB_TEXT_PROPERTIES_BASE_H__
