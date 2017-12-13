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

#include <list>
#include <unordered_map>
#include <profile.h>

#include <sch_component.h>
#include <sch_pin_connection.h>
#include <sch_sheet.h>
#include <sch_text.h>

#include <connection_graph.h>

#ifdef USE_OPENMP
#include <omp.h>
#endif


void CONNECTION_GRAPH::UpdateItemConnectivity( std::vector<SCH_ITEM*> aItemList )
{
    PROF_COUNTER phase1;

    std::unordered_map< wxPoint, std::vector<SCH_ITEM*> > connection_map;

    for( auto item : aItemList )
    {
        std::vector< wxPoint > points;
        item->GetConnectionPoints( points );
        item->ConnectedItems().clear();

        if( item->Type() == SCH_SHEET_T )
        {
            for( auto& pin : static_cast<SCH_SHEET*>( item )->GetPins() )
            {
                if( !pin.Connection() )
                {
                    pin.InitializeConnection();
                }

                pin.ConnectedItems().clear();
                pin.Connection()->Reset();

                connection_map[ pin.GetTextPos() ].push_back( &pin );
                m_items.push_back( &pin );
            }
        }
        else if( item->Type() == SCH_COMPONENT_T )
        {
            auto component = static_cast<SCH_COMPONENT*>( item );

            component->UpdatePinConnections();

            for( auto pin_connection : component->m_pin_connections )
            {
                // TODO(JE) use cached location from m_Pins
                auto pin_pos = pin_connection->m_pin->GetPosition();
                auto pos = component->GetTransform().TransformCoordinate( pin_pos ) +
                           component->GetPosition();
                connection_map[ pos ].push_back( pin_connection );
                m_items.push_back( pin_connection );
            }
        }
        else
        {
            m_items.push_back( item );

            if( !item->Connection() )
            {
                item->InitializeConnection();
            }

            item->Connection()->Reset();

            // Set bus/net property here so that the propagation code uses it
            switch( item->Type() )
            {
            case SCH_LINE_T:
                item->Connection()->SetType( ( item->GetLayer() == LAYER_BUS ) ?
                                             CONNECTION_BUS : CONNECTION_NET );
                break;

            case SCH_BUS_BUS_ENTRY_T:
                item->Connection()->SetType( CONNECTION_BUS );
                break;

            case SCH_PIN_CONNECTION_T:
            case SCH_BUS_WIRE_ENTRY_T:
                item->Connection()->SetType( CONNECTION_NET );
                break;

            default:
                break;
            }

            for( auto point : points )
            {
                connection_map[ point ].push_back( item );
            }
        }
    }

    for( auto& it : connection_map )
    {
        auto connection_vec = it.second;
        SCH_ITEM* junction = nullptr;

        for( auto connected_item : connection_vec )
        {
            // Look for junctions.  For points that have a junction, we want all
            // items to connect to the junction but not to each other.

            if( connected_item->Type() == SCH_JUNCTION_T )
            {
                junction = connected_item;
            }

            for( auto test_item : connection_vec )
            {
                if( !junction && test_item->Type() == SCH_JUNCTION_T )
                {
                    junction = test_item;
                }

                if( connected_item != test_item &&
                    connected_item != junction &&
                    connected_item->ConnectionPropagatesTo( test_item ) &&
                    test_item->ConnectionPropagatesTo( connected_item ) )
                {
                    connected_item->ConnectedItems().insert( test_item );
                    test_item->ConnectedItems().insert( connected_item );
                }
            }
        }
    }

    phase1.Stop();
    std::cout << "Phase 1 " << phase1.msecs() << " ms" << std::endl;
}


