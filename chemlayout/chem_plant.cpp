/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2023 KiCad Developers, see AUTHORS.txt for contributors.
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

#include "chem_plant.h"
#include "chem_layout_item.h"
#include <wx/log.h>

CHEM_PLANT::CHEM_PLANT() : CHEM_LAYOUT_ITEM( nullptr )
{
    // Initialize default plant properties
    m_name = wxT( "New Plant" );
    m_description = wxT( "" );
    m_position = wxPoint( 0, 0 );
}


CHEM_PLANT::~CHEM_PLANT()
{
    // Clean up resources
    ClearPlantItems();
}


void CHEM_PLANT::SetName( const wxString& aName )
{
    m_name = aName;
}


wxString CHEM_PLANT::GetName() const
{
    return m_name;
}


void CHEM_PLANT::SetDescription( const wxString& aDescription )
{
    m_description = aDescription;
}


wxString CHEM_PLANT::GetDescription() const
{
    return m_description;
}


void CHEM_PLANT::SetPosition( const wxPoint& aPosition )
{
    m_position = aPosition;
}


wxPoint CHEM_PLANT::GetPosition() const
{
    return m_position;
}


void CHEM_PLANT::AddItem( CHEM_LAYOUT_ITEM* aItem )
{
    if( aItem )
    {
        // Check for duplicates
        for( auto item : m_items )
        {
            if( item == aItem )
                return;
        }
        
        m_items.push_back( aItem );
    }
}


void CHEM_PLANT::RemoveItem( CHEM_LAYOUT_ITEM* aItem )
{
    if( !aItem )
        return;
        
    auto it = std::find( m_items.begin(), m_items.end(), aItem );
    
    if( it != m_items.end() )
    {
        m_items.erase( it );
        delete aItem;
    }
}


void CHEM_PLANT::ClearPlantItems()
{
    for( auto item : m_items )
    {
        delete item;
    }
    
    m_items.clear();
}


size_t CHEM_PLANT::GetItemCount() const
{
    return m_items.size();
}


CHEM_LAYOUT_ITEM* CHEM_PLANT::GetItem( size_t aIndex )
{
    if( aIndex < m_items.size() )
        return m_items[aIndex];
        
    wxLogWarning( wxT( "Requested item %zu is out of range (0-%zu)" ), aIndex, m_items.size() - 1 );
    return nullptr;
}


bool CHEM_PLANT::Save( const wxString& aFileName )
{
    // Placeholder for actual file saving implementation
    // In the future, this would use JSON or XML format
    wxLogMessage( wxT( "Saving chemical plant to %s" ), aFileName );
    return true;
}


bool CHEM_PLANT::Load( const wxString& aFileName )
{
    // Clear the current layout
    ClearPlantItems();
    
    // Placeholder for actual file loading implementation
    // In the future, this would parse JSON or XML format
    wxLogMessage( wxT( "Loading chemical plant from %s" ), aFileName );
    return true;
} 