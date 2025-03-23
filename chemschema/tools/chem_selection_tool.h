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

#ifndef CHEM_SELECTION_TOOL_H
#define CHEM_SELECTION_TOOL_H

#include <eda_units.h>
#include <tool/tool_manager.h>
#include <tool/action_menu.h>
#include <tool/tool_event.h>
#include <tool/tool_action.h>
#include <tool/common_tools.h>
#include <tool/tool_interactive.h>
#include <tool/selection.h>
#include <tool/selection_conditions.h>
#include <tool/tool_menu.h>
#include <tool/actions.h>
#include <tool/selection_tool.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <geometry/seg.h>

#include "chem_item.h"
#include "chem_schematic.h"
#include "chem_base_frame.h"
#include "chem_draw_panel.h"
#include "chem_edit_frame.h"
#include "chem_symbol.h"
#include "chem_sheet.h"
#include "chem_sheet_path.h"
#include "chem_connection.h"
#include "chem_label.h"
#include "tools/chem_selection.h"
#include "../chem_collectors.h"

class CHEM_EDIT_FRAME;

/**
 * CHEM_SELECTION_TOOL
 * Tool responsible for selecting and editing items in the chemical schematic editor.
 */
class CHEM_SELECTION_TOOL : public SELECTION_TOOL
{
public:
    /**
     * Constructor
     */
    CHEM_SELECTION_TOOL();

    /**
     * Destructor
     */
    ~CHEM_SELECTION_TOOL() override;

    /**
     * Reset the tool state
     * @param aReason - Reason for the reset
     */
    void Reset( RESET_REASON aReason = MODEL_RELOAD ) override;

    /**
     * Initialize this tool
     */
    void setTransitions() override;

    /**
     * Main loop for selection tool
     * @param aEvent - Tool activation event
     */
    int Main( const TOOL_EVENT& aEvent );

    /**
     * Select a single item at the current cursor position
     * @param aEvent - Tool event
     */
    int SelectSingle( const TOOL_EVENT& aEvent );

    /**
     * Select multiple items using drag selection
     * @param aEvent - Tool event
     */
    int SelectMultiple( const TOOL_EVENT& aEvent );

    /**
     * Select all visible items in the schematic
     * @param aEvent - Tool event
     */
    int SelectAll( const TOOL_EVENT& aEvent );

    /**
     * Set the context menu for the selection tool
     * @param aMenu - Pointer to the menu
     */
    void SetContextMenu( TOOL_MENU* aMenu ) { m_menu = aMenu; }

    /**
     * Initialize the selection tool's menu
     */
    void InitMenu();

    /**
     * Move selected items
     * @param aEvent - Tool event
     */
    int MoveSelected( const TOOL_EVENT& aEvent );

    /**
     * Handle context menu activation
     * @param aEvent - Tool event
     */
    int ContextMenu( const TOOL_EVENT& aEvent );

    void ClearSelection();

    /**
     * @return true if the given point is contained in any of selected items' bounding boxes.
     */
    bool selectionContains( const VECTOR2I& aPoint ) const;

    /**
     * Get the current selection
     */
    CHEM_SELECTION& GetSelection() { return m_selection; }

protected:
    /**
     * Handle click on an item
     * @param aPosition - Current cursor position
     * @param aEvent - Tool event
     * @param aAllowDisambiguation - True to allow disambiguation menu
     */
    bool HandleClick( const VECTOR2I& aPosition, const TOOL_EVENT& aEvent, bool aAllowDisambiguation = true );
    /**
     * Disambiguate multiple items at the same position
     * @param aItems - List of items to disambiguate
     * @param aPosition - Current cursor position
     */
    EDA_ITEM* disambiguateItem( const std::vector<EDA_ITEM*>& aItems, const VECTOR2I& aPosition );

    SELECTION& selection() override { return m_selection; }

private:
    // Private members
    CHEM_EDIT_FRAME*  m_frame;
    TOOL_MENU*        m_menu;             ///< Context menu
    VECTOR2I          m_dragStartPos;     ///< Start position of drag selection
    bool              m_skipMenuEvent;    ///< Flag to skip menu events
    CHEM_SELECTION    m_selection;        ///< Current selection
};

#endif // CHEM_SELECTION_TOOL_H 