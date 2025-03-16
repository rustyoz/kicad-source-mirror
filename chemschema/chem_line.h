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

#ifndef CHEM_LINE_H
#define CHEM_LINE_H

#include <vector>
#include <wx/gdicmn.h>
#include "chem_item.h"
#include <color4d.h>

class PLOTTER;

/**
 * Class CHEM_LINE
 * Represents a line or connection in a chemical process flow diagram.
 */
class CHEM_LINE : public CHEM_ITEM
{
public:
    /**
     * Enum defining the line styles
     */
    enum LINE_STYLE
    {
        SOLID,
        DASHED,
        DOTTED,
        DASHDOT
    };

    /**
     * Constructor
     */
    CHEM_LINE();
    
    /**
     * Copy constructor
     * @param aLine - Source object
     */
    CHEM_LINE( const CHEM_LINE& aLine );

    /**
     * Destructor
     */
    virtual ~CHEM_LINE();

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
     * Set the line points
     * @param aPoints - Vector of points defining the line
     */
    void SetPoints( const std::vector<wxPoint>& aPoints );

    /**
     * Get the line points
     * @return Vector of points defining the line
     */
    const std::vector<wxPoint>& GetPoints() const;

    /**
     * Add a point to the line
     * @param aPoint - Point to add
     */
    void AddPoint( const wxPoint& aPoint );

    /**
     * Set the line width
     * @param aWidth - The line width
     */
    void SetWidth( int aWidth );

    /**
     * Get the line width
     * @return The line width
     */
    int GetWidth() const;

    /**
     * Set the line style
     * @param aStyle - The line style
     */
    void SetLineStyle( LINE_STYLE aStyle );

    /**
     * Get the line style
     * @return The line style
     */
    LINE_STYLE GetLineStyle() const;

    /**
     * Set the line color
     * @param aColor - The line color
     */
    void SetColor( const COLOR4D& aColor );

    /**
     * Get the line color
     * @return The line color
     */
    COLOR4D GetColor() const;

    /**
     * Get the bounding boxes for this line
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
        return wxT( "CHEM_LINE" );
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
     * Plot the line to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

private:
    std::vector<wxPoint> m_points;   ///< Points defining the line
    int m_width;                     ///< Width of the line
    LINE_STYLE m_lineStyle;          ///< Style of the line
    COLOR4D m_color;                 ///< Color of the line
};

#endif // CHEM_LINE_H 