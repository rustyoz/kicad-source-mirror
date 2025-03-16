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

#include "chem_sheet.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>

CHEM_SHEET::CHEM_SHEET() :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_SHEET_T )
{
    m_position = wxPoint( 0, 0 );
    m_size = wxSize( 2000, 1500 );  // Default size (width 2 inches, height 1.5 inches in mils)
    m_name = wxT( "New Sheet" );
    m_fileName = wxEmptyString;
    m_borderColor = COLOR4D::UNSPECIFIED;
    m_backgroundColor = COLOR4D::UNSPECIFIED;
    m_borderWidth = 10;  // Default width in mils
}


CHEM_SHEET::CHEM_SHEET( const CHEM_SHEET& aSheet ) :
    CHEM_ITEM( aSheet )
{
    m_position = aSheet.m_position;
    m_size = aSheet.m_size;
    m_name = aSheet.m_name;
    m_fileName = aSheet.m_fileName;
    m_borderColor = aSheet.m_borderColor;
    m_backgroundColor = aSheet.m_backgroundColor;
    m_borderWidth = aSheet.m_borderWidth;
}


CHEM_SHEET::~CHEM_SHEET()
{
}


EDA_ITEM* CHEM_SHEET::Clone() const
{
    return new CHEM_SHEET( *this );
}


void CHEM_SHEET::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_SYMBOLS;  // Sheets appear on the same layer as symbols
    aCount = 1;
}


void CHEM_SHEET::SwapData( CHEM_ITEM* aItem )
{
    CHEM_SHEET* item = static_cast<CHEM_SHEET*>( aItem );

    std::swap( m_position, item->m_position );
    std::swap( m_size, item->m_size );
    std::swap( m_name, item->m_name );
    std::swap( m_fileName, item->m_fileName );
    std::swap( m_borderColor, item->m_borderColor );
    std::swap( m_backgroundColor, item->m_backgroundColor );
    std::swap( m_borderWidth, item->m_borderWidth );
}


void CHEM_SHEET::SetPosition( const wxPoint& aPosition )
{
    m_position = aPosition;
}


wxPoint CHEM_SHEET::GetPosition() const
{
    return m_position;
}


void CHEM_SHEET::SetSize( const wxSize& aSize )
{
    m_size = aSize;
}


wxSize CHEM_SHEET::GetSize() const
{
    return m_size;
}


void CHEM_SHEET::SetName( const wxString& aName )
{
    m_name = aName;
}


wxString CHEM_SHEET::GetName() const
{
    return m_name;
}


void CHEM_SHEET::SetFileName( const wxString& aFileName )
{
    m_fileName = aFileName;
}


wxString CHEM_SHEET::GetFileName() const
{
    return m_fileName;
}


void CHEM_SHEET::SetBorderColor( const COLOR4D& aColor )
{
    m_borderColor = aColor;
}


COLOR4D CHEM_SHEET::GetBorderColor() const
{
    return m_borderColor;
}


void CHEM_SHEET::SetBackgroundColor( const COLOR4D& aColor )
{
    m_backgroundColor = aColor;
}


COLOR4D CHEM_SHEET::GetBackgroundColor() const
{
    return m_backgroundColor;
}


void CHEM_SHEET::SetBorderWidth( int aWidth )
{
    m_borderWidth = aWidth;
}


int CHEM_SHEET::GetBorderWidth() const
{
    return m_borderWidth;
}


BOX2I CHEM_SHEET::GetBoundingBox() const
{
    return BOX2I( VECTOR2I( m_position.x, m_position.y ), 
                 VECTOR2I( m_size.x, m_size.y ) );
}


std::vector<BOX2I> CHEM_SHEET::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    boxes.push_back( GetBoundingBox() );
    return boxes;
}


wxString CHEM_SHEET::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    return wxString::Format( _( "Sheet '%s'" ), m_name );
}


BITMAPS CHEM_SHEET::GetMenuImage() const
{
    return BITMAPS::sheet;
}


bool CHEM_SHEET::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    BOX2I box = GetBoundingBox();
    
    // Expand the box by half the border width plus accuracy for the outer edge
    int halfWidth = m_borderWidth / 2 + aAccuracy;
    BOX2I outerBox = box;
    outerBox.Inflate( halfWidth );
    
    // Shrink the box by half the border width plus accuracy for the inner edge
    BOX2I innerBox = box;
    innerBox.Inflate( -halfWidth );
    
    // Check if the point is inside the outer box but outside the inner box (on the border)
    bool onBorder = outerBox.Contains( aPosition ) && !innerBox.Contains( aPosition );
    
    // Or if the point is inside the box
    bool inside = box.Contains( aPosition );
    
    return onBorder || inside;
}


bool CHEM_SHEET::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    BOX2I box = GetBoundingBox();
    
    if( aContains )
    {
        // The selection rectangle must fully contain the sheet
        return aRect.Contains( box );
    }
    else
    {
        // The selection rectangle must intersect the sheet
        return aRect.Intersects( box );
    }
}


void CHEM_SHEET::Plot( PLOTTER* aPlotter ) const
{
    // Implement plotting for the sheet
    // This is a placeholder for actual implementation
}


bool CHEM_SHEET::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // Check name and filename
    if( EDA_ITEM::Matches( m_name, aSearchData ) )
        return true;
        
    if( EDA_ITEM::Matches( m_fileName, aSearchData ) )
        return true;
        
    return false;
} 