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

#include "chem_junction.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>

CHEM_JUNCTION::CHEM_JUNCTION() :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_JUNCTION_T )
{
    m_position = wxPoint( 0, 0 );
    m_type = NORMAL;
    m_diameter = 20;  // Default diameter in mils
    m_color = COLOR4D::UNSPECIFIED;
}


CHEM_JUNCTION::CHEM_JUNCTION( const CHEM_JUNCTION& aJunction ) :
    CHEM_ITEM( aJunction )
{
    m_position = aJunction.m_position;
    m_type = aJunction.m_type;
    m_diameter = aJunction.m_diameter;
    m_color = aJunction.m_color;
}


CHEM_JUNCTION::~CHEM_JUNCTION()
{
}


EDA_ITEM* CHEM_JUNCTION::Clone() const
{
    return new CHEM_JUNCTION( *this );
}


void CHEM_JUNCTION::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_CONNECTIONS;
    aCount = 1;
}


void CHEM_JUNCTION::SwapData( CHEM_ITEM* aItem )
{
    CHEM_JUNCTION* item = static_cast<CHEM_JUNCTION*>( aItem );

    std::swap( m_position, item->m_position );
    std::swap( m_type, item->m_type );
    std::swap( m_diameter, item->m_diameter );
    std::swap( m_color, item->m_color );
}


void CHEM_JUNCTION::SetPosition( const wxPoint& aPosition )
{
    m_position = aPosition;
}


wxPoint CHEM_JUNCTION::GetPosition() const
{
    return m_position;
}


void CHEM_JUNCTION::SetJunctionType( JUNCTION_TYPE aType )
{
    m_type = aType;
}


CHEM_JUNCTION::JUNCTION_TYPE CHEM_JUNCTION::GetJunctionType() const
{
    return m_type;
}


void CHEM_JUNCTION::SetDiameter( int aDiameter )
{
    m_diameter = aDiameter;
}


int CHEM_JUNCTION::GetDiameter() const
{
    return m_diameter;
}


void CHEM_JUNCTION::SetColor( const COLOR4D& aColor )
{
    m_color = aColor;
}


COLOR4D CHEM_JUNCTION::GetColor() const
{
    return m_color;
}


BOX2I CHEM_JUNCTION::GetBoundingBox() const
{
    int radius = m_diameter / 2;
    return BOX2I( VECTOR2I( m_position.x - radius, m_position.y - radius ),
                 VECTOR2I( m_diameter, m_diameter ) );
}


std::vector<BOX2I> CHEM_JUNCTION::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    boxes.push_back( GetBoundingBox() );
    return boxes;
}


wxString CHEM_JUNCTION::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    wxString type;
    
    switch( m_type )
    {
    case NORMAL:
        type = _( "Junction" );
        break;
    case TEE:
        type = _( "T-Junction" );
        break;
    case CROSS:
        type = _( "Cross Junction" );
        break;
    case ELBOW:
        type = _( "Elbow Junction" );
        break;
    default:
        type = _( "Junction" );
        break;
    }
    
    return type;
}


BITMAPS CHEM_JUNCTION::GetMenuImage() const
{
    return BITMAPS::add_junction;
}


bool CHEM_JUNCTION::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    // Calculate the distance from the position to the center of the junction
    int dx = aPosition.x - m_position.x;
    int dy = aPosition.y - m_position.y;
    int distance = (int) sqrt( dx * dx + dy * dy );
    
    // Hit if the distance is less than the radius plus accuracy
    return distance <= ( m_diameter / 2 + aAccuracy );
}


bool CHEM_JUNCTION::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    BOX2I box = GetBoundingBox();
    
    if( aContains )
    {
        // The selection rectangle must fully contain the junction
        return aRect.Contains( box );
    }
    else
    {
        // The selection rectangle must intersect the junction
        return aRect.Intersects( box );
    }
}


void CHEM_JUNCTION::Plot( PLOTTER* aPlotter ) const
{
    // Implement plotting for the junction
    // This is a placeholder for actual implementation
} 