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

#ifndef CHEM_SHEET_H
#define CHEM_SHEET_H

#include <vector>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/filename.h>
#include "chem_item.h"
#include "chem_includes.h"

class PLOTTER;

/**
 * Class CHEM_SHEET
 * Represents a hierarchical sheet in a chemical process flow diagram.
 */
class CHEM_SHEET : public CHEM_ITEM
{
public:
    /**
     * Constructor
     */
    CHEM_SHEET();
    
    /**
     * Copy constructor
     * @param aSheet - Source object
     */
    CHEM_SHEET( const CHEM_SHEET& aSheet );

    /**
     * Destructor
     */
    virtual ~CHEM_SHEET();

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
     * Set the sheet position
     * @param aPosition - The position of the sheet
     */
    void SetPosition( const wxPoint& aPosition );

    /**
     * Get the sheet position
     * @return The position of the sheet
     */
    wxPoint GetPosition() const;

    /**
     * Set the sheet size
     * @param aSize - The size of the sheet
     */
    void SetSize( const wxSize& aSize );

    /**
     * Get the sheet size
     * @return The size of the sheet
     */
    wxSize GetSize() const;

    /**
     * Set the sheet name
     * @param aName - The name of the sheet
     */
    void SetName( const wxString& aName );

    /**
     * Get the sheet name
     * @return The name of the sheet
     */
    wxString GetName() const;

    /**
     * Set the filename for the sheet
     * @param aFileName - The filename for the sheet
     */
    void SetFileName( const wxString& aFileName );

    /**
     * Get the filename for the sheet
     * @return The filename for the sheet
     */
    wxString GetFileName() const;

    /**
     * Set the border color
     * @param aColor - The border color
     */
    void SetBorderColor( const COLOR4D& aColor );

    /**
     * Get the border color
     * @return The border color
     */
    COLOR4D GetBorderColor() const;

    /**
     * Set the background color
     * @param aColor - The background color
     */
    void SetBackgroundColor( const COLOR4D& aColor );

    /**
     * Get the background color
     * @return The background color
     */
    COLOR4D GetBackgroundColor() const;

    /**
     * Set the border width
     * @param aWidth - The border width
     */
    void SetBorderWidth( int aWidth );

    /**
     * Get the border width
     * @return The border width
     */
    int GetBorderWidth() const;

    /**
     * Get the bounding box for this sheet
     * @return The bounding box
     */
    BOX2I GetBoundingBox() const;

    /**
     * Get the bounding boxes for this sheet
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
        return wxT( "CHEM_SHEET" );
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
     * Plot the sheet to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

    /**
     * Test if sheet matches the search criteria
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data for search
     * @return True if matches
     */
    virtual bool Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const override;

private:
    wxPoint m_position;           ///< Position of the sheet
    wxSize m_size;                ///< Size of the sheet
    wxString m_name;              ///< Name of the sheet
    wxString m_fileName;          ///< Filename for the sheet
    COLOR4D m_borderColor;        ///< Color of the sheet border
    COLOR4D m_backgroundColor;    ///< Background color of the sheet
    int m_borderWidth;            ///< Width of the sheet border
};

#endif // CHEM_SHEET_H 