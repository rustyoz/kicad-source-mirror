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

#ifndef CHEM_SCREEN_H
#define CHEM_SCREEN_H

#include "chem_item.h"
#include <color4d.h>
#include <vector>

class PLOTTER;

/**
 * Class CHEM_SCREEN
 * Represents a filter/screen element in a chemical process flow diagram.
 */
class CHEM_SCREEN : public CHEM_ITEM
{
public:
    /**
     * Enum defining the screen types
     */
    enum SCREEN_TYPE
    {
        FILTER,         ///< Basic filter
        MEMBRANE,       ///< Membrane filter
        SIEVE,          ///< Molecular sieve
        ION_EXCHANGE,   ///< Ion exchange filter
        CARBON,         ///< Carbon filter
        SEPARATOR,      ///< Phase separator
        CUSTOM          ///< Custom filter
    };

    /**
     * Constructor
     * @param aIUScale - The internal units scale
     */
    CHEM_SCREEN( const IU_PER_MILS& aIUScale = Mils2iu );

    /**
     * Copy constructor
     * @param aScreen - Source object
     */
    CHEM_SCREEN( const CHEM_SCREEN& aScreen );

    /**
     * Destructor
     */
    virtual ~CHEM_SCREEN();

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
     * Set the screen position
     * @param aPosition - The position
     */
    void SetPosition( const VECTOR2I& aPosition );

    /**
     * Get the screen position
     * @return The position
     */
    const VECTOR2I& GetPosition() const;

    /**
     * Set the screen size
     * @param aSize - The size
     */
    void SetSize( const VECTOR2I& aSize );

    /**
     * Get the screen size
     * @return The size
     */
    const VECTOR2I& GetSize() const;

    /**
     * Set the screen type
     * @param aType - The type
     */
    void SetScreenType( SCREEN_TYPE aType );

    /**
     * Get the screen type
     * @return The type
     */
    SCREEN_TYPE GetScreenType() const;

    /**
     * Set the screen description
     * @param aDescription - The description
     */
    void SetDescription( const wxString& aDescription );

    /**
     * Get the screen description
     * @return The description
     */
    const wxString& GetDescription() const;

    /**
     * Set the screen name
     * @param aName - The name
     */
    void SetName( const wxString& aName );

    /**
     * Get the screen name
     * @return The name
     */
    const wxString& GetName() const;

    /**
     * Set the rotation angle
     * @param aAngle - The rotation angle in tenths of a degree
     */
    void SetRotationAngle( int aAngle );

    /**
     * Get the rotation angle
     * @return The rotation angle in tenths of a degree
     */
    int GetRotationAngle() const;

    /**
     * Set the outline color
     * @param aColor - The color
     */
    void SetOutlineColor( const COLOR4D& aColor );

    /**
     * Get the outline color
     * @return The color
     */
    COLOR4D GetOutlineColor() const;

    /**
     * Set the fill color
     * @param aColor - The color
     */
    void SetFillColor( const COLOR4D& aColor );

    /**
     * Get the fill color
     * @return The color
     */
    COLOR4D GetFillColor() const;

    /**
     * Set the line width
     * @param aWidth - The line width
     */
    void SetLineWidth( int aWidth );

    /**
     * Get the line width
     * @return The line width
     */
    int GetLineWidth() const;

    /**
     * Set the mesh density (for drawing screen pattern)
     * @param aDensity - The mesh density (1-10)
     */
    void SetMeshDensity( int aDensity );

    /**
     * Get the mesh density
     * @return The mesh density
     */
    int GetMeshDensity() const;

    /**
     * Get the bounding box
     * @return The bounding box
     */
    BOX2I GetBoundingBox() const;

    /**
     * Get the bounding boxes for this screen
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
        return wxT( "CHEM_SCREEN" );
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
     * Plot the screen to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

    /**
     * Test if screen matches the search criteria
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data for search
     * @return True if matches
     */
    virtual bool Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const override;

private:
    VECTOR2I m_position;       ///< Position of the screen
    VECTOR2I m_size;           ///< Size of the screen
    SCREEN_TYPE m_screenType;  ///< Type of screen
    wxString m_description;    ///< Description
    wxString m_name;           ///< Name
    int m_rotationAngle;       ///< Rotation angle in tenths of a degree
    COLOR4D m_outlineColor;    ///< Color of the outline
    COLOR4D m_fillColor;       ///< Fill color
    int m_lineWidth;           ///< Width of the outline
    int m_meshDensity;         ///< Density of the mesh pattern (1-10)
};

#endif // CHEM_SCREEN_H 