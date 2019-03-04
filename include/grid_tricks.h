/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2012 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef _GRID_TRICKS_H_
#define _GRID_TRICKS_H_


#include <wx/grid.h>
#include <wx/event.h>
#include <widgets/wx_grid.h>

enum
{
    GRIDTRICKS_FIRST_ID = 901,
    GRIDTRICKS_ID_CUT,
    GRIDTRICKS_ID_COPY,
    GRIDTRICKS_ID_PASTE,
    GRIDTRICKS_ID_SELECT,

    GRIDTRICKS_FIRST_SHOWHIDE = 979,    // reserve 20 IDs for show/hide-column-n

    GRIDTRICKS_LAST_ID = 999
};


/**
 * Class GRID_TRICKS
 * is used to add mouse and command handling (such as cut, copy, and paste) to a WX_GRID instance.
 */
class GRID_TRICKS : public wxEvtHandler
{
public:

    explicit GRID_TRICKS( WX_GRID* aGrid );

protected:
    WX_GRID* m_grid;     ///< I don't own the grid, but he owns me

    // row & col "selection" acquisition
    // selected area by cell coordinate and count
    int      m_sel_row_start;
    int      m_sel_col_start;
    int      m_sel_row_count;
    int      m_sel_col_count;

    /// Puts the selected area into a sensible rectangle of m_sel_{row,col}_{start,count} above.
    void getSelectedArea();

    static bool isCtl( int aChar, const wxKeyEvent& e )
    {
        return e.GetKeyCode() == aChar && e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown();
    }

    void onGridCellLeftClick( wxGridEvent& event );
    void onGridCellLeftDClick( wxGridEvent& event );
    void onGridCellRightClick( wxGridEvent& event );
    void onGridLabelRightClick( wxGridEvent& event );
    void onPopupSelection( wxCommandEvent& event );
    void onKeyDown( wxKeyEvent& ev );
    void onUpdateUI( wxUpdateUIEvent& event );

    virtual bool handleDoubleClick( wxGridEvent& aEvent );
    virtual void showPopupMenu( wxMenu& menu );
    virtual void doPopupSelection( wxCommandEvent& event );

    bool toggleCell( int aRow, int aCol );
    bool showEditor( int aRow, int aCol );

    virtual void paste_clipboard();
    virtual void paste_text( const wxString& cb_text );
    virtual void cutcopy( bool doCut );
};

#endif  // _GRID_TRICKS_H_
