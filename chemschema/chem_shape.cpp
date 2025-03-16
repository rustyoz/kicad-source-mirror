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

#include "chem_shape.h"

CHEM_SHAPE::CHEM_SHAPE() : CHEM_ITEM( nullptr, CHEM_SHAPE_T )
{
    // Initialize default shape properties
    m_width = 0;
    m_height = 0;
    m_pos = wxPoint( 0, 0 );
    m_rotation = 0.0;
    m_filled = false;
    m_shapeType = SHAPE_RECTANGLE;
}


CHEM_SHAPE::~CHEM_SHAPE()
{
    // Clean up any resources
}


void CHEM_SHAPE::SetPosition( const wxPoint& aPosition )
{
    m_pos = aPosition;
}


wxPoint CHEM_SHAPE::GetPosition() const
{
    return m_pos;
}


void CHEM_SHAPE::SetSize( int aWidth, int aHeight )
{
    m_width = aWidth;
    m_height = aHeight;
}


void CHEM_SHAPE::GetSize( int& aWidth, int& aHeight ) const
{
    aWidth = m_width;
    aHeight = m_height;
}


void CHEM_SHAPE::SetRotation( double aRotation )
{
    m_rotation = aRotation;
}


double CHEM_SHAPE::GetRotation() const
{
    return m_rotation;
}


void CHEM_SHAPE::SetShapeType( SHAPE_TYPE aType )
{
    m_shapeType = aType;
}


CHEM_SHAPE::SHAPE_TYPE CHEM_SHAPE::GetShapeType() const
{
    return m_shapeType;
}


void CHEM_SHAPE::SetFilled( bool aFilled )
{
    m_filled = aFilled;
}


bool CHEM_SHAPE::IsFilled() const
{
    return m_filled;
}


void CHEM_SHAPE::ViewGetLayers( int aLayers[], int& aCount ) const
{
    // In the future, implement proper layer handling
    aCount = 0;
} 