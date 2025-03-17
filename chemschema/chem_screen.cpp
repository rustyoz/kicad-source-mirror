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

#include "chem_screen.h"
#include "chem_includes.h"
// Comment out the problematic include
// #include <plotter.h>
#include <trigo.h>
#include <eda_search.h>

CHEM_SCREEN::CHEM_SCREEN( const IU_PER_MILS& aIUScale ) :
    CHEM_ITEM( CHEM_ITEM::SCREEN )
{
    m_screenType = FILTER;
    m_position = VECTOR2I( 0, 0 );
    m_size = VECTOR2I( 100, 50 ) * aIUScale;
    m_description = wxEmptyString;
    m_name = wxEmptyString;
    m_rotationAngle = 0;
    m_outlineColor = COLOR4D::BLACK;
    m_fillColor = COLOR4D::WHITE;
    m_lineWidth = 10 * aIUScale;
    m_meshDensity = 5;
}

CHEM_SCREEN::CHEM_SCREEN( const CHEM_SCREEN& aScreen ) :
    CHEM_ITEM( aScreen )
{
    m_screenType = aScreen.m_screenType;
    m_position = aScreen.m_position;
    m_size = aScreen.m_size;
    m_description = aScreen.m_description;
    m_name = aScreen.m_name;
    m_rotationAngle = aScreen.m_rotationAngle;
    m_outlineColor = aScreen.m_outlineColor;
    m_fillColor = aScreen.m_fillColor;
    m_lineWidth = aScreen.m_lineWidth;
    m_meshDensity = aScreen.m_meshDensity;
}

CHEM_SCREEN::~CHEM_SCREEN()
{
}

EDA_ITEM* CHEM_SCREEN::Clone() const
{
    return new CHEM_SCREEN( *this );
}

void CHEM_SCREEN::ViewGetLayers( int aLayers[], int& aCount ) const
{
    // Screens are typically drawn on a specific layer, but can be adjusted
    // For now, we'll use a placeholder layer index
    aLayers[0] = 0;
    aCount = 1;
}

void CHEM_SCREEN::SwapData( CHEM_ITEM* aItem )
{
    CHEM_SCREEN* other = static_cast<CHEM_SCREEN*>( aItem );
    
    std::swap( m_screenType, other->m_screenType );
    std::swap( m_position, other->m_position );
    std::swap( m_size, other->m_size );
    std::swap( m_description, other->m_description );
    std::swap( m_name, other->m_name );
    std::swap( m_rotationAngle, other->m_rotationAngle );
    std::swap( m_outlineColor, other->m_outlineColor );
    std::swap( m_fillColor, other->m_fillColor );
    std::swap( m_lineWidth, other->m_lineWidth );
    std::swap( m_meshDensity, other->m_meshDensity );
    
    CHEM_ITEM::SwapData( aItem );
}

void CHEM_SCREEN::SetPosition( const VECTOR2I& aPosition )
{
    m_position = aPosition;
}

const VECTOR2I& CHEM_SCREEN::GetPosition() const
{
    return m_position;
}

void CHEM_SCREEN::SetSize( const VECTOR2I& aSize )
{
    m_size = aSize;
}

const VECTOR2I& CHEM_SCREEN::GetSize() const
{
    return m_size;
}

void CHEM_SCREEN::SetScreenType( SCREEN_TYPE aType )
{
    m_screenType = aType;
}

CHEM_SCREEN::SCREEN_TYPE CHEM_SCREEN::GetScreenType() const
{
    return m_screenType;
}

void CHEM_SCREEN::SetDescription( const wxString& aDescription )
{
    m_description = aDescription;
}

const wxString& CHEM_SCREEN::GetDescription() const
{
    return m_description;
}

void CHEM_SCREEN::SetName( const wxString& aName )
{
    m_name = aName;
}

const wxString& CHEM_SCREEN::GetName() const
{
    return m_name;
}

void CHEM_SCREEN::SetRotationAngle( int aAngle )
{
    // Normalize angle to 0-3600 range (0-360 degrees)
    m_rotationAngle = NormalizeAngle360( aAngle );
}

int CHEM_SCREEN::GetRotationAngle() const
{
    return m_rotationAngle;
}

void CHEM_SCREEN::SetOutlineColor( const COLOR4D& aColor )
{
    m_outlineColor = aColor;
}

COLOR4D CHEM_SCREEN::GetOutlineColor() const
{
    return m_outlineColor;
}

