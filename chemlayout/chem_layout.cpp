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

#include <algorithm>
#include <wx/file.h>
#include <wx/log.h>

#include "chem_layout.h"
#include "chem_layout_item.h"

CHEM_LAYOUT::CHEM_LAYOUT() :
    m_modified( false )
{
    m_title = wxT( "New Chemical Reactor Layout" );
}


CHEM_LAYOUT::~CHEM_LAYOUT()
{
    Clear();
}


void CHEM_LAYOUT::AddItem( CHEM_LAYOUT_ITEM* aItem )
{
    // Check for duplicates
    if( std::find( m_items.begin(), m_items.end(), aItem ) != m_items.end() )
        return;
    
    m_items.push_back( aItem );
}


void CHEM_LAYOUT::RemoveItem( CHEM_LAYOUT_ITEM* aItem )
{
    auto it = std::find( m_items.begin(), m_items.end(), aItem );
    
    if( it != m_items.end() )
    {
        m_items.erase( it );
        delete aItem;
    }
}


void CHEM_LAYOUT::Clear()
{
    // Delete all reactors
    for( REACTOR* reactor : m_reactors )
    {
        delete reactor;
    }
    m_reactors.clear();
    
    // Delete all pipes
    for( PIPE* pipe : m_pipes )
    {
        delete pipe;
    }
    m_pipes.clear();
    
    // Delete all labels
    for( LAYOUT_LABEL* label : m_labels )
    {
        delete label;
    }
    m_labels.clear();
    
    m_modified = false;
}


bool CHEM_LAYOUT::Save( const wxString& aFileName )
{
    wxString fileName = aFileName;
    
    if( fileName.IsEmpty() )
        fileName = m_fileName;
    
    if( fileName.IsEmpty() )
    {
        wxLogError( wxT( "No filename specified for saving layout" ) );
        return false;
    }
    
    // Placeholder for actual file saving implementation
    // In the future, this would use JSON or XML format
    
    return true;
}


bool CHEM_LAYOUT::Load( const wxString& aFileName )
{
    // Clear the current layout
    Clear();
    
    // Set the new filename
    m_fileName = aFileName;
    
    if( !wxFile::Exists( aFileName ) )
    {
        wxLogError( wxT( "Layout file not found: %s" ), aFileName );
        return false;
    }
    
    // Placeholder for actual file loading implementation
    // In the future, this would parse JSON or XML format
    
    return true;
}


void CHEM_LAYOUT::AddReactor( REACTOR* aReactor )
{
    if( aReactor )
    {
        m_reactors.push_back( aReactor );
        m_modified = true;
    }
}


void CHEM_LAYOUT::RemoveReactor( REACTOR* aReactor )
{
    if( aReactor )
    {
        auto it = std::find( m_reactors.begin(), m_reactors.end(), aReactor );
        
        if( it != m_reactors.end() )
        {
            m_reactors.erase( it );
            m_modified = true;
        }
    }
}


const std::vector<REACTOR*>& CHEM_LAYOUT::GetReactors() const
{
    return m_reactors;
}


void CHEM_LAYOUT::AddPipe( PIPE* aPipe )
{
    if( aPipe )
    {
        m_pipes.push_back( aPipe );
        m_modified = true;
    }
}


void CHEM_LAYOUT::RemovePipe( PIPE* aPipe )
{
    if( aPipe )
    {
        auto it = std::find( m_pipes.begin(), m_pipes.end(), aPipe );
        
        if( it != m_pipes.end() )
        {
            m_pipes.erase( it );
            m_modified = true;
        }
    }
}


const std::vector<PIPE*>& CHEM_LAYOUT::GetPipes() const
{
    return m_pipes;
}


void CHEM_LAYOUT::AddLabel( LAYOUT_LABEL* aLabel )
{
    if( aLabel )
    {
        m_labels.push_back( aLabel );
        m_modified = true;
    }
}


void CHEM_LAYOUT::RemoveLabel( LAYOUT_LABEL* aLabel )
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


const std::vector<LAYOUT_LABEL*>& CHEM_LAYOUT::GetLabels() const
{
    return m_labels;
}


void CHEM_LAYOUT::SetFilename( const wxFileName& aFilename )
{
    m_filename = aFilename;
}


const wxFileName& CHEM_LAYOUT::GetFilename() const
{
    return m_filename;
}


void CHEM_LAYOUT::SetTitle( const wxString& aTitle )
{
    m_title = aTitle;
    m_modified = true;
}


const wxString& CHEM_LAYOUT::GetTitle() const
{
    return m_title;
}


void CHEM_LAYOUT::SetModified( bool aModified )
{
    m_modified = aModified;
}


bool CHEM_LAYOUT::IsModified() const
{
    return m_modified;
} 