/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2010 CERN
 * Copyright (C) 2014-2018 KiCad Developers, see CHANGELOG.TXT for contributors.
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

/**
 * These strings are used in menus and tools, that do the same command
 * But they are internationalized, and therefore must be created
 * at run time, on the fly.
 * So they cannot be static.
 *
 * Therefore they are defined by \#define, used inside menu constructors
 */

#define HELP_ZOOM_IN     _( "Zoom in" )
#define HELP_ZOOM_OUT    _( "Zoom out" )
#define HELP_ZOOM_FIT    _( "Zoom to fit schematic page" )
#define HELP_ZOOM_REDRAW _( "Redraw schematic view" )

// Schematic editor:
#define HELP_IMPORT_FOOTPRINTS \
    _( "Back-import symbol footprint association fields from the .cmp back import file created by Pcbnew" )

