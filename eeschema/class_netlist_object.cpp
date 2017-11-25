/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2013 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file class_netlist_object.cpp
 * @brief Class NETLIST_OBJECT to handle 1 item connected (in netlist and erc calculations)
 */

#include <fctsys.h>
#include <macros.h>
#include <schframe.h>

#include <sch_component.h>
#include <class_netlist_object.h>

#include <wx/regex.h>
#include <wx/tokenzr.h>


/**
 *
 * Buses can be defined in multiple ways. A homogenous bus is defined as a
 * vector:
 *
 *     BUS_NAME[M..N]
 *
 * For example, the bus A[3..0] will contain nets A3, A2, A1, and A0.
 * The BUS_NAME is required.  M and N must be integers but do not need to be in
 * any particular order -- A[0..3] produces the same result.
 *
 * Like net names, bus names cannot contain whitespace.
 *
 * A heterogenous bus is just a grouping of signals, separated by spaces, some
 * of which may be vectors.  Heterogenous buses can have names, but do not need to.
 *
 *     MEMORY{A[15..0] D[7..0] RW CE OE}
 *
 * In named heterogenous buses, the net names are expanded as <BUS_NAME>.<NET_NAME>
 * In the above example, the nets would be named like MEMORY.A15, MEMORY.D0, MEMORY.OE, etc.
 *
 *     {USB_DP USB_DN}
 *
 * In the above example, the bus is unnamed and
 *
 */
static wxRegEx busLabelRe( wxT( "^([^[:space:]]+)(\\[[\\d]+\\.+[\\d]+\\])$" ), wxRE_ADVANCED );
static wxRegEx heteroBusLabelRe( wxT( "^([^[:space:]]+)?\\{((?:[^[:space:]]+(?:\\[[\\d]+\\.+[\\d]+\\])? ?)+)\\}$" ), wxRE_ADVANCED );


bool IsBusLabel( const wxString& aLabel )
{
    return IsBusVectorLabel( aLabel ) || IsHeteroBusLabel( aLabel );
}


bool IsBusVectorLabel( const wxString& aLabel )
{
    wxCHECK_MSG( busLabelRe.IsValid(), false,
                 wxT( "Invalid regular expression in IsBusLabel()." ) );

    return busLabelRe.Matches( aLabel );
}


bool IsHeteroBusLabel( const wxString& aLabel )
{
    wxCHECK_MSG( heteroBusLabelRe.IsValid(), false,
                 wxT( "Invalid regular expression in IsHeteroBusLabel()." ) );

    return heteroBusLabelRe.Matches( aLabel );
}


#if defined(DEBUG)

#include <iostream>
const char* ShowType( NETLIST_ITEM_T aType )
{
    const char* ret;

    switch( aType )
    {
    case NET_SEGMENT:
        ret = "segment";            break;

    case NET_BUS:
        ret = "bus";                break;

    case NET_JUNCTION:
        ret = "junction";           break;

    case NET_LABEL:
        ret = "label";              break;

    case NET_HIERLABEL:
        ret = "hierlabel";          break;

    case NET_GLOBLABEL:
        ret = "glabel";             break;

    case NET_BUSLABELMEMBER:
        ret = "buslblmember";       break;

    case NET_HIERBUSLABELMEMBER:
        ret = "hierbuslblmember";   break;

    case NET_GLOBBUSLABELMEMBER:
        ret = "gbuslblmember";      break;

    case NET_SHEETBUSLABELMEMBER:
        ret = "sbuslblmember";      break;

    case NET_SHEETLABEL:
        ret = "sheetlabel";         break;

    case NET_PINLABEL:
        ret = "pinlabel";           break;

    case NET_PIN:
        ret = "pin";                break;

    case NET_NOCONNECT:
        ret = "noconnect";          break;

    default:
        ret = "??";                 break;
    }

    return ret;
}


