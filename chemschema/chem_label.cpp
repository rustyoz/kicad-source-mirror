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

#include "chem_label.h"
#include "chem_includes.h"
// Comment out the problematic include
// #include <plotter.h>
#include <trigo.h>
#include <eda_search.h>

CHEM_LABEL::CHEM_LABEL( const IU_PER_MILS& aIUScale ) :
    CHEM_ITEM( CHEM_ITEM::LABEL ),
    EDA_TEXT( wxEmptyString )
{
    m_labelType = INLINE;
    m_hasBox = false;
    m_boxLineWidth = 0;
    m_boxLineColor = COLOR4D::UNSPECIFIED;
    m_boxFillColor = COLOR4D::UNSPECIFIED;
    
    SetTextSize( VECTOR2I( 50, 50 ) * aIUScale );
    SetTextThickness( 10 * aIUScale );
    SetItalic( false );
    SetBold( false );
    SetVisible( true );
    SetMirrored( false );
    SetHorizJustify( GR_TEXT_H_ALIGN_LEFT );
    SetVertJustify( GR_TEXT_V_ALIGN_CENTER );
}

CHEM_LABEL::CHEM_LABEL( const CHEM_LABEL& aLabel ) :
    CHEM_ITEM( aLabel ),
    EDA_TEXT( aLabel )
{
    m_labelType = aLabel.m_labelType;
    m_hasBox = aLabel.m_hasBox;
    m_boxLineWidth = aLabel.m_boxLineWidth;
    m_boxLineColor = aLabel.m_boxLineColor;
    m_boxFillColor = aLabel.m_boxFillColor;
}

CHEM_LABEL::~CHEM_LABEL()
{
}

EDA_ITEM* CHEM_LABEL::Clone() const
{
    return new CHEM_LABEL( *this );
}

void CHEM_LABEL::ViewGetLayers( int aLayers[], int& aCount ) const
{
    // Labels are typically drawn on a specific layer, but can be adjusted
    // For now, we'll use a placeholder layer index
    aLayers[0] = 0;
    aCount = 1;
}

void CHEM_LABEL::SwapData( CHEM_ITEM* aItem )
{
    CHEM_LABEL* other = static_cast<CHEM_LABEL*>( aItem );
    
    std::swap( m_labelType, other->m_labelType );
    std::swap( m_hasBox, other->m_hasBox );
    std::swap( m_boxLineWidth, other->m_boxLineWidth );
    std::swap( m_boxLineColor, other->m_boxLineColor );
    std::swap( m_boxFillColor, other->m_boxFillColor );
    
    EDA_TEXT::SwapText( *other );
    CHEM_ITEM::SwapData( aItem );
}

void CHEM_LABEL::SetLabelType( LABEL_TYPE aType )
{
    m_labelType = aType;
}

CHEM_LABEL::LABEL_TYPE CHEM_LABEL::GetLabelType() const
{
    return m_labelType;
}

void CHEM_LABEL::SetHasBox( bool aHasBox )
{
    m_hasBox = aHasBox;
}

bool CHEM_LABEL::HasBox() const
{
    return m_hasBox;
}

void CHEM_LABEL::SetBoxLineWidth( int aWidth )
{
    m_boxLineWidth = aWidth;
}

int CHEM_LABEL::GetBoxLineWidth() const
{
    return m_boxLineWidth;
}

void CHEM_LABEL::SetBoxLineColor( const COLOR4D& aColor )
{
    m_boxLineColor = aColor;
}

COLOR4D CHEM_LABEL::GetBoxLineColor() const
{
    return m_boxLineColor;
}

void CHEM_LABEL::SetBoxFillColor( const COLOR4D& aColor )
{
    m_boxFillColor = aColor;
}

COLOR4D CHEM_LABEL::GetBoxFillColor() const
{
    return m_boxFillColor;
}

BOX2I CHEM_LABEL::GetBoxBounds() const
{
    BOX2I textBounds = GetTextBox();
    
    // Add some padding around the text
    VECTOR2I padding( m_boxLineWidth + 10, m_boxLineWidth + 5 );
    
    textBounds.Inflate( padding );
    
    return textBounds;
}

std::vector<BOX2I> CHEM_LABEL::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    
    if( m_hasBox )
    {
        boxes.push_back( GetBoxBounds() );
    }
    else
    {
        boxes.push_back( GetTextBox() );
    }
    
    return boxes;
}

