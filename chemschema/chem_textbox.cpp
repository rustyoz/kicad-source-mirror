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

#include "chem_textbox.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <pgm_base.h>
#include <settings/settings_manager.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>

CHEM_TEXTBOX::CHEM_TEXTBOX( const IU_PER_MILS& aIUScale ) :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_TEXTBOX_T ),
    EDA_TEXT( aIUScale )
{
    SetTextSize( VECTOR2I( DEFAULT_SIZE_TEXT, DEFAULT_SIZE_TEXT ) );
    SetVisible( true );
    SetTextThickness( 0 );  // Use default line thickness
    
    // Default box size
    m_boxSize = VECTOR2I( 500, 300 );  // Default size in mils
    m_lineWidth = 10;                  // Default line width in mils
    m_fillColor = COLOR4D::UNSPECIFIED;
    m_lineColor = COLOR4D::UNSPECIFIED;
    m_cornerRadius = 0;                // Default to sharp corners
}


CHEM_TEXTBOX::CHEM_TEXTBOX( const CHEM_TEXTBOX& aTextBox ) :
    CHEM_ITEM( aTextBox ),
    EDA_TEXT( aTextBox )
{
    m_boxSize = aTextBox.m_boxSize;
    m_lineWidth = aTextBox.m_lineWidth;
    m_fillColor = aTextBox.m_fillColor;
    m_lineColor = aTextBox.m_lineColor;
    m_cornerRadius = aTextBox.m_cornerRadius;
}


CHEM_TEXTBOX::~CHEM_TEXTBOX()
{
}


EDA_ITEM* CHEM_TEXTBOX::Clone() const
{
    return new CHEM_TEXTBOX( *this );
}


void CHEM_TEXTBOX::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_LABELS;
    aCount = 1;
}


void CHEM_TEXTBOX::SwapData( CHEM_ITEM* aItem )
{
    CHEM_TEXTBOX* item = static_cast<CHEM_TEXTBOX*>( aItem );

    SwapAttributes( *item );
    SwapText( *item );

    // Swap position and textbox-specific properties
    std::swap( m_pos, item->m_pos );
    std::swap( m_boxSize, item->m_boxSize );
    std::swap( m_lineWidth, item->m_lineWidth );
    std::swap( m_fillColor, item->m_fillColor );
    std::swap( m_lineColor, item->m_lineColor );
    std::swap( m_cornerRadius, item->m_cornerRadius );
}


void CHEM_TEXTBOX::SetBoxSize( const VECTOR2I& aSize )
{
    m_boxSize = aSize;
}


VECTOR2I CHEM_TEXTBOX::GetBoxSize() const
{
    return m_boxSize;
}


void CHEM_TEXTBOX::SetLineWidth( int aWidth )
{
    m_lineWidth = aWidth;
}


int CHEM_TEXTBOX::GetLineWidth() const
{
    return m_lineWidth;
}


void CHEM_TEXTBOX::SetFillColor( const COLOR4D& aColor )
{
    m_fillColor = aColor;
}


COLOR4D CHEM_TEXTBOX::GetFillColor() const
{
    return m_fillColor;
}


void CHEM_TEXTBOX::SetLineColor( const COLOR4D& aColor )
{
    m_lineColor = aColor;
}


COLOR4D CHEM_TEXTBOX::GetLineColor() const
{
    return m_lineColor;
}


void CHEM_TEXTBOX::SetCornerRadius( int aRadius )
{
    m_cornerRadius = aRadius;
}


int CHEM_TEXTBOX::GetCornerRadius() const
{
    return m_cornerRadius;
}


BOX2I CHEM_TEXTBOX::GetBoxBounds() const
{
    VECTOR2I pos = GetTextPos();
    return BOX2I( pos - VECTOR2I( m_boxSize.x / 2, m_boxSize.y / 2 ), 
                 VECTOR2I( m_boxSize.x, m_boxSize.y ) );
}


std::vector<BOX2I> CHEM_TEXTBOX::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    
    // Add the box bounds
    boxes.push_back( GetBoxBounds() );
    
    return boxes;
}


wxString CHEM_TEXTBOX::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    return wxString::Format( _( "Text Box '%s'" ), ShortenedText() );
}


BITMAPS CHEM_TEXTBOX::GetMenuImage() const
{
    return BITMAPS::text_box;
}


wxString CHEM_TEXTBOX::ShortenedText() const
{
    wxString tmp = GetText();
    
    // Truncate the text if it's too long
    if( tmp.Length() > 15 )
        tmp = tmp.Left( 12 ) + wxT( "..." );
        
    return tmp;
}


bool CHEM_TEXTBOX::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    // Check if the point is within the box
    if( GetBoxBounds().Contains( aPosition ) )
        return true;
        
    // If not inside the box, check if it's on the box outline
    BOX2I box = GetBoxBounds();
    int halfWidth = m_lineWidth / 2 + aAccuracy;
    
    // Expand the box by half the line width plus accuracy for the outer edge
    BOX2I outerBox = box;
    outerBox.Inflate( halfWidth );
    
    // Shrink the box by half the line width plus accuracy for the inner edge
    BOX2I innerBox = box;
    innerBox.Inflate( -halfWidth );
    
    // If the point is inside the outer box but outside the inner box, it's on the outline
    return outerBox.Contains( aPosition ) && !innerBox.Contains( aPosition );
}


bool CHEM_TEXTBOX::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    if( aContains )
    {
        // The selection rectangle must fully contain the textbox
        return aRect.Contains( GetBoxBounds() );
    }
    else
    {
        // The selection rectangle must intersect the textbox
        return aRect.Intersects( GetBoxBounds() );
    }
}


void CHEM_TEXTBOX::Plot( PLOTTER* aPlotter ) const
{
    // Implement plotting for the textbox
    // This is a placeholder for actual implementation
}


bool CHEM_TEXTBOX::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // If we have no text, we can never match
    if( GetText().IsEmpty() )
        return false;

    // If search includes text, inspect the text
    if( aSearchData.IsSearchAllFields() )
        return EDA_ITEM::Matches( GetText(), aSearchData );

    return false;
} 