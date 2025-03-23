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
#include "chem_sheet.h"
#include "chem_screen.h"

CHEM_SCHEMATIC::CHEM_SCHEMATIC( PROJECT* aPrj ) :
    SCHEMATIC( aPrj ),
    m_rootSheet( nullptr ),
    m_currentSheet( nullptr ),
    m_modified( false )
{
    m_currentSheet = new CHEM_SHEET_PATH();
}





CHEM_SCHEMATIC::~CHEM_SCHEMATIC()
{
    delete m_currentSheet;
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

    //Clear();
    wxLogMessage( wxT( "Loading chemical process flow diagram from %s" ), aFileName );
    return true;
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

void CHEM_SCHEMATIC::Reset()
{
    if( m_rootSheet )
    {
        delete m_rootSheet;
        m_rootSheet = nullptr;
    }

   // m_currentSheet->m_sheets.clear();
}

void CHEM_SCHEMATIC::SetRoot( CHEM_SHEET* aRootSheet )
{
    wxCHECK_RET( aRootSheet, "Cannot set NULL root sheet" );

    m_rootSheet = aRootSheet;

    // Clear the current sheet path and add the root sheet
    m_currentSheet->clear();
    m_currentSheet->push_back( m_rootSheet );

    // Build the hierarchy and notify listeners
    RefreshHierarchy();
    OnChemSheetChanged();
}

CHEM_SCREEN* CHEM_SCHEMATIC::RootScreen() const
{
    return IsValid() ? m_rootSheet->Screen() : nullptr;
}

wxString CHEM_SCHEMATIC::GetFileName() const
{
    CHEM_SCREEN* screen = RootScreen();
    return screen ? screen->GetName() : wxString( wxEmptyString );
}

CHEM_SHEET_LIST CHEM_SCHEMATIC::Hierarchy() const
{
    CHEM_SHEET_LIST hierarchy( m_rootSheet );
    //hierarchy.SortByPageNumbers();
    return hierarchy;
}

void CHEM_SCHEMATIC::RefreshHierarchy()
{
    // Rebuild the hierarchy from the root sheet
    if( m_rootSheet )
    {
        CHEM_SHEET_LIST hierarchy( m_rootSheet );
        hierarchy.BuildSheetList( m_rootSheet, false );
    }
}

bool CHEM_SCHEMATIC::IsComplexHierarchy() const
{
    if( !m_rootSheet )
        return false;

    // Check if any screen is referenced by more than one sheet
    std::map<CHEM_SCREEN*, int> screenRefCount;

    CHEM_SHEET_LIST hierarchy( m_rootSheet );
    for( const CHEM_SHEET_PATH& path : hierarchy )
    {
        CHEM_SCREEN* screen = path.LastScreen();
        if( screen )
        {
            screenRefCount[screen]++;
            if( screenRefCount[screen] > 1 )
                return true;
        }
    }

    return false;
}

void CHEM_SCHEMATIC::AddChemListener( CHEM_SCHEMATIC_LISTENER* aListener )
{
    if( !alg::contains( m_chemListeners, aListener ) )
        m_chemListeners.push_back( aListener );
}


void CHEM_SCHEMATIC::RemoveChemListener( CHEM_SCHEMATIC_LISTENER* aListener )
{
    auto it = std::find( m_chemListeners.begin(), m_chemListeners.end(), aListener );

    if( it != m_chemListeners.end() )
    {
        std::iter_swap( it, m_chemListeners.end() - 1 );
        m_chemListeners.pop_back();
    }
}


void CHEM_SCHEMATIC::RemoveAllChemListeners()
{
    m_chemListeners.clear();
}


void CHEM_SCHEMATIC::OnChemItemsAdded( std::vector<CHEM_ITEM*>& aItems )
{
    InvokeChemListeners( &CHEM_SCHEMATIC_LISTENER::OnChemItemsAdded, *this, aItems );
}


void CHEM_SCHEMATIC::OnChemItemsRemoved( std::vector<CHEM_ITEM*>& aItems )
{
    InvokeChemListeners( &CHEM_SCHEMATIC_LISTENER::OnChemItemsRemoved, *this, aItems );
}


void CHEM_SCHEMATIC::OnChemItemsChanged( std::vector<CHEM_ITEM*>& aItems )
{
    InvokeChemListeners( &CHEM_SCHEMATIC_LISTENER::OnChemItemsChanged, *this, aItems );
}


void CHEM_SCHEMATIC::OnChemSheetChanged()
{
    InvokeChemListeners( &CHEM_SCHEMATIC_LISTENER::OnChemSheetChanged, *this );
} 