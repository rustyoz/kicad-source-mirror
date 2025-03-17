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

#ifndef CHEM_TEXTBOX_H
#define CHEM_TEXTBOX_H

#include <eda_text.h>
#include "chem_item.h"
#include "chem_includes.h"

class PLOTTER;

/**
 * Class CHEM_TEXTBOX
 * Represents a text element contained in a box in a chemical process flow diagram.
 */
class CHEM_TEXTBOX : public CHEM_ITEM, public EDA_TEXT
{
public:
    /**
     * Constructor
     * @param aIUScale - The internal units scale
     */
    CHEM_TEXTBOX( const IU_PER_MILS& aIUScale = Mils2iu );

    /**
     * Copy constructor
     * @param aTextBox - Source object
     */
    CHEM_TEXTBOX( const CHEM_TEXTBOX& aTextBox );

    /**
     * Destructor
     */
    virtual ~CHEM_TEXTBOX();

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
     * Set the size of the text box
     * @param aSize - The box size
     */
    void SetBoxSize( const VECTOR2I& aSize );

    /**
     * Get the size of the text box
     * @return The box size
     */
    VECTOR2I GetBoxSize() const;

    /**
     * Set the line width of the text box
     * @param aWidth - The line width
     */
    void SetLineWidth( int aWidth );

    /**
     * Get the line width of the text box
     * @return The line width
     */
    int GetLineWidth() const;

    /**
     * Set the fill color of the text box
     * @param aColor - The fill color
     */
    void SetFillColor( const COLOR4D& aColor );

    /**
     * Get the fill color of the text box
     * @return The fill color
     */
    COLOR4D GetFillColor() const;

    /**
     * Set the line color of the text box
     * @param aColor - The line color
     */
    void SetLineColor( const COLOR4D& aColor );

    /**
     * Get the line color of the text box
     * @return The line color
     */
    COLOR4D GetLineColor() const;

    /**
     * Set the corner radius of the text box
     * @param aRadius - The corner radius
     */
    void SetCornerRadius( int aRadius );

    /**
     * Get the corner radius of the text box
     * @return The corner radius
     */
    int GetCornerRadius() const;

    /**
     * Get the bounds of the text box
     * @return The box bounds
     */
    BOX2I GetBoxBounds() const;

    /**
     * Get the bounding boxes for this text box
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
        return wxT( "CHEM_TEXTBOX" );
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
     * Plot the text box to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

    /**
     * Test if text matches the search criteria
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data for search
     * @return True if matches
     */
    virtual bool Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const override;

private:
    VECTOR2I m_boxSize;      ///< Size of the text box
    int m_lineWidth;         ///< Width of the box outline
    COLOR4D m_fillColor;     ///< Fill color of the box
    COLOR4D m_lineColor;     ///< Color of the box outline
    int m_cornerRadius;      ///< Radius of the box corners
};

#endif // CHEM_TEXTBOX_H 