void NETLIST_OBJECT::Show( std::ostream& out, int ndx ) const
{
    wxString path = m_SheetPath.PathHumanReadable();

    out << "<netItem ndx=\"" << ndx << '"' <<
    " type=\"" << ShowType( m_Type ) << '"' <<
    " netCode=\"" << GetNet() << '"' <<
    " sheet=\"" << TO_UTF8( path ) << '"' <<
    ">\n";

    out << " <start " << m_Start << "/> <end " << m_End << "/>\n";

    if( !m_Label.IsEmpty() )
        out << " <label>" << m_Label.mb_str() << "</label>\n";

    out << " <sheetpath>" << m_SheetPath.PathHumanReadable().mb_str() << "</sheetpath>\n";

    switch( m_Type )
    {
    case NET_PIN:
        /* GetRef() needs to be const
        out << " <refOfComp>" << GetComponentParent()->GetRef(&m_SheetPath).mb_str()
            << "</refOfComp>\n";
        */

        if( m_Comp )
            m_Comp->Show( 1, out );

        break;

    default:
        // not all the m_Comp classes have working Show functions.
        ;
    }

/*  was segfault-ing
    if( m_Comp )
        m_Comp->Show( 1, out );     // labels may not have good Show() funcs?
    else
        out << " m_Comp==NULL\n";
*/

    out << "</netItem>\n";
}

#endif


NETLIST_OBJECT::NETLIST_OBJECT()
{
    m_Type = NET_ITEM_UNSPECIFIED;  /* Type of this item (see NETLIST_ITEM_T enum) */
    m_Comp = NULL;                  /* Pointer on the library item that created this net object
                                     * (the parent)*/
    m_Link = NULL;                  /* For SCH_SHEET_PIN:
                                     * Pointer to the hierarchy sheet that contains this
                                     * SCH_SHEET_PIN For Pins: pointer to the component that
                                     * contains this pin
                                     */
    m_Flag = 0;                     /* flag used in calculations */
    m_ElectricalPinType = PIN_INPUT;   /* Has meaning only for Pins: electrical type of the pin
                                     * used to detect conflicts between pins in ERC
                                     */
    m_netCode    = 0;               /* net code for all items except BUS labels because a BUS
                                     * label has as many net codes as bus members
                                     */
    m_BusNetCode = 0;               /* Used for BUS connections */
    m_Member     = 0;               /* for labels type NET_BUSLABELMEMBER ( bus member created
                                     * from the BUS label )  member number
                                     */
    m_ConnectionType = UNCONNECTED;
    m_netNameCandidate = NULL;      /* a pointer to a NETLIST_OBJECT type label connected to this
                                     * object used to give a name to the net
                                     */
}


// Copy constructor
NETLIST_OBJECT::NETLIST_OBJECT( NETLIST_OBJECT& aSource )
{
    *this = aSource;
}


NETLIST_OBJECT::~NETLIST_OBJECT()
{
}


// return true if the object is a label of any type
bool NETLIST_OBJECT::IsLabelType() const
{
    return m_Type == NET_LABEL
        || m_Type == NET_GLOBLABEL || m_Type == NET_HIERLABEL
        || m_Type == NET_BUSLABELMEMBER || m_Type == NET_GLOBBUSLABELMEMBER
        || m_Type == NET_HIERBUSLABELMEMBER
        || m_Type == NET_PINLABEL;
}

bool NETLIST_OBJECT::IsLabelConnected( NETLIST_OBJECT* aNetItem )
{
    if( aNetItem == this )   // Don't compare the same net list object.
        return false;

    int at = m_Type;
    int bt = aNetItem->m_Type;

    if(  ( at == NET_HIERLABEL || at == NET_HIERBUSLABELMEMBER )
      && ( bt == NET_SHEETLABEL || bt == NET_SHEETBUSLABELMEMBER ) )
    {
        if( m_SheetPath == aNetItem->m_SheetPathInclude )
        {
            return true; //connected!
        }
    }
    else if( ( at == NET_GLOBLABEL ) && ( bt == NET_GLOBLABEL ) )
    {
        if( m_Label == aNetItem->m_Label )
            return true; //connected!
    }

    return false; //these two are unconnected
}


