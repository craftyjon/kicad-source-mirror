/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2014-2019 KiCad Developers, see AUTHORS.txt for contributors.
 * @author Jon Evans <jon@craftyjon.com>
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
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _APPEARANCE_PANEL_H
#define _APPEARANCE_PANEL_H

#include <appearance_panel_base.h>
#include <gal/color4d.h>

class PCB_BASE_FRAME;

using KIGFX::COLOR4D;


class APPEARANCE_PANEL : public APPEARANCE_PANEL_BASE
{
public:
    struct APPEARANCE_SETTING
    {
        int         id;
        wxString    label;
        wxString    tooltip;
        COLOR4D     color;
        bool        visible;
        bool        can_control_visibility;
        bool        can_control_opacity;

        APPEARANCE_SETTING( const wxString& aLabel, int aId, COLOR4D aColor = COLOR4D::UNSPECIFIED,
                            const wxString& aTooltip = wxEmptyString, bool aVisible = true,
                            bool aCanControlVisibility = true, bool aCanControlOpacity = true )
        {
            label   = aLabel;
            id      = aId;
            color   = aColor;
            visible = aVisible;
            tooltip = aTooltip;
            can_control_visibility = aCanControlVisibility;
            can_control_opacity = aCanControlOpacity;
        }

        APPEARANCE_SETTING() : id( -1 ), label( "" ), tooltip( "" ), color( COLOR4D::UNSPECIFIED ),
                               visible( false ), can_control_visibility( false ),
                               can_control_opacity( false )
        {}
    };

    APPEARANCE_PANEL( PCB_BASE_FRAME* aParent, wxWindow* aFocusOwner, bool aFpEditorMode = false );

    wxSize GetBestSize() const;

private:
    wxWindow* m_focus_owner;

    static const APPEARANCE_PANEL::APPEARANCE_SETTING s_render_rows[];

    void rebuild();
};


#endif
