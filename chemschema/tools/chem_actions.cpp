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

#include "tools/chem_actions.h"

#include <bitmaps.h>
#include <tool/tool_action.h>

// Actions, being statically-defined, require specialized I18N handling.  We continue to
// use the _() macro so that string harvesting by the I18N framework doesn't have to be
// specialized, but we don't translate on initialization and instead do it in the getters.

#undef _
#define _(s) s

// Selection Tool
TOOL_ACTION CHEM_ACTIONS::selectionActivate( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection" )
        .Scope( AS_GLOBAL )
        .Flags( AF_ACTIVATE ) );

TOOL_ACTION CHEM_ACTIONS::clearSelection( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.clearSelection" )
        .Scope( AS_GLOBAL )
        .FriendlyName( _( "Clear Selection" ) )
        .Tooltip( _( "Deselect all items" ) )
        .Icon( BITMAPS::cursor ) );

TOOL_ACTION CHEM_ACTIONS::addItemToSel( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.addItemToSel" )
        .Scope( AS_GLOBAL ) );

TOOL_ACTION CHEM_ACTIONS::removeItemFromSel( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.removeItemFromSel" )
        .Scope( AS_GLOBAL ) );

TOOL_ACTION CHEM_ACTIONS::addItemsToSel( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.addItemsToSel" )
        .Scope( AS_GLOBAL ) );

TOOL_ACTION CHEM_ACTIONS::removeItemsFromSel( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.removeItemsFromSel" )
        .Scope( AS_GLOBAL ) );

TOOL_ACTION CHEM_ACTIONS::selectionMenu( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveSelection.selectionMenu" )
        .Scope( AS_GLOBAL ) );

// Basic editing
TOOL_ACTION CHEM_ACTIONS::moveSelected( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveEdit.moveSelected" )
        .Scope( AS_GLOBAL )
        .FriendlyName( _( "Move Selected" ) )
        .Tooltip( _( "Move selected items" ) )
        .Icon( BITMAPS::move ) );

TOOL_ACTION CHEM_ACTIONS::deleteSelected( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveEdit.deleteSelected" )
        .Scope( AS_GLOBAL )
        .FriendlyName( _( "Delete Selected" ) )
        .Tooltip( _( "Delete selected items" ) )
        .Icon( BITMAPS::delete ) );

TOOL_ACTION CHEM_ACTIONS::duplicateSelected( TOOL_ACTION_ARGS()
        .Name( "chemschema.InteractiveEdit.duplicateSelected" )
        .Scope( AS_GLOBAL )
        .FriendlyName( _( "Duplicate Selected" ) )
        .Tooltip( _( "Duplicate selected items" ) )
        .Icon( BITMAPS::duplicate ) );

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