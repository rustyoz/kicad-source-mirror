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
#include <algorithm>

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


CHEM_SCHEMATIC::CHEM_SCHEMATIC() :
    m_modified( false )
{
    m_title = wxT( "New Chemical Process Flow Diagram" );
}


void CHEM_SCHEMATIC::AddSymbol( CHEM_SYMBOL* aSymbol )
{
    if( aSymbol )
    {
        m_symbols.push_back( aSymbol );
        m_modified = true;
    }
}


void CHEM_SCHEMATIC::RemoveSymbol( CHEM_SYMBOL* aSymbol )
{
    if( aSymbol )
    {
        auto it = std::find( m_symbols.begin(), m_symbols.end(), aSymbol );
        
        if( it != m_symbols.end() )
        {
            m_symbols.erase( it );
            m_modified = true;
        }
    }
}


const std::vector<CHEM_SYMBOL*>& CHEM_SCHEMATIC::GetSymbols() const
{
    return m_symbols;
}


void CHEM_SCHEMATIC::AddConnection( CHEM_CONNECTION* aConnection )
{
    if( aConnection )
    {
        m_connections.push_back( aConnection );
        m_modified = true;
    }
}


void CHEM_SCHEMATIC::RemoveConnection( CHEM_CONNECTION* aConnection )
{
    if( aConnection )
    {
        auto it = std::find( m_connections.begin(), m_connections.end(), aConnection );
        
        if( it != m_connections.end() )
        {
            m_connections.erase( it );
            m_modified = true;
        }
    }
}


const std::vector<CHEM_CONNECTION*>& CHEM_SCHEMATIC::GetConnections() const
{
    return m_connections;
}


void CHEM_SCHEMATIC::AddLabel( CHEM_LABEL* aLabel )
{
    if( aLabel )
    {
        m_labels.push_back( aLabel );
        m_modified = true;
    }
}


void CHEM_SCHEMATIC::RemoveLabel( CHEM_LABEL* aLabel )
{
    if( aLabel )
    {
        auto it = std::find( m_labels.begin(), m_labels.end(), aLabel );
        
        if( it != m_labels.end() )
        {
            m_labels.erase( it );
            m_modified = true;
        }
    }
}


const std::vector<CHEM_LABEL*>& CHEM_SCHEMATIC::GetLabels() const
{
    return m_labels;
}


void CHEM_SCHEMATIC::SetFilename( const wxFileName& aFilename )
{
    m_filename = aFilename;
}


const wxFileName& CHEM_SCHEMATIC::GetFilename() const
{
    return m_filename;
}


void CHEM_SCHEMATIC::SetTitle( const wxString& aTitle )
{
    m_title = aTitle;
    m_modified = true;
}


const wxString& CHEM_SCHEMATIC::GetTitle() const
{
    return m_title;
}


void CHEM_SCHEMATIC::SetModified( bool aModified )
{
    m_modified = aModified;
}


bool CHEM_SCHEMATIC::IsModified() const
{
    return m_modified;
} 