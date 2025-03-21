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

#ifndef CHEM_DRAWING_TOOLS_H
#define CHEM_DRAWING_TOOLS_H

#include <confirm.h>
#include <string_utils.h>
#include <kiface_base.h>
#include <eda_units.h>
#include <tool/tool_interactive.h>
#include <tool/tool_menu.h>

// Drawing states
enum DRAWING_STATE
{
    WAITING_FOR_START,
    DRAWING,
    WAITING_FOR_END
};

class CHEM_EDIT_FRAME;
class CHEM_ITEM;
class CHEM_LINE;
class CHEM_SYMBOL;
class CHEM_JUNCTION;
class CHEM_LABEL;
class CHEM_SCREEN;
class CHEM_SHEET;

/**
 * CHEM_DRAWING_TOOLS
 * Tools for interactive drawing of chemical schematic elements.
 */
class CHEM_DRAWING_TOOLS : public TOOL_INTERACTIVE
{
public:
    /**
     * Constructor
     */
    CHEM_DRAWING_TOOLS();

    /**
     * Destructor
     */
    ~CHEM_DRAWING_TOOLS();

    /**
     * Get the name of the tool
     * @return Tool name
     */
    const char* GetName() const { return "chemschema.InteractiveDrawing"; }

    /**
     * Reset the tool state
     * @param aReason - Reason for the reset
     */
    void Reset( RESET_REASON aReason ) override;

    /**
     * Initialize this tool
     */
    void setTransitions() override;

    /**
     * Main function to handle interactive drawing
     * @param aEvent - Tool activation event
     */
    int InteractiveDrawing( const TOOL_EVENT& aEvent );

    /**
     * Draw a line interactively
     * @param aEvent - Tool event
     */
    int DrawLine( const TOOL_EVENT& aEvent );

    /**
     * Place a symbol interactively
     * @param aEvent - Tool event
     */
    int PlaceSymbol( const TOOL_EVENT& aEvent );

    /**
     * Place a junction interactively
     * @param aEvent - Tool event
     */
    int PlaceJunction( const TOOL_EVENT& aEvent );

    /**
     * Place a label interactively
     * @param aEvent - Tool event
     */
    int PlaceLabel( const TOOL_EVENT& aEvent );



    /**
     * Place a sheet interactively
     * @param aEvent - Tool event
     */
    int PlaceSheet( const TOOL_EVENT& aEvent );

private:
    /**
     * Draw a preview of the item being placed
     * @param aItem - The item to preview
     */
    void DrawPreview( CHEM_ITEM* aItem );

    /**
     * Update the position of an item being placed
     * @param aItem - The item to update
     * @param aPosition - The new position
     */
    void UpdateItemPosition( CHEM_ITEM* aItem, const VECTOR2I& aPosition );

    /**
     * Get the current cursor position
     * @return The current cursor position
     */
    VECTOR2I GetCursorPosition();

    /**
     * Set the drawing mode
     * @param aMode - The drawing mode
     */
    void SetDrawingMode( int aMode );

    /**
     * Check if the current position is valid for placement
     * @param aItem - The item to check
     * @param aPosition - The position to check
     * @return True if the position is valid
     */
    bool IsPositionValid( CHEM_ITEM* aItem, const VECTOR2I& aPosition );

    // Private members
    CHEM_EDIT_FRAME* m_frame;   ///< Editor frame
    int m_drawingMode;          ///< Current drawing mode
    DRAWING_STATE m_drawingState; ///< Current drawing state
    CHEM_ITEM* m_currentItem;   ///< Item currently being drawn
    VECTOR2I m_startPoint;      ///< Start point for line drawing
    bool m_isAddingMultiple;    ///< Flag to add multiple items
};

// Drawing modes
enum CHEM_DRAWING_MODE
{
    CHEM_DRAWING_LINE,
    CHEM_DRAWING_SYMBOL,
    CHEM_DRAWING_JUNCTION,
    CHEM_DRAWING_LABEL,
    CHEM_DRAWING_SCREEN,
    CHEM_DRAWING_SHEET
};

#endif // CHEM_DRAWING_TOOLS_H
