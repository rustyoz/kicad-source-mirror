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

#include "chem_line.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>
#include <geometry/seg.h>

CHEM_LINE::CHEM_LINE() :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_LINE_T )
{
    m_width = 10;  // Default width in mils
    m_lineStyle = SOLID;
    m_color = COLOR4D::UNSPECIFIED;
}


CHEM_LINE::CHEM_LINE( const CHEM_LINE& aLine ) :
    CHEM_ITEM( aLine )
{
    m_points = aLine.m_points;
    m_width = aLine.m_width;
    m_lineStyle = aLine.m_lineStyle;
    m_color = aLine.m_color;
}


CHEM_LINE::~CHEM_LINE()
{
}


EDA_ITEM* CHEM_LINE::Clone() const
{
    return new CHEM_LINE( *this );
}


void CHEM_LINE::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_CONNECTIONS;
    aCount = 1;
}


void CHEM_LINE::SwapData( CHEM_ITEM* aItem )
{
    CHEM_LINE* item = static_cast<CHEM_LINE*>( aItem );

    std::swap( m_points, item->m_points );
    std::swap( m_width, item->m_width );
    std::swap( m_lineStyle, item->m_lineStyle );
    std::swap( m_color, item->m_color );
}


void CHEM_LINE::SetPoints( const std::vector<wxPoint>& aPoints )
{
    m_points = aPoints;
}


const std::vector<wxPoint>& CHEM_LINE::GetPoints() const
{
    return m_points;
}


void CHEM_LINE::AddPoint( const wxPoint& aPoint )
{
    m_points.push_back( aPoint );
}


void CHEM_LINE::SetWidth( int aWidth )
{
    m_width = aWidth;
}


int CHEM_LINE::GetWidth() const
{
    return m_width;
}


void CHEM_LINE::SetLineStyle( LINE_STYLE aStyle )
{
    m_lineStyle = aStyle;
}


CHEM_LINE::LINE_STYLE CHEM_LINE::GetLineStyle() const
{
    return m_lineStyle;
}


void CHEM_LINE::SetColor( const COLOR4D& aColor )
{
    m_color = aColor;
}


COLOR4D CHEM_LINE::GetColor() const
{
    return m_color;
}


std::vector<BOX2I> CHEM_LINE::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    
    if( m_points.empty() )
        return boxes;
    
    // Find the bounding box of all points
    int minX = m_points[0].x;
    int minY = m_points[0].y;
    int maxX = m_points[0].x;
    int maxY = m_points[0].y;
    
    for( const wxPoint& pt : m_points )
    {
        minX = std::min( minX, pt.x );
        minY = std::min( minY, pt.y );
        maxX = std::max( maxX, pt.x );
        maxY = std::max( maxY, pt.y );
    }
    
    // Add half the line width to the bounding box
    int halfWidth = m_width / 2;
    BOX2I box( VECTOR2I( minX - halfWidth, minY - halfWidth ),
              VECTOR2I( maxX - minX + m_width, maxY - minY + m_width ) );
    
    boxes.push_back( box );
    
    return boxes;
}


wxString CHEM_LINE::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    return wxString::Format( _( "Line with %zu segments" ), m_points.size() > 0 ? m_points.size() - 1 : 0 );
}


BITMAPS CHEM_LINE::GetMenuImage() const
{
    return BITMAPS::add_line;
}


bool CHEM_LINE::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    if( m_points.size() < 2 )
        return false;
        
    // Check if the point is near any segment of the line
    for( size_t i = 0; i < m_points.size() - 1; i++ )
    {
        SEG segment( VECTOR2I( m_points[i].x, m_points[i].y ),
                    VECTOR2I( m_points[i+1].x, m_points[i+1].y ) );
        
        int distance = segment.Distance( aPosition );
        
        if( distance <= m_width / 2 + aAccuracy )
            return true;
    }
    
    return false;
}


bool CHEM_LINE::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    if( m_points.size() < 2 )
        return false;
        
    if( aContains )
    {
        // The selection rectangle must fully contain all line points
        for( const wxPoint& pt : m_points )
        {
            if( !aRect.Contains( VECTOR2I( pt.x, pt.y ) ) )
                return false;
        }
        return true;
    }
    else
    {
        // The selection rectangle must intersect at least one segment of the line
        for( size_t i = 0; i < m_points.size() - 1; i++ )
        {
            SEG segment( VECTOR2I( m_points[i].x, m_points[i].y ),
                        VECTOR2I( m_points[i+1].x, m_points[i+1].y ) );
                        
            if( segment.Intersects( aRect ) )
                return true;
        }
        return false;
    }
}


void CHEM_LINE::Plot( PLOTTER* aPlotter ) const
{
    // Implement plotting for the line
    // This is a placeholder for actual implementation
} 