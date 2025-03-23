/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2024 KiCad Developers.
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

#include "chem_collectors.h"
#include "chem_sheet.h"
#include "chem_line.h"
#include "chem_junction.h"
#include "chem_symbol.h"
#include "chem_text.h"

const COLLECTOR_FILTER CHEM_COLLECTOR::ChemicalItemsFilter =
    [ ]( const EDA_ITEM* aItem, void* aTestData )
    {
        if( !aItem )
            return false;

        switch( aItem->Type() )
        {
        case CHEM_ITEM_T:
        case CHEM_LINE_T:
        case CHEM_JUNCTION_T:
        case CHEM_SYMBOL_T:
        case CHEM_TEXT_T:
            return true;
        default:
            return false;
        }
    };

SEARCH_RESULT CHEM_COLLECTOR::Inspect( EDA_ITEM* aItem, void* aTestData )
{
    // Skip items that don't match our filter
    if( !m_Filter( aItem, aTestData ) )
        return SEARCH_CONTINUE;

    // Add matching items to our collection
    Append( aItem );
    return SEARCH_CONTINUE;
}

void CHEM_COLLECTOR::Collect( CHEM_SHEET* aSheet, const VECTOR2I& aPosition,
                            const COLLECTOR_FILTER& aFilter )
{
    Empty();        // empty the collection
    SetFilter( aFilter );

    // Set up search parameters
    EDA_RECT bbox;
    bbox.SetOrigin( aPosition );
    bbox.Inflate( Millimeter2iu( 0.2 ) );  // Small search area around the point

    // Search for items
    if( aSheet )
        aSheet->SearchAndQuery( bbox, m_List, this );
} 