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

#ifndef CHEM_JUNCTION_H
#define CHEM_JUNCTION_H

#include <wx/gdicmn.h>
#include "chem_item.h"
#include <color4d.h>

class PLOTTER;

/**
 * Class CHEM_JUNCTION
 * Represents a junction point in a chemical process flow diagram.
 */
class CHEM_JUNCTION : public CHEM_ITEM
{
public:
    /**
     * Enum defining the junction types
     */
    enum JUNCTION_TYPE
    {
        NORMAL,     ///< Standard junction
        TEE,        ///< T-junction
        CROSS,      ///< Cross junction
        ELBOW       ///< Elbow junction (corner)
    };

    /**
     * Constructor
     */
    CHEM_JUNCTION();
    
    /**
     * Copy constructor
     * @param aJunction - Source object
     */
    CHEM_JUNCTION( const CHEM_JUNCTION& aJunction );

    /**
     * Destructor
     */
    virtual ~CHEM_JUNCTION();

    /**
     * Create a copy of this object
     * @return A new copy of this object
     */
    virtual EDA_ITEM* Clone() const override;

    /**
     * Implementation of the ViewGetLayers method from CHEM_ITEM
     */
    virtual void ViewGetLayers( int aLayers[], int& aCount ) const override;

    /**
     * Swap data with another item
     * @param aItem - Item to swap data with
     */
    virtual void SwapData( CHEM_ITEM* aItem );

    /**
     * Set the junction position
     * @param aPosition - The position of the junction
     */
    void SetPosition( const wxPoint& aPosition );

    /**
     * Get the junction position
     * @return The position of the junction
     */
    wxPoint GetPosition() const;

    /**
     * Set the junction type
     * @param aType - The type of junction
     */
    void SetJunctionType( JUNCTION_TYPE aType );

    /**
     * Get the junction type
     * @return The type of junction
     */
    JUNCTION_TYPE GetJunctionType() const;

    /**
     * Set the junction diameter
     * @param aDiameter - The diameter of the junction
     */
    void SetDiameter( int aDiameter );

    /**
     * Get the junction diameter
     * @return The diameter of the junction
     */
    int GetDiameter() const;

    /**
     * Set the junction color
     * @param aColor - The color of the junction
     */
    void SetColor( const COLOR4D& aColor );

    /**
     * Get the junction color
     * @return The color of the junction
     */
    COLOR4D GetColor() const;

    /**
     * Get the bounding box for this junction
     * @return The bounding box
     */
    BOX2I GetBoundingBox() const;

    /**
     * Get the bounding boxes for this junction
     * @return Vector of bounding boxes
     */
    virtual std::vector<BOX2I> GetBoundingBoxes() const;

    /**
     * Get the text to display in a selection menu
     * @param aUnits - Units for display
     * @return String for menu
     */
    virtual wxString GetSelectMenuText( EDA_UNITS aUnits ) const override;

    /**
     * Get the icon for menus
     * @return Icon for menus
     */
    virtual BITMAPS GetMenuImage() const override;

    /**
     * Return the class name
     * @return Class name
     */
    virtual wxString GetClass() const override
    {
        return wxT( "CHEM_JUNCTION" );
    }

    /**
     * Hit test for position
     * @param aPosition - Position to test
     * @param aAccuracy - Hit test accuracy
     * @return True if hit
     */
    virtual bool HitTest( const VECTOR2I& aPosition, int aAccuracy = 0 ) const override;

    /**
     * Hit test for bounding box
     * @param aRect - Rectangle to test
     * @param aContains - True to test if fully contains, false to test for intersection
     * @param aAccuracy - Hit test accuracy
     * @return True if hit
     */
    virtual bool HitTest( const BOX2I& aRect, bool aContains, int aAccuracy = 0 ) const override;

    /**
     * Plot the junction to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

private:
    wxPoint m_position;       ///< Position of the junction
    JUNCTION_TYPE m_type;     ///< Type of junction
    int m_diameter;           ///< Diameter of the junction
    COLOR4D m_color;          ///< Color of the junction
};

#endif // CHEM_JUNCTION_H 