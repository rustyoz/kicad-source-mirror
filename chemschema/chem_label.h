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

#ifndef CHEM_LABEL_H
#define CHEM_LABEL_H

#include <eda_text.h>
#include "chem_item.h"
#include <gal/color4d.h>

class PLOTTER;

/**
 * Class CHEM_LABEL
 * Represents a label in a chemical process flow diagram.
 */
class CHEM_LABEL : public CHEM_ITEM, public EDA_TEXT
{
public:
    /**
     * Enum defining the label types
     */
    enum LABEL_TYPE
    {
        INLINE,         ///< Inline label
        FREESTANDING,   ///< Freestanding label
        FLOW,           ///< Flow label
        TEMPERATURE,    ///< Temperature label
        PRESSURE,       ///< Pressure label
        CHEMICAL,       ///< Chemical label
        CUSTOM          ///< Custom label
    };

    /**
     * Constructor
     * @param aIUScale - The internal units scale
     */
    CHEM_LABEL( const IU_PER_MILS& aIUScale = Mils2iu );

    /**
     * Copy constructor
     * @param aLabel - Source object
     */
    CHEM_LABEL( const CHEM_LABEL& aLabel );

    /**
     * Destructor
     */
    virtual ~CHEM_LABEL();

    /**
     * Create a copy of this object
     * @return A new copy of this object
     */
    virtual EDA_ITEM* Clone() const override;

    /**
     * Implementation of the ViewGetLayers method from CHEM_ITEM
     */
    virtual std::vector<int> ViewGetLayers() const override;

    /**
     * Swap data with another item
     * @param aItem - Item to swap data with
     */
    virtual void SwapData( CHEM_ITEM* aItem );

    /**
     * Set the label type
     * @param aType - The type of label
     */
    void SetLabelType( LABEL_TYPE aType );

    /**
     * Get the label type
     * @return The type of label
     */
    LABEL_TYPE GetLabelType() const;

    /**
     * Set whether the label has a box
     * @param aHasBox - True if the label has a box
     */
    void SetHasBox( bool aHasBox );

    /**
     * Check if the label has a box
     * @return True if the label has a box
     */
    bool HasBox() const;

    /**
     * Set the box line width
     * @param aWidth - The line width
     */
    void SetBoxLineWidth( int aWidth );

    /**
     * Get the box line width
     * @return The line width
     */
    int GetBoxLineWidth() const;

    /**
     * Set the box line color
     * @param aColor - The line color
     */
    void SetBoxLineColor( const COLOR4D& aColor );

    /**
     * Get the box line color
     * @return The line color
     */
    COLOR4D GetBoxLineColor() const;

    /**
     * Set the box fill color
     * @param aColor - The fill color
     */
    void SetBoxFillColor( const COLOR4D& aColor );

    /**
     * Get the box fill color
     * @return The fill color
     */
    COLOR4D GetBoxFillColor() const;

    /**
     * Get the box bounds
     * @return The box bounds
     */
    BOX2I GetBoxBounds() const;

    /**
     * Get the bounding boxes for this label
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
     * Get a shortened version of the text for display
     * @return Shortened text
     */
    wxString ShortenedText() const;

    /**
     * Return the class name
     * @return Class name
     */
    virtual wxString GetClass() const override
    {
        return wxT( "CHEM_LABEL" );
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
     * Plot the label to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

    /**
     * Test if label matches the search criteria
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data for search
     * @return True if matches
     */
    virtual bool Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const override;

private:
    LABEL_TYPE m_labelType;     ///< Type of label
    bool m_hasBox;              ///< Whether the label has a box
    int m_boxLineWidth;         ///< Width of the box line
    COLOR4D m_boxLineColor;     ///< Color of the box line
    COLOR4D m_boxFillColor;     ///< Fill color of the box
};

#endif // CHEM_LABEL_H 