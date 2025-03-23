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

#ifndef CHEM_VIEW_H
#define CHEM_VIEW_H

#include <view/view.h>
#include <memory>

class CHEM_BASE_FRAME;
class CHEM_SCREEN;
class LIB_SYMBOL;
class DS_PROXY_VIEW_ITEM;

namespace KIGFX
{
    class VIEW_GROUP;

    namespace PREVIEW
    {
        class SELECTION_AREA;
    };

/**
 * Class to store display options for the chemical schematic
 */
class CHEM_DISPLAY_OPTIONS
{
public:
    bool m_showGrid;
    bool m_showLabels;
    bool m_showConnections;
    bool m_showSymbols;
    
    CHEM_DISPLAY_OPTIONS() :
        m_showGrid( true ),
        m_showLabels( true ),
        m_showConnections( true ),
        m_showSymbols( true )
    {
    }
};

class CHEM_SCHEMATIC;

/**
 * Class CHEM_VIEW
 * is a VIEW implementation for the chemical schematic.
 */
class CHEM_VIEW : public KIGFX::VIEW
{
public:
    // Note: aFrame is used to know the sheet path name when drawing the drawing sheet.
    // It can be null.
    CHEM_VIEW( CHEM_BASE_FRAME* aFrame );
    ~CHEM_VIEW();

    void Cleanup();

    void DisplaySheet( const CHEM_SCREEN* aScreen );
    void DisplaySymbol( LIB_SYMBOL* aSymbol );

    // Call it to set new draw area limits (max working and draw area size)
    void ResizeSheetWorkingArea( const CHEM_SCREEN* aScreen );

    void SetScale( double aScale, VECTOR2D aAnchor = { 0, 0 } ) override;

    /**
     * Clear the hide flag of all items in the view
     */
    void ClearHiddenFlags();

    void HideDrawingSheet();

    DS_PROXY_VIEW_ITEM* GetDrawingSheet() const { return m_drawingSheet.get(); }

    // Set the chemical schematic model for this view
    void SetChemSchematic( CHEM_SCHEMATIC* aChemSchematic );
    
    // Get the chemical schematic model
    CHEM_SCHEMATIC* GetChemSchematic() const;
    
    // Update all items in the view
    void UpdateAllItems();
    
    // Update display options
    void UpdateDisplayOptions( const CHEM_DISPLAY_OPTIONS& aOptions );

private:
    CHEM_BASE_FRAME* m_frame;    // The frame using this view. Can be null. Used mainly
                                 // to know the sheet path name when drawing the drawing sheet

    std::unique_ptr<DS_PROXY_VIEW_ITEM> m_drawingSheet;

    CHEM_SCHEMATIC* m_chemSchematic;  // The chemical schematic model being viewed
};

}; // namespace KIGFX

#endif // CHEM_VIEW_H 