void NETLIST_OBJECT::ParseBusVector( wxString vector, wxString* name, long* begin, long* end )
{
    wxCHECK_RET( IsBusLabel( vector ),
                 wxT( "<" ) + vector + wxT( "> is not a valid bus vector." ) );

    *name = busLabelRe.GetMatch( vector, 1 );
    wxString numberString = busLabelRe.GetMatch( vector, 2 );

    // numberString will include the brackets, e.g. [5..0] so skip the first one
    size_t i = 1, len = numberString.Len();
    wxString tmp;

    while( i < len && numberString[i] != '.' )
    {
        tmp.Append( numberString[i] );
        i++;
    }

    tmp.ToLong( begin );

    while( i < len && numberString[i] == '.' )
        i++;

    tmp.Empty();

    while( i < len && numberString[i] != ']' )
    {
        tmp.Append( numberString[i] );
        i++;
    }

    tmp.ToLong( end );

    if( *begin < 0 )
        *begin = 0;

    if( *end < 0 )
        *end = 0;

    if( *begin > *end )
        std::swap( *begin, *end );
}


bool NETLIST_OBJECT::ParseBusGroup( wxString aGroup, wxString* name,
                                    std::vector<wxString>& aMemberList )
{
    if( !IsHeteroBusLabel( aGroup ) )
    {
        return false;
    }

    *name = heteroBusLabelRe.GetMatch( aGroup, 1 );
    auto contents = heteroBusLabelRe.GetMatch( aGroup, 2 );

    wxStringTokenizer tokenizer( contents, " " );
    while( tokenizer.HasMoreTokens() )
    {
        aMemberList.push_back( tokenizer.GetNextToken() );
    }

    return true;
}


void NETLIST_OBJECT::ConvertBusToNetListItems( NETLIST_OBJECT_LIST& aNetListItems )
{
    wxCHECK_RET( IsBusLabel( m_Label ),
                 wxT( "<" ) + m_Label + wxT( "> is not a valid bus label." ) );

    if( m_Type == NET_HIERLABEL )
        m_Type = NET_HIERBUSLABELMEMBER;
    else if( m_Type == NET_GLOBLABEL )
        m_Type = NET_GLOBBUSLABELMEMBER;
    else if( m_Type == NET_SHEETLABEL )
        m_Type = NET_SHEETBUSLABELMEMBER;
    else if( m_Type == NET_LABEL )
        m_Type = NET_BUSLABELMEMBER;
    else
        wxCHECK_RET( false, wxT( "Net list object type is not valid." ) );

    if( IsHeteroBusLabel( m_Label ) )
    {
        // Hetero bus label: first group is the (optional) name,
        // second group is the contents of the group (space-delimited), e.g.
        // NET1 NET2 NETVECTOR[M..N] LASTNET

        wxString bus_name;
        bool selfSet = false;
        std::vector<wxString> bus_contents;

        if( ParseBusGroup( m_Label, &bus_name, bus_contents ) )
        {
            for( auto bus_member : bus_contents )
            {
                // Handle a nested vector bus inside the hetero bus
                if( IsBusLabel( bus_member ) )
                {
                    wxString vec_name, vec_member;
                    long begin = 0, end = 0, member = 0;

                    ParseBusVector( bus_member, &vec_name, &begin, &end );

                    if( !selfSet )
                    {
                        member = begin;
                        vec_member = vec_name;
                        vec_member << member;
                        m_Label = vec_member;
                        m_Member = member;

                        selfSet = true;
                    }

                    for( member++; member <= end; member++ )
                    {
                        auto item = new NETLIST_OBJECT( *this );

                        vec_member = vec_name;
                        vec_member << member;
                        item->m_Label = vec_member;
                        item->m_Member = member;

                        aNetListItems.push_back( item );
                    }
                }
                else
                {
                    if( !selfSet )
                    {
                        m_Label = bus_member;
                        selfSet = true;
                    }
                    else
                    {
                        auto item = new NETLIST_OBJECT( *this );
                        item->m_Label = bus_member;
                        aNetListItems.push_back( item );
                    }
                }
            }
        }
    }
    else
    {
        // Normal bus label

        wxString busName, tmp;
        long begin, end, member;

        ParseBusVector( m_Label, &busName, &begin, &end );

        member = begin;
        tmp = busName;
        tmp << member;
        m_Label = tmp;
        m_Member = member;

        for( member++; member <= end; member++ )
        {
            auto item = new NETLIST_OBJECT( *this );

            // Conversion of bus label to the root name + the current member id.
            tmp = busName;
            tmp << member;
            item->m_Label = tmp;
            item->m_Member = member;

            aNetListItems.push_back( item );
        }
    }
}


