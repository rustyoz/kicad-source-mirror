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

#include "chem_sheet_path.h"
#include "chem_sheet.h"
#include "chem_screen.h"
#include <hash.h>

CHEM_SHEET_PATH::CHEM_SHEET_PATH()
{
    m_virtualPageNumber = 1;
    m_current_hash = 0;
}

CHEM_SHEET_PATH::CHEM_SHEET_PATH( const CHEM_SHEET_PATH& aOther )
{
    m_sheets = aOther.m_sheets;
    m_virtualPageNumber = aOther.m_virtualPageNumber;
    m_current_hash = aOther.m_current_hash;
    m_cached_page_number = aOther.m_cached_page_number;
}

CHEM_SHEET_PATH::~CHEM_SHEET_PATH()
{
}

CHEM_SHEET_PATH& CHEM_SHEET_PATH::operator=( const CHEM_SHEET_PATH& aOther )
{
    m_sheets = aOther.m_sheets;
    m_virtualPageNumber = aOther.m_virtualPageNumber;
    m_current_hash = aOther.m_current_hash;
    m_cached_page_number = aOther.m_cached_page_number;
    return *this;
}

CHEM_SHEET_PATH CHEM_SHEET_PATH::operator+( const CHEM_SHEET_PATH& aOther ) const
{
    CHEM_SHEET_PATH retv = *this;

    size_t size = aOther.size();

    for( size_t i = 0; i < size; i++ )
        retv.m_sheets.push_back( aOther.at( i ) );

    return retv;
}

bool CHEM_SHEET_PATH::operator==( const CHEM_SHEET_PATH& aOther ) const
{
    return m_current_hash == aOther.GetCurrentHash();
}

CHEM_SHEET* CHEM_SHEET_PATH::Last() const
{
    if( !empty() )
        return m_sheets.back();

    return nullptr;
}

CHEM_SCREEN* CHEM_SHEET_PATH::LastScreen() const
{
    CHEM_SHEET* lastSheet = Last();

    if( lastSheet )
        return lastSheet->GetScreen();

    return nullptr;
}

wxString CHEM_SHEET_PATH::PathAsString() const
{
    wxString s;

    s = wxT( "/" );     // This is the root path

    // Start at 1 to avoid the root sheet, which does not need to be added to the path.
    // Its timestamp changes anyway.
    for( unsigned i = 1; i < size(); i++ )
        s += at( i )->m_Uuid.AsString() + "/";

    return s;
}

KIID_PATH CHEM_SHEET_PATH::Path() const
{
    KIID_PATH path;
    path.reserve( m_sheets.size() );

    for( const CHEM_SHEET* sheet : m_sheets )
        path.push_back( sheet->m_Uuid );

    return path;
}

wxString CHEM_SHEET_PATH::PathHumanReadable( bool aUseShortRootName,
                                           bool aStripTrailingSeparator ) const
{
    wxString s;

    if( aUseShortRootName )
    {
        s = wxS( "/" ); // Use only the short name in netlists
    }
    else
    {
        wxString fileName;

        if( !empty() && at( 0 )->GetScreen() )
            fileName = at( 0 )->GetScreen()->GetFileName();

        wxFileName fn = fileName;
        s = fn.GetName() + wxS( "/" );
    }

    // Start at 1 since we've already processed the root sheet.
    for( unsigned i = 1; i < size(); i++ )
        s << at( i )->GetName() << wxS( "/" );

    if( aStripTrailingSeparator && s.EndsWith( "/" ) )
        s = s.Left( s.length() - 1 );

    return s;
}

EDA_ITEM* CHEM_SHEET_PATH::GetItem( const KIID& aID ) const
{
    for( EDA_ITEM* aItem : LastScreen()->Items() )
    {
        if( aItem->m_Uuid == aID )
            return aItem;

        EDA_ITEM* childMatch = nullptr;

        aItem->RunOnChildren(
                [&]( EDA_ITEM* aChild )
                {
                    if( aChild->m_Uuid == aID )
                        childMatch = aChild;
                } );

        if( childMatch )
            return childMatch;
    }

    return nullptr;
}

wxString CHEM_SHEET_PATH::GetPageNumber() const
{
    CHEM_SHEET* sheet = Last();

    wxCHECK( sheet, wxEmptyString );

    KIID_PATH tmpPath = Path();
    tmpPath.pop_back();

    return sheet->GetPageNumber( tmpPath );
}

void CHEM_SHEET_PATH::SetPageNumber( const wxString& aPageNumber )
{
    CHEM_SHEET* sheet = Last();

    wxCHECK( sheet, /* void */ );

    KIID_PATH tmpPath = Path();
    tmpPath.pop_back();

    sheet->AddInstance( tmpPath );
    sheet->SetPageNumber( tmpPath, aPageNumber );
}

