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

#pragma once

#include <tool/tool_action.h>
#include <tool/actions.h>

class TOOL_EVENT;
class TOOL_MANAGER;

/**
 * Gather all the actions that are shared by tools. The instance of CHEM_ACTIONS is created
 * inside of ACTION_MANAGER object that registers the actions.
 */
class CHEM_ACTIONS : public ACTIONS
{
public:
    // Selection Tool
    /// Activation of the selection tool
    static TOOL_ACTION selectionActivate;

    /// Clears the current selection
    static TOOL_ACTION clearSelection;

    /// Selects an item (specified as the event parameter).
    static TOOL_ACTION addItemToSel;
    static TOOL_ACTION removeItemFromSel;

    /// Selects a list of items (specified as the event parameter)
    static TOOL_ACTION addItemsToSel;
    static TOOL_ACTION removeItemsFromSel;

    /// Runs a selection menu to select from a list of items
    static TOOL_ACTION selectionMenu;

    // Basic editing
    static TOOL_ACTION moveSelected;
    static TOOL_ACTION deleteSelected;
    static TOOL_ACTION duplicateSelected;
}; 