bool NETLIST_OBJECT::IsLabelGlobal() const
{
    // return true if the object is a global label
    // * a actual global label
    // * a pin label coming from a invisible power pin
    return ( m_Type == NET_PINLABEL ) ||
           ( m_Type == NET_GLOBLABEL ) ||
           ( m_Type == NET_GLOBBUSLABELMEMBER );
}


bool NETLIST_OBJECT::IsLabelBusMemberType() const
{
    // return true if the object is a bus label member build from a
    // schematic bus label (like label[xx..yy)
    // They are labels with very specific properties, especially for connection
    // between them: 2 bus label members can be connected only
    // if they have the same member value.
    return ( m_Type == NET_SHEETBUSLABELMEMBER ) ||
           ( m_Type == NET_BUSLABELMEMBER ) ||
           ( m_Type == NET_HIERBUSLABELMEMBER ) ||
           ( m_Type == NET_GLOBBUSLABELMEMBER );
}


/*
 * return the net name of the item
 */
wxString NETLIST_OBJECT::GetNetName( bool adoptTimestamp ) const
{
    if( m_netNameCandidate == NULL )
        return wxEmptyString;

    wxString netName;

    if( m_netNameCandidate->m_Type == NET_PIN )
        return GetShortNetName( adoptTimestamp );

    if( !m_netNameCandidate->IsLabelGlobal() )
    {
        // usual net name, prefix it by the sheet path
        netName = m_netNameCandidate->m_SheetPath.PathHumanReadable();
    }

    netName += m_netNameCandidate->m_Label;

    return netName;
}

/**
 * return the short net name of the item i.e. the net name
 * from the "best" label without any prefix.
 * 2 different nets can have the same short name
 */
wxString NETLIST_OBJECT::GetShortNetName( bool adoptTimestamp ) const
{
    if( m_netNameCandidate == NULL )
        return wxEmptyString;

    wxString netName;

    if( m_netNameCandidate->m_Type == NET_PIN )
    {
        SCH_COMPONENT* link = m_netNameCandidate->GetComponentParent();
        if( link )  // Should be always true
        {
            netName = wxT("Net-(");
            netName << link->GetRef( &m_netNameCandidate->m_SheetPath );

            if( adoptTimestamp && netName.Last() == '?' )
                netName << link->GetTimeStamp();

            netName << wxT("-Pad") << m_netNameCandidate->m_PinNum << wxT(")");
        }
    }
    else
        netName = m_netNameCandidate->m_Label;

    return netName;
}

/**
 * Set m_netNameCandidate to a connected item which will
 * be used to calcule the net name of the item
 * Obviously the candidate can be only a label
 * If there is no label on the net, a pad name will be
 * used to build a net name (something like Cmp<REF>_Pad<PAD_NAME>
 * @param aCandidate = the connected item candidate
 */
void NETLIST_OBJECT::SetNetNameCandidate( NETLIST_OBJECT* aCandidate )
{
    switch( aCandidate->m_Type )
    {
        case NET_HIERLABEL:
        case NET_LABEL:
        case NET_PINLABEL:
        case NET_GLOBLABEL:
        case NET_GLOBBUSLABELMEMBER:
        case NET_SHEETBUSLABELMEMBER:
        case NET_PIN:
            m_netNameCandidate = aCandidate;
            break;

        default:
            break;
    }
}
