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

#include <vector>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include "base_screen.h"
// Forward declare CHEM_ITEM instead of including
class CHEM_ITEM;
#include <gal/color4d.h>
#include <geometry/rtree.h>

// Plot options for chemical schematics
struct CHEM_PLOT_OPTS
{
    bool m_plotBackground;
    bool m_plotBorder;
    bool m_plotTitle;
    bool m_plotLegend;
    bool m_plotGrid;
    bool m_plotAxes;
    bool m_plotLabels;
    bool m_plotValues;
    bool m_plotUnits;
    bool m_plotComments;
    bool m_plotAnnotations;
    bool m_plotFootprints;
    bool m_plotConnections;
    bool m_plotNetNames;
    bool m_plotNetCodes;
    bool m_plotBusEntries;
    bool m_plotHiddenPins;
    bool m_plotHiddenFields;
    bool m_plotHiddenText;
    bool m_plotHiddenGraphics;
    bool m_plotHiddenFootprints;
    bool m_plotHiddenConnections;
    bool m_plotHiddenNetNames;
    bool m_plotHiddenNetCodes;
    bool m_plotHiddenBusEntries;
    bool m_plotHiddenAnnotations;
    bool m_plotHiddenComments;
    bool m_plotHiddenValues;
    bool m_plotHiddenUnits;
    bool m_plotHiddenLabels;
    bool m_plotHiddenGrid;
    bool m_plotHiddenAxes;
    bool m_plotHiddenTitle;
    bool m_plotHiddenLegend;
    bool m_plotHiddenBorder;
    bool m_plotHiddenBackground;
};

class PLOTTER;
class UNITS_PROVIDER;
class CHEM_SCHEMATIC;

/**
 * Class CHEM_SCREEN
 * Represents a screen in a chemical process flow diagram.
 */
class CHEM_SCREEN : public BASE_SCREEN
{
public:
    /**
     * Constructor
     * @param aParent - Parent item
     */
    CHEM_SCREEN( EDA_ITEM* aParent = nullptr );

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
     * Implementation of the ViewGetLayers method from BASE_SCREEN
     */
    virtual std::vector<int> ViewGetLayers() const override;

    /**
     * Swap data with another item
     * @param aItem - Item to swap data with
     */
    virtual void SwapData( EDA_ITEM* aItem );

    /**
     * Set the screen position
     * @param aPosition - The position
     */
    virtual void SetPosition( const VECTOR2I& aPosition ) override;

    /**
     * Get the screen position
     * @return The position
     */
    virtual VECTOR2I GetPosition() const override;

    /**
     * Get the focus position (visual center)
     * @return The focus position
     */
    virtual const VECTOR2I GetFocusPosition() const override;

    /**
     * Get the sort position
     * @return The sort position
     */
    virtual VECTOR2I GetSortPosition() const override;

    /**
     * Get the item description for display
     * @param aUnitsProvider - Units provider for conversion
     * @param aFull - Whether to show full description
     * @return The description
     */
    virtual wxString GetItemDescription( UNITS_PROVIDER* aUnitsProvider, bool aFull ) const override;

    /**
     * Check if item supports text replacement
     * @return True if replaceable
     */
    virtual bool IsReplaceable() const override;

    /**
     * Replace text in the item
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data
     * @return True if replaced
     */
    virtual bool Replace( const EDA_SEARCH_DATA& aSearchData, void* aAuxData = nullptr ) override;

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
    void SetOutlineColor( const KIGFX::COLOR4D& aColor );

    /**
     * Get the outline color
     * @return The color
     */
    KIGFX::COLOR4D GetOutlineColor() const;

    /**
     * Set the fill color
     * @param aColor - The color
     */
    void SetFillColor( const KIGFX::COLOR4D& aColor );

    /**
     * Get the fill color
     * @return The color
     */
    KIGFX::COLOR4D GetFillColor() const;

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
     * Get the bounding box
     * @return The bounding box
     */
    virtual const BOX2I GetBoundingBox() const override;

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
    wxString GetSelectMenuText( EDA_UNITS aUnits ) const;

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

    /**
     * Get the parent schematic this screen belongs to.
     * @return The parent schematic, or nullptr.
     */
    CHEM_SCHEMATIC* Schematic() const;

    /**
     * Get the full RTree for iterating over items.
     * @return Complete RTree of the screen's items.
     */
    const RTree<CHEM_ITEM*, int, 2>& Items() const { return m_rtree; }

    /**
     * Check if the screen is empty.
     * @return true if the screen has no items.
     */
    bool IsEmpty() const
    {
        return m_rtree.Count() == 0;
    }

    /**
     * Check if the screen has items of a specific type.
     * @param aItemType - The type to check for.
     * @return true if the screen has items of the specified type.
     */
    bool HasItems( KICAD_T aItemType ) const;

    /**
     * Plot the screen to a plotter.
     * @param aPlotter - The plotter to plot to.
     * @param aBackground - Whether to plot background.
     * @param aPlotOpts - Plot options.
     * @param aUnit - Unit to plot.
     * @param aBodyStyle - Body style to plot.
     * @param aOffset - Offset for plotting.
     * @param aDimmed - Whether to plot dimmed.
     */
    virtual void Plot( PLOTTER* aPlotter, bool aBackground,
                      const CHEM_PLOT_OPTS& aPlotOpts,
                      int aUnit, int aBodyStyle,
                      const VECTOR2I& aOffset, bool aDimmed );

private:
    VECTOR2I m_position;     ///< Position of the screen
    VECTOR2I m_size;         ///< Size of the screen
    wxString m_description;  ///< Description
    wxString m_name;         ///< Name
    int m_rotationAngle;     ///< Rotation angle in tenths of a degree
    KIGFX::COLOR4D m_outlineColor;  ///< Color of the outline
    KIGFX::COLOR4D m_fillColor;     ///< Fill color
    int m_lineWidth;         ///< Width of the outline
    RTree<CHEM_ITEM*, int, 2> m_rtree;  ///< Spatial index for items
};

#endif // CHEM_SCREEN_H 