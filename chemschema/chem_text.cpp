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

#include "chem_text.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <pgm_base.h>
#include <settings/settings_manager.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>

CHEM_TEXT::CHEM_TEXT( const IU_PER_MILS& aIUScale ) :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_TEXT_T ),
    EDA_TEXT( aIUScale )
{
    SetTextSize( VECTOR2I( DEFAULT_SIZE_TEXT, DEFAULT_SIZE_TEXT ) );
    SetVisible( true );
    SetTextThickness( 0 );  // Use default line thickness
}


CHEM_TEXT::CHEM_TEXT( const CHEM_TEXT& aText ) :
    CHEM_ITEM( aText ),
    EDA_TEXT( aText )
{
}


CHEM_TEXT::~CHEM_TEXT()
{
}


EDA_ITEM* CHEM_TEXT::Clone() const
{
    return new CHEM_TEXT( *this );
}


void CHEM_TEXT::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_LABELS;
    aCount = 1;
}


void CHEM_TEXT::SwapData( CHEM_ITEM* aItem )
{
    CHEM_TEXT* item = static_cast<CHEM_TEXT*>( aItem );

    SwapAttributes( *item );
    SwapText( *item );

    // Swap position
    std::swap( m_pos, item->m_pos );
}


std::vector<BOX2I> CHEM_TEXT::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    
    boxes.push_back( GetTextBox() );
    
    return boxes;
}


wxString CHEM_TEXT::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    return wxString::Format( _( "Text '%s'" ), ShortenedText() );
}


BITMAPS CHEM_TEXT::GetMenuImage() const
{
    return BITMAPS::text;
}


wxString CHEM_TEXT::ShortenedText() const
{
    wxString tmp = GetText();
    
    // Truncate the text if it's too long
    if( tmp.Length() > 15 )
        tmp = tmp.Left( 12 ) + wxT( "..." );
        
    return tmp;
}


bool CHEM_TEXT::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    return TextHitTest( aPosition, aAccuracy );
}


bool CHEM_TEXT::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    return TextHitTest( aRect, aContains, aAccuracy );
}


void CHEM_TEXT::Plot( PLOTTER* aPlotter ) const
{
    // Implement plotting for the text
    // This is a placeholder for actual implementation
}


bool CHEM_TEXT::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // If we have no text, we can never match
    if( GetText().IsEmpty() )
        return false;

    // If search includes text, inspect the text
    if( aSearchData.IsSearchAllFields() )
        return EDA_ITEM::Matches( GetText(), aSearchData );

    return false;
} 