void CHEM_SCREEN::SetFillColor( const COLOR4D& aColor )
{
    m_fillColor = aColor;
}

COLOR4D CHEM_SCREEN::GetFillColor() const
{
    return m_fillColor;
}

void CHEM_SCREEN::SetLineWidth( int aWidth )
{
    m_lineWidth = aWidth;
}

int CHEM_SCREEN::GetLineWidth() const
{
    return m_lineWidth;
}

void CHEM_SCREEN::SetMeshDensity( int aDensity )
{
    // Clamp to 1-10 range
    m_meshDensity = std::max( 1, std::min( 10, aDensity ) );
}

int CHEM_SCREEN::GetMeshDensity() const
{
    return m_meshDensity;
}

BOX2I CHEM_SCREEN::GetBoundingBox() const
{
    BOX2I bbox;
    
    if( m_rotationAngle == 0 )
    {
        // No rotation, simple calculation
        bbox = BOX2I( m_position - m_size / 2, m_size );
    }
    else
    {
        // With rotation, calculate the bounding box of the rotated rectangle
        VECTOR2I halfSize = m_size / 2;
        
        // Calculate the four corners before rotation
        VECTOR2I corners[4] = {
            VECTOR2I( -halfSize.x, -halfSize.y ),
            VECTOR2I( halfSize.x, -halfSize.y ),
            VECTOR2I( halfSize.x, halfSize.y ),
            VECTOR2I( -halfSize.x, halfSize.y )
        };
        
        // Rotate each corner and find the bounding box
        double angleRad = DECIDEG2RAD( m_rotationAngle );
        double sinA = sin( angleRad );
        double cosA = cos( angleRad );
        
        // Initialize with a first corner
        VECTOR2I rotated = VECTOR2I(
            KiROUND( corners[0].x * cosA - corners[0].y * sinA ),
            KiROUND( corners[0].x * sinA + corners[0].y * cosA )
        );
        
        VECTOR2I topLeft = m_position + rotated;
        VECTOR2I bottomRight = topLeft;
        
        // Process remaining corners
        for( int i = 1; i < 4; i++ )
        {
            rotated = VECTOR2I(
                KiROUND( corners[i].x * cosA - corners[i].y * sinA ),
                KiROUND( corners[i].x * sinA + corners[i].y * cosA )
            );
            
            VECTOR2I pos = m_position + rotated;
            
            topLeft.x = std::min( topLeft.x, pos.x );
            topLeft.y = std::min( topLeft.y, pos.y );
            bottomRight.x = std::max( bottomRight.x, pos.x );
            bottomRight.y = std::max( bottomRight.y, pos.y );
        }
        
        bbox = BOX2I( topLeft, bottomRight - topLeft );
    }
    
    return bbox;
}

std::vector<BOX2I> CHEM_SCREEN::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    boxes.push_back( GetBoundingBox() );
    return boxes;
}

wxString CHEM_SCREEN::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    wxString menuText;
    
    switch( m_screenType )
    {
    case FILTER:
        menuText = _( "Filter" );
        break;
    case MEMBRANE:
        menuText = _( "Membrane Filter" );
        break;
    case SIEVE:
        menuText = _( "Molecular Sieve" );
        break;
    case ION_EXCHANGE:
        menuText = _( "Ion Exchange Filter" );
        break;
    case CARBON:
        menuText = _( "Carbon Filter" );
        break;
    case SEPARATOR:
        menuText = _( "Phase Separator" );
        break;
    case CUSTOM:
        menuText = _( "Custom Filter" );
        break;
    default:
        menuText = _( "Screen" );
        break;
    }
    
    if( !m_name.IsEmpty() )
        menuText += wxString::Format( " '%s'", m_name );
    
    return menuText;
}

BITMAPS CHEM_SCREEN::GetMenuImage() const
{
    return BITMAPS::filter;  // Assuming filter bitmap is available
}

bool CHEM_SCREEN::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    // Simple hit test using the bounding box
    BOX2I bbox = GetBoundingBox();
    
    // Add accuracy margin
    bbox.Inflate( aAccuracy );
    
    return bbox.Contains( aPosition );
}

bool CHEM_SCREEN::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    BOX2I bbox = GetBoundingBox();
    
    // Add accuracy margin
    bbox.Inflate( aAccuracy );
    
    if( aContains )
        return aRect.Contains( bbox );
    else
        return bbox.Intersects( aRect );
}

