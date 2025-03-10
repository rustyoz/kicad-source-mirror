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

#include <wx/log.h>
#include <wx/file.h>
#include "chem_schematic.h"
#include "chem_edit_frame.h"

CHEM_SCHEMATIC::CHEM_SCHEMATIC( CHEM_EDIT_FRAME* aFrame ) : EDA_ITEM( nullptr )
{
    m_frame = aFrame;
}


CHEM_SCHEMATIC::CHEM_SCHEMATIC( const CHEM_SCHEMATIC& other ) : EDA_ITEM( nullptr )
{
    m_frame = other.m_frame;
    
    // Deep copy items
    for( EDA_ITEM* item : other.m_items )
    {
        if( item )
            m_items.push_back( item->Clone() );
    }
}


CHEM_SCHEMATIC::~CHEM_SCHEMATIC()
{
    Clear();
}


bool CHEM_SCHEMATIC::SaveFile( const wxString& aFileName )
{
    // Implement file saving - will need to develop a file format for chemical process flow diagrams
    wxLogMessage( wxT( "Saving chemical process flow diagram to %s" ), aFileName );
    return true;
}


bool CHEM_SCHEMATIC::LoadFile( const wxString& aFileName )
{
    // Implement file loading - will need to develop a file format for chemical process flow diagrams
    if( !wxFile::Exists( aFileName ) )
    {
        wxLogMessage( wxT( "File %s does not exist" ), aFileName );
        return false;
    }

    Clear();
    wxLogMessage( wxT( "Loading chemical process flow diagram from %s" ), aFileName );
    return true;
}


void CHEM_SCHEMATIC::Clear()
{
    // Delete all items
    for( EDA_ITEM* item : m_items )
        delete item;

    m_items.clear();
    UpdateView();
}


void CHEM_SCHEMATIC::AddItem( EDA_ITEM* aItem )
{
    if( aItem )
    {
        m_items.push_back( aItem );
        UpdateView();
    }
}


void CHEM_SCHEMATIC::RemoveItem( EDA_ITEM* aItem )
{
    if( !aItem )
        return;

    auto it = std::find( m_items.begin(), m_items.end(), aItem );
    
    if( it != m_items.end() )
    {
        m_items.erase( it );
        delete aItem;
        UpdateView();
    }
}


void CHEM_SCHEMATIC::UpdateView()
{
    // Will need to implement view update when rendering system is in place
} 