CHEM_SHEET_LIST::CHEM_SHEET_LIST( CHEM_SHEET* aSheet )
{
    if( aSheet )
        BuildSheetList( aSheet, true );
}

bool CHEM_SHEET_LIST::IsModified() const
{
    for( const CHEM_SHEET_PATH& sheet : *this )
    {
        if( sheet.LastScreen() && sheet.LastScreen()->IsContentModified() )
            return true;
    }

    return false;
}

void CHEM_SHEET_LIST::ClearModifyStatus()
{
    for( CHEM_SHEET_PATH& sheet : *this )
    {
        if( sheet.LastScreen() )
            sheet.LastScreen()->SetContentModified( false );
    }
}

CHEM_ITEM* CHEM_SHEET_LIST::GetItem( const KIID& aID, CHEM_SHEET_PATH* aPathOut ) const
{
    for( const CHEM_SHEET_PATH& sheet : *this )
    {
        CHEM_SCREEN* screen = sheet.LastScreen();
        if( !screen )
            continue;

        for( CHEM_ITEM* item : screen->Items() )
        {
            if( item->m_Uuid == aID )
            {
                if( aPathOut )
                    *aPathOut = sheet;
                return item;
            }
        }
    }

    return nullptr;
}

void CHEM_SHEET_LIST::FillItemMap( std::map<KIID, EDA_ITEM*>& aMap )
{
    for( const CHEM_SHEET_PATH& sheet : *this )
    {
        CHEM_SCREEN* screen = sheet.LastScreen();
        if( !screen )
            continue;

        for( CHEM_ITEM* item : screen->Items() )
        {
            aMap[item->m_Uuid] = item;
        }
    }
}

void CHEM_SHEET_LIST::BuildSheetList( CHEM_SHEET* aSheet, bool aCheckIntegrity )
{
    wxCHECK_RET( aSheet != nullptr, wxT( "Cannot build sheet list from undefined sheet." ) );

    std::vector<CHEM_SHEET*> badSheets;

    m_currentSheetPath.push_back( aSheet );
    push_back( m_currentSheetPath );

    if( m_currentSheetPath.LastScreen() )
    {
        wxString parentFileName = aSheet->GetFileName();
        std::vector<CHEM_ITEM*> childSheets;
        m_currentSheetPath.LastScreen()->GetSheets( &childSheets );

        for( CHEM_ITEM* item : childSheets )
        {
            CHEM_SHEET* sheet = dynamic_cast<CHEM_SHEET*>( item );
            if( !sheet )
                continue;

            if( aCheckIntegrity )
            {
                if( !m_currentSheetPath.TestForRecursion( sheet->GetFileName(), parentFileName ) )
                    BuildSheetList( sheet, true );
                else
                    badSheets.push_back( sheet );
            }
            else
            {
                // If we are not performing a full recursion test, at least check if we are in
                // a simple recursion scenario to prevent stack overflow crashes
                wxCHECK2_MSG( sheet->GetFileName() != aSheet->GetFileName(), continue,
                              wxT( "Recursion prevented in CHEM_SHEET_LIST::BuildSheetList" ) );

                BuildSheetList( sheet, false );
            }
        }
    }

    if( aCheckIntegrity )
    {
        for( CHEM_SHEET* sheet : badSheets )
        {
            m_currentSheetPath.LastScreen()->Remove( sheet );
            m_currentSheetPath.LastScreen()->SetContentModified();
        }
    }

    m_currentSheetPath.pop_back();
}

CHEM_SHEET_PATH* CHEM_SHEET_LIST::FindSheetForPath( const CHEM_SHEET_PATH* aPath )
{
    for( CHEM_SHEET_PATH& path : *this )
    {
        if( path.Path() == aPath->Path() )
            return &path;
    }

    return nullptr;
}

CHEM_SHEET_PATH CHEM_SHEET_LIST::FindSheetForScreen( const CHEM_SCREEN* aScreen )
{
    for( CHEM_SHEET_PATH& sheetpath : *this )
    {
        if( sheetpath.LastScreen() == aScreen )
            return sheetpath;
    }

    return CHEM_SHEET_PATH();
}

CHEM_SHEET_LIST CHEM_SHEET_LIST::FindAllSheetsForScreen( const CHEM_SCREEN* aScreen ) const
{
    CHEM_SHEET_LIST retval;

    for( const CHEM_SHEET_PATH& sheetpath : *this )
    {
        if( sheetpath.LastScreen() == aScreen )
            retval.push_back( sheetpath );
    }

    return retval;
}

bool CHEM_SHEET_LIST::ContainsSheet( const CHEM_SHEET* aSheet ) const
{
    for( const CHEM_SHEET_PATH& path : *this )
    {
        for( size_t i = 0; i < path.size(); i++ )
        {
            if( path.at( i ) == aSheet )
                return true;
        }
    }

    return false;
} 