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

#ifndef CHEM_SHAPE_H
#define CHEM_SHAPE_H

#include <wx/gdicmn.h>
#include "chem_item.h"

/**
 * Class CHEM_SHAPE
 * Represents a basic shape in a chemical process flow diagram.
 */
class CHEM_SHAPE : public CHEM_ITEM
{
public:
    /**
     * Enum defining the basic shape types
     */
    enum SHAPE_TYPE
    {
        SHAPE_RECTANGLE,
        SHAPE_CIRCLE,
        SHAPE_ELLIPSE,
        SHAPE_TRIANGLE,
        SHAPE_HEXAGON,
        SHAPE_POLYGON
    };

    CHEM_SHAPE();
    virtual ~CHEM_SHAPE();

    /**
     * Set the position of the shape.
     * @param aPosition The position coordinates.
     */
    void SetPosition( const wxPoint& aPosition );

    /**
     * Get the position of the shape.
     * @return The position coordinates.
     */
    wxPoint GetPosition() const;

    /**
     * Set the size of the shape.
     * @param aWidth The width of the shape.
     * @param aHeight The height of the shape.
     */
    void SetSize( int aWidth, int aHeight );

    /**
     * Get the size of the shape.
     * @param aWidth Reference to store the width.
     * @param aHeight Reference to store the height.
     */
    void GetSize( int& aWidth, int& aHeight ) const;

    /**
     * Set the rotation of the shape.
     * @param aRotation The rotation angle in degrees.
     */
    void SetRotation( double aRotation );

    /**
     * Get the rotation of the shape.
     * @return The rotation angle in degrees.
     */
    double GetRotation() const;

    /**
     * Set the shape type.
     * @param aType The shape type.
     */
    void SetShapeType( SHAPE_TYPE aType );

    /**
     * Get the shape type.
     * @return The shape type.
     */
    SHAPE_TYPE GetShapeType() const;

    /**
     * Set whether the shape is filled or just outlined.
     * @param aFilled True for filled, false for outline only.
     */
    void SetFilled( bool aFilled );

    /**
     * Check if the shape is filled.
     * @return True if filled, false if outline only.
     */
    bool IsFilled() const;

    /**
     * Implementation of the ViewGetLayers method from CHEM_ITEM
     */
    virtual void ViewGetLayers( int aLayers[], int& aCount ) const override;

private:
    wxPoint m_pos;          ///< Position of the shape
    int m_width;            ///< Width of the shape
    int m_height;           ///< Height of the shape
    double m_rotation;      ///< Rotation angle in degrees
    bool m_filled;          ///< Whether the shape is filled or just outlined
    SHAPE_TYPE m_shapeType; ///< Type of the shape
};

#endif // CHEM_SHAPE_H 