void CONNECTION_GRAPH::BuildConnectionGraph()
{
    PROF_COUNTER phase2;

    long subgraph_code = 1;
    std::vector<CONNECTION_SUBGRAPH> subgraphs;

    for( auto item : m_items )
    {
        if( item->Connection()->SubgraphCode() == 0 )
        {
            CONNECTION_SUBGRAPH subgraph;

            subgraph.m_code = subgraph_code++;
            subgraph.m_items.push_back( item );

            // std::cout << "SG " << subgraph.m_code << " started with "
            //           << item->GetSelectMenuText() << std::endl;

            if( item->Connection()->IsDriver() )
                subgraph.m_drivers.push_back( item );

            item->Connection()->SetSubgraphCode( subgraph.m_code );

            std::list<SCH_ITEM*> members( item->ConnectedItems().begin(),
                                          item->ConnectedItems().end() );

            for( auto connected_item : members )
            {
                if( !connected_item->Connection() )
                    connected_item->InitializeConnection();

                if( connected_item->Connection()->SubgraphCode() == 0 )
                {
                    connected_item->Connection()->SetSubgraphCode( subgraph.m_code );
                    subgraph.m_items.push_back( connected_item );

                    // std::cout << "   +" << connected_item->GetSelectMenuText() << std::endl;

                    if( connected_item->Connection()->IsDriver() )
                        subgraph.m_drivers.push_back( connected_item );

                    members.insert( members.end(),
                                    connected_item->ConnectedItems().begin(),
                                    connected_item->ConnectedItems().end() );
                }
            }

            subgraphs.push_back( subgraph );
        }
    }

    phase2.Stop();
    std::cout << "Phase 2 " <<  phase2.msecs() << " ms" << std::endl;

    PROF_COUNTER phase3;

    #ifdef USE_OPENMP
        #pragma omp parallel
    #endif
    {
        #ifdef USE_OPENMP
            #pragma omp for schedule(dynamic)
        #endif
        for( auto it = subgraphs.begin(); it < subgraphs.end(); it++ )
        {
            auto subgraph = *it;

            if( !subgraph.ResolveDrivers() )
            {
                // TODO(JE) ERC Error: multiple equivalent drivers
            }
            else
            {
                // Now the subgraph has only one driver
                auto& connection = subgraph.m_driver->Connection();

                // TODO(JE) This should live in SCH_CONNECTION probably
                switch( subgraph.m_driver->Type() )
                {
                case SCH_LABEL_T:
                case SCH_GLOBAL_LABEL_T:
                case SCH_HIERARCHICAL_LABEL_T:
                case SCH_PIN_CONNECTION_T:
                case SCH_SHEET_PIN_T:
                case SCH_SHEET_T:
                {
                    if( subgraph.m_driver->Type() == SCH_PIN_CONNECTION_T )
                    {
                        auto pin = static_cast<SCH_PIN_CONNECTION*>( subgraph.m_driver );
                        connection->ConfigureFromLabel( pin->m_pin->GetName() );
                    }
                    else
                    {
                        auto text = static_cast<SCH_TEXT*>( subgraph.m_driver );
                        connection->ConfigureFromLabel( text->GetText() );
                    }

                    connection->SetDriver( subgraph.m_driver );
                    connection->ClearDirty();
                    break;
                }
                default:
                    break;
                }

                // std::cout << "Propagating SG " << subgraph.m_code << " driven by "
                //           << subgraph.m_driver->GetSelectMenuText() << " net "
                //           << subgraph.m_driver->Connection()->Name() << std::endl;

                for( auto item : subgraph.m_items )
                {
                    if( ( connection->IsBus() && item->Connection()->IsNet() ) ||
                        ( connection->IsNet() && item->Connection()->IsBus() ) )
                    {
                        continue;
                    }

                    if( item != subgraph.m_driver )
                    {
                        // std::cout << "   +" << item->GetSelectMenuText() << std::endl;
                        item->Connection()->Clone( *connection );
                        item->Connection()->ClearDirty();
                    }
                }
            }
        }
    }

    phase3.Stop();
    std::cout << "Phase 3 " <<  phase3.msecs() << " ms" << std::endl;
}


bool CONNECTION_SUBGRAPH::ResolveDrivers()
{
    SCH_ITEM* candidate = nullptr;
    int highest_priority = -1;
    int num_items = 0;

    m_driver = nullptr;

    for( auto item : m_drivers )
    {
        int item_priority = 0;

        switch( item->Type() )
        {
        case LIB_PIN_T:                 item_priority = 1; break;
        case SCH_LABEL_T:               item_priority = 2; break;
        case SCH_HIERARCHICAL_LABEL_T:  item_priority = 3; break;
        case SCH_SHEET_PIN_T:           item_priority = 4; break;
        case SCH_PIN_CONNECTION_T:
        {
            auto pin_connection = static_cast<SCH_PIN_CONNECTION*>( item );
            if( pin_connection->m_pin->IsPowerConnection() )
                item_priority = 5;
            else
                item_priority = 1;
            break;
        }
        case SCH_GLOBAL_LABEL_T:        item_priority = 6; break;
        default: break;
        }

        if( item_priority > highest_priority )
        {
            candidate = item;
            highest_priority = item_priority;
            num_items = 1;
        }
        else if( candidate && ( item_priority == highest_priority ) )
        {
            num_items++;
        }
    }

    if( num_items > 0 )
    {
        m_driver = candidate;

        if( num_items > 1 )
        {
            // TODO(JE) ERC warning about multiple drivers?
        }
    }
    else
    {
        std::cout << "Warning: could not resolve drivers for SG " << m_code << std::endl;
        for( auto item : m_items )
        {
            std::cout << "    " << item->GetSelectMenuText() << std::endl;
        }
    }

    return ( m_driver != nullptr );
}