wxString CHEM_LABEL::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    wxString menuText;
    wxString shortText = ShortenedText();
    
    switch( m_labelType )
    {
    case INLINE:
        menuText = wxString::Format( _( "Inline Label '%s'" ), shortText );
        break;
    case FREESTANDING:
        menuText = wxString::Format( _( "Freestanding Label '%s'" ), shortText );
        break;
    case FLOW:
        menuText = wxString::Format( _( "Flow Label '%s'" ), shortText );
        break;
    case TEMPERATURE:
        menuText = wxString::Format( _( "Temperature Label '%s'" ), shortText );
        break;
    case PRESSURE:
        menuText = wxString::Format( _( "Pressure Label '%s'" ), shortText );
        break;
    case CHEMICAL:
        menuText = wxString::Format( _( "Chemical Label '%s'" ), shortText );
        break;
    case CUSTOM:
        menuText = wxString::Format( _( "Custom Label '%s'" ), shortText );
        break;
    default:
        menuText = wxString::Format( _( "Label '%s'" ), shortText );
        break;
    }
    
    return menuText;
}

BITMAPS CHEM_LABEL::GetMenuImage() const
{
    return BITMAPS::text;  // Assuming text bitmap is available
}

wxString CHEM_LABEL::ShortenedText() const
{
    wxString text = GetText();
    
    // Shorten text to 15 characters if it's longer
    if( text.Length() > 15 )
        return text.Left( 12 ) + wxT( "..." );
    
    return text;
}

bool CHEM_LABEL::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    if( m_hasBox )
    {
        // For boxed labels, check if the position is within the box bounds
        return GetBoxBounds().Contains( aPosition );
    }
    else
    {
        // For regular labels, check if the position is within the text bounds
        return EDA_TEXT::TextHitTest( aPosition, aAccuracy );
    }
}

bool CHEM_LABEL::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    if( m_hasBox )
    {
        BOX2I boxBounds = GetBoxBounds();
        
        if( aContains )
            return aRect.Contains( boxBounds );
        else
            return boxBounds.Intersects( aRect );
    }
    else
    {
        BOX2I textBox = GetTextBox();
        
        if( aContains )
            return aRect.Contains( textBox );
        else
            return textBox.Intersects( aRect );
    }
}

void CHEM_LABEL::Plot( PLOTTER* aPlotter ) const
{
    if( !IsVisible() )
        return;

    // Plot the box if it exists
    if( m_hasBox )
    {
        BOX2I boxBounds = GetBoxBounds();
        
        COLOR4D lineColor = m_boxLineColor;
        if( lineColor == COLOR4D::UNSPECIFIED )
            lineColor = COLOR4D::BLACK;
        
        aPlotter->SetColor( lineColor );
        aPlotter->SetCurrentLineWidth( m_boxLineWidth );
        
        // Plot the box outline
        aPlotter->MoveTo( boxBounds.GetPosition() );
        aPlotter->LineTo( VECTOR2I( boxBounds.GetRight(), boxBounds.GetTop() ) );
        aPlotter->LineTo( VECTOR2I( boxBounds.GetRight(), boxBounds.GetBottom() ) );
        aPlotter->LineTo( VECTOR2I( boxBounds.GetLeft(), boxBounds.GetBottom() ) );
        aPlotter->LineTo( boxBounds.GetPosition() );
        
        // Fill the box if a fill color is specified
        if( m_boxFillColor != COLOR4D::UNSPECIFIED )
        {
            aPlotter->SetColor( m_boxFillColor );
            aPlotter->Rectangle( boxBounds.GetPosition(), 
                                VECTOR2I( boxBounds.GetRight(), boxBounds.GetBottom() ), 
                                FILL_TYPE::FILLED_SHAPE );
        }
    }
    
    // Plot the text
    COLOR4D textColor = GetTextColor();
    if( textColor == COLOR4D::UNSPECIFIED )
        textColor = COLOR4D::BLACK;
    
    aPlotter->SetColor( textColor );
    
    // Call the EDA_TEXT plotting function
    EDA_TEXT::Plot( aPlotter );
}

bool CHEM_LABEL::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // Search in text
    if( EDA_TEXT::Matches( aSearchData, aAuxData ) )
        return true;
    
    // Add any specific label properties to search if needed
    
    return false;
} 