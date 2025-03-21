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

#ifndef CHEM_SYMBOL_H
#define CHEM_SYMBOL_H

#include <vector>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include "chem_item.h"
#include "chem_includes.h"

class PLOTTER;
class CHEM_SHAPE;
class CHEM_TEXT;
class CHEM_LINE;

/**
 * Class CHEM_SYMBOL
 * Represents a chemical symbol in a process flow diagram.
 */
class CHEM_SYMBOL : public CHEM_ITEM
{
public:
    /**
     * Enum defining the symbol types
     */
    enum SYMBOL_TYPE
    {
        REACTOR,
        TANK,
        PUMP,
        VALVE,
        HEAT_EXCHANGER,
        COMPRESSOR,
        DISTILLATION_COLUMN,
        FILTER,
        MIXER,
        CUSTOM
    };

    /**
     * Constructor
     */
    CHEM_SYMBOL();
    
    /**
     * Copy constructor
     * @param aSymbol - Source object
     */
    CHEM_SYMBOL( const CHEM_SYMBOL& aSymbol );

    /**
     * Destructor
     */
    virtual ~CHEM_SYMBOL();

    /**
     * Create a copy of this object
     * @return A new copy of this object
     */
    virtual EDA_ITEM* Clone() const override;

    /**
     * Get the list of layers this item is drawn on
     * @return Vector of layer indices
     */
    virtual std::vector<int> ViewGetLayers() const override;

    /**
     * Swap data with another item
     * @param aItem - Item to swap data with
     */
    virtual void SwapData( CHEM_ITEM* aItem );

    /**
     * Set the symbol position
     * @param aPosition - The position of the symbol
     */
    void SetPosition( const wxPoint& aPosition );

    /**
     * Get the symbol position
     * @return The position of the symbol
     */
    virtual VECTOR2I GetPosition() const override;

    /**
     * Set the symbol type
     * @param aType - The symbol type
     */
    void SetSymbolType( SYMBOL_TYPE aType );

    /**
     * Get the symbol type
     * @return The symbol type
     */
    SYMBOL_TYPE GetSymbolType() const;

    /**
     * Set the symbol name
     * @param aName - The name of the symbol
     */
    void SetName( const wxString& aName );

    /**
     * Get the symbol name
     * @return The name of the symbol
     */
    wxString GetName() const;

    /**
     * Set the symbol description
     * @param aDescription - The description of the symbol
     */
    void SetDescription( const wxString& aDescription );

    /**
     * Get the symbol description
     * @return The description of the symbol
     */
    wxString GetDescription() const;

    /**
     * Set the symbol rotation angle
     * @param aAngle - The rotation angle in degrees
     */
    void SetRotationAngle( double aAngle );

    /**
     * Get the symbol rotation angle
     * @return The rotation angle in degrees
     */
    double GetRotationAngle() const;

    /**
     * Add a shape to the symbol
     * @param aShape - The shape to add
     */
    void AddShape( CHEM_SHAPE* aShape );

    /**
     * Get all shapes in the symbol
     * @return Vector of shape pointers
     */
    const std::vector<CHEM_SHAPE*>& GetShapes() const;

    /**
     * Add text to the symbol
     * @param aText - The text to add
     */
    void AddText( CHEM_TEXT* aText );

    /**
     * Get all text in the symbol
     * @return Vector of text pointers
     */
    const std::vector<CHEM_TEXT*>& GetTexts() const;

    /**
     * Add a line to the symbol
     * @param aLine - The line to add
     */
    void AddLine( CHEM_LINE* aLine );

    /**
     * Get all lines in the symbol
     * @return Vector of line pointers
     */
    const std::vector<CHEM_LINE*>& GetLines() const;

    /**
     * Get the bounding boxes for this symbol
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
        return wxT( "CHEM_SYMBOL" );
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
     * Plot the symbol to a plotter
     * @param aPlotter - Plotter object
     */
    virtual void Plot( PLOTTER* aPlotter ) const;

    /**
     * Test if symbol matches the search criteria
     * @param aSearchData - Search criteria
     * @param aAuxData - Auxiliary data for search
     * @return True if matches
     */
    virtual bool Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const override;

private:
    wxPoint m_position;                    ///< Position of the symbol
    SYMBOL_TYPE m_symbolType;              ///< Type of the symbol
    wxString m_name;                       ///< Name of the symbol
    wxString m_description;                ///< Description of the symbol
    double m_rotationAngle;                ///< Rotation angle in degrees
    std::vector<CHEM_SHAPE*> m_shapes;     ///< Shapes in the symbol
    std::vector<CHEM_TEXT*> m_texts;       ///< Text in the symbol
    std::vector<CHEM_LINE*> m_lines;       ///< Lines in the symbol
};

#endif // CHEM_SYMBOL_H 