void CHEM_SCREEN::Plot( PLOTTER* aPlotter ) const
{
    // Set up the plotter
    aPlotter->SetColor( m_outlineColor );
    aPlotter->SetCurrentLineWidth( m_lineWidth );
    
    BOX2I bbox = GetBoundingBox();
    
    // Draw the outline first
    if( m_rotationAngle == 0 )
    {
        // Simple rectangle for non-rotated screen
        aPlotter->Rect( bbox.GetPosition(), bbox.GetPosition() + bbox.GetSize(), FILL_TYPE::NO_FILL );
        
        // Draw the mesh pattern based on density
        int spacing = bbox.GetWidth() / (m_meshDensity + 1);
        
        // Draw vertical lines for mesh
        for( int i = 1; i <= m_meshDensity; i++ )
        {
            int x = bbox.GetLeft() + i * spacing;
            aPlotter->MoveTo( VECTOR2I( x, bbox.GetTop() ) );
            aPlotter->LineTo( VECTOR2I( x, bbox.GetBottom() ) );
        }
    }
    else
    {
        // For rotated screens, we need to calculate the actual corners
        VECTOR2I halfSize = m_size / 2;
        
        // Calculate the four corners before rotation
        VECTOR2I corners[4] = {
            VECTOR2I( -halfSize.x, -halfSize.y ),
            VECTOR2I( halfSize.x, -halfSize.y ),
            VECTOR2I( halfSize.x, halfSize.y ),
            VECTOR2I( -halfSize.x, halfSize.y )
        };
        
        // Rotate each corner and draw the outline
        double angleRad = DECIDEG2RAD( m_rotationAngle );
        double sinA = sin( angleRad );
        double cosA = cos( angleRad );
        
        VECTOR2I rotatedCorners[4];
        
        for( int i = 0; i < 4; i++ )
        {
            rotatedCorners[i] = VECTOR2I(
                KiROUND( corners[i].x * cosA - corners[i].y * sinA ),
                KiROUND( corners[i].x * sinA + corners[i].y * cosA )
            );
            
            rotatedCorners[i] += m_position;
        }
        
        // Draw the outline
        aPlotter->MoveTo( rotatedCorners[0] );
        for( int i = 1; i < 4; i++ )
            aPlotter->LineTo( rotatedCorners[i] );
        aPlotter->LineTo( rotatedCorners[0] );  // Close the shape
        
        // For rotated screens, draw simplified mesh pattern
        // We'll draw diagonal lines to represent the mesh
        int numLines = m_meshDensity * 2;
        double diagonalLength = std::sqrt( m_size.x * m_size.x + m_size.y * m_size.y );
        double spacing = diagonalLength / (numLines + 1);
        
        // Starting position for the mesh lines
        VECTOR2I centerOffset = VECTOR2I(
            KiROUND( -diagonalLength/2 * cosA - 0 * sinA ),
            KiROUND( -diagonalLength/2 * sinA + 0 * cosA )
        );
        
        VECTOR2I lineOffset = VECTOR2I(
            KiROUND( spacing * cosA ),
            KiROUND( spacing * sinA )
        );
        
        // Draw diagonal mesh lines
        for( int i = 1; i <= numLines; i++ )
        {
            VECTOR2I start = m_position + centerOffset + lineOffset * i;
            VECTOR2I perpDir = VECTOR2I( -lineOffset.y, lineOffset.x );
            
            aPlotter->MoveTo( start - perpDir );
            aPlotter->LineTo( start + perpDir );
        }
    }
}

bool CHEM_SCREEN::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // Search in screen name and description
    if( SCH_FIND_COLLECTOR_DATA* collector_data = static_cast<SCH_FIND_COLLECTOR_DATA*>( aAuxData ) )
    {
        if( aSearchData.IsTextUnescaped() )
        {
            if( collector_data->matchName && EDA_PATTERN_MATCH_SEARCH::Find( m_name, aSearchData.GetText() ) )
                return true;

            if( collector_data->matchDescription && EDA_PATTERN_MATCH_SEARCH::Find( m_description, aSearchData.GetText() ) )
                return true;
        }
        else
        {
            if( collector_data->matchName && EDA_PATTERN_MATCH_SEARCH::Find( m_name, aSearchData.GetText() ) )
                return true;

            if( collector_data->matchDescription && EDA_PATTERN_MATCH_SEARCH::Find( m_description, aSearchData.GetText() ) )
                return true;
        }
    }
    
    return false;
} 