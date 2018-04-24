///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 19 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_EESCHEMA_OPTIONS_BASE_H__
#define __DIALOG_EESCHEMA_OPTIONS_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include "widgets/stepped_slider.h"
#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_EESCHEMA_OPTIONS_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_EESCHEMA_OPTIONS_BASE : public DIALOG_SHIM
{
	DECLARE_EVENT_TABLE()
	private:
		
		// Private event handlers
		void _wxFB_OnSize( wxSizeEvent& event ){ OnSize( event ); }
		void _wxFB_OnScaleSlider( wxScrollEvent& event ){ OnScaleSlider( event ); }
		void _wxFB_OnScaleAuto( wxCommandEvent& event ){ OnScaleAuto( event ); }
		void _wxFB_OnChooseUnits( wxCommandEvent& event ){ OnChooseUnits( event ); }
		void _wxFB_OnAddButtonClick( wxCommandEvent& event ){ OnAddButtonClick( event ); }
		void _wxFB_OnDeleteButtonClick( wxCommandEvent& event ){ OnDeleteButtonClick( event ); }
		
	
	protected:
		enum
		{
			ID_M_SPINAUTOSAVEINTERVAL = 1000,
			xwID_ANY,
			wxID_ADD_FIELD,
			wxID_DELETE_FIELD
		};
		
		wxNotebook* m_notebook;
		wxPanel* m_panel5;
		wxStaticText* m_staticText3;
		wxChoice* m_choiceGridSize;
		wxStaticText* m_staticGridUnits;
		wxStaticText* m_staticText51;
		wxSpinCtrl* m_spinBusWidth;
		wxStaticText* m_staticBusWidthUnits;
		wxStaticText* m_staticText5;
		wxSpinCtrl* m_spinLineWidth;
		wxStaticText* m_staticLineWidthUnits;
		wxStaticText* m_staticText26;
		wxChoice* m_choiceSeparatorRefId;
		wxStaticText* m_stIconScale;
		STEPPED_SLIDER* m_scaleSlider;
		wxStaticText* m_staticText211;
		wxCheckBox* m_scaleAuto;
		wxStaticLine* m_staticline3;
		wxCheckBox* m_checkShowGrid;
		wxCheckBox* m_checkHVOrientation;
		wxCheckBox* m_checkShowHiddenPins;
		wxCheckBox* m_checkPageLimits;
		wxCheckBox* m_footprintPreview;
		wxPanel* m_panel3;
		wxStaticText* m_staticText2;
		wxChoice* m_choiceUnits;
		wxStaticText* m_staticTextHpitch;
		wxSpinCtrl* m_spinRepeatHorizontal;
		wxStaticText* m_staticRepeatXUnits;
		wxStaticText* m_staticTextVpitch;
		wxSpinCtrl* m_spinRepeatVertical;
		wxStaticText* m_staticRepeatYUnits;
		wxStaticText* m_staticText16;
		wxSpinCtrl* m_spinRepeatLabel;
		wxStaticText* m_staticTextTsize;
		wxSpinCtrl* m_spinTextSize;
		wxStaticText* m_staticTextSizeUnits;
		wxStaticText* m_staticTextTimeInterval;
		wxSpinCtrl* m_spinAutoSaveInterval;
		wxStaticText* m_staticText23;
		wxStaticLine* m_staticline2;
		wxCheckBox* m_checkAutoplaceFields;
		wxCheckBox* m_checkAutoplaceJustify;
		wxCheckBox* m_checkAutoplaceAlign;
		wxPanel* m_tabControls;
		wxBoxSizer* m_controlsSizer;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText21;
		wxPanel* m_panelHotkeys;
		wxCheckBox* m_checkEnableZoomCenter;
		wxCheckBox* m_checkEnableMousewheelPan;
		wxCheckBox* m_checkAutoPan;
		wxPanel* m_tabColors;
		wxPanel* m_panelColors;
		wxPanel* m_panel2;
		wxGrid* m_fieldGrid;
		wxButton* addFieldButton;
		wxButton* deleteFieldButton;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		wxButton* m_sdbSizerCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnScaleSlider( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnScaleAuto( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChooseUnits( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_EESCHEMA_OPTIONS_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Schematic Editor Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DIALOG_EESCHEMA_OPTIONS_BASE();
	
};

#endif //__DIALOG_EESCHEMA_OPTIONS_BASE_H__
