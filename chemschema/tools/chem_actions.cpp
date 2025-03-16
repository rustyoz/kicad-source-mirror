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

#include <tool/actions.h>
#include <tool/tool_action.h>
#include "chem_editor_control.h"

// Drawing tool actions
TOOL_ACTION CHEMSCHEMA_ACTIONS::newSymbol(
    "chemschema.EditorControl.newSymbol",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'S',
    _( "New Symbol" ), _( "Create a new symbol" ),
    symbol_xpm, AF_NONE );

TOOL_ACTION CHEMSCHEMA_ACTIONS::newLine(
    "chemschema.EditorControl.newLine",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'L',
    _( "New Line" ), _( "Create a new line" ),
    add_line_xpm, AF_NONE );

TOOL_ACTION CHEMSCHEMA_ACTIONS::newJunction(
    "chemschema.EditorControl.newJunction",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'J',
    _( "New Junction" ), _( "Create a new junction" ),
    add_junction_xpm, AF_NONE );

TOOL_ACTION CHEMSCHEMA_ACTIONS::newLabel(
    "chemschema.EditorControl.newLabel",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'T',
    _( "New Label" ), _( "Create a new label" ),
    text_xpm, AF_NONE );

TOOL_ACTION CHEMSCHEMA_ACTIONS::newScreen(
    "chemschema.EditorControl.newScreen",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'F',
    _( "New Screen/Filter" ), _( "Create a new screen or filter" ),
    filter_xpm, AF_NONE );

TOOL_ACTION CHEMSCHEMA_ACTIONS::newSheet(
    "chemschema.EditorControl.newSheet",
    AS_GLOBAL, MD_SHIFT + MD_CTRL + 'H',
    _( "New Sheet" ), _( "Create a new hierarchical sheet" ),
    sheet_xpm, AF_NONE );

// Constructor - needed even if empty, to avoid linker errors
CHEMSCHEMA_ACTIONS::CHEMSCHEMA_ACTIONS()
{
} 