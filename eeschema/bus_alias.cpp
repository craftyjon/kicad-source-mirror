/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2017 CERN
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

#include <algorithm>

#include "bus_alias.h"


BUS_ALIAS::BUS_ALIAS( SCH_SCREEN* aParent ) :
    m_parent( aParent )
{
}


BUS_ALIAS::~BUS_ALIAS()
{
}


bool BUS_ALIAS::Contains( const wxString& aName )
{
    return ( std::find( m_members.begin(), m_members.end(), aName )
             != m_members.end() );
}
