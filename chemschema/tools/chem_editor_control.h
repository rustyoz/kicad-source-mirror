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

#ifndef CHEM_EDITOR_CONTROL_H
#define CHEM_EDITOR_CONTROL_H

#include <tool/tool_interactive.h>
#include <tool/tool_menu.h>

class CHEM_EDIT_FRAME;
class CHEM_SELECTION_TOOL;  // Forward declaration

/**
 * CHEM_EDITOR_ACTIONS
 * Gathers all the actions that are shared by tools. The instance of
 * CHEM_EDITOR_ACTIONS is created inside the CHEM_EDIT_FRAME (the main 
 * chemical schematic editor frame).
 */
class CHEM_EDITOR_ACTIONS
{
public:
    // Constructor
    CHEM_EDITOR_ACTIONS();

    // Drawing tool actions
    static TOOL_ACTION newSymbol;
    static TOOL_ACTION newLine;
    static TOOL_ACTION newJunction;
    static TOOL_ACTION newLabel;
    static TOOL_ACTION newScreen;
    static TOOL_ACTION newSheet;
};

/**
 * CHEM_EDITOR_CONTROL
 * Main control tool for the chemical schematic editor.
 * Handles most of the commands shared between tools (zooming, view controls, etc.),
 * initialization of tools and user interactions.
 */
class CHEM_EDITOR_CONTROL : public TOOL_INTERACTIVE
{
public:
    /**
     * Constructor
     */
    CHEM_EDITOR_CONTROL();

    /**
     * Destructor
     */
    ~CHEM_EDITOR_CONTROL();

    /**
     * Get the name of the tool
     * @return Tool name
     */
    const std::string& GetName() const;

    /**
     * Main loop for this tool
     * @param aEvent - Tool activation event
     */
    int Main( const TOOL_EVENT& aEvent );

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
     * Handler for the new symbol command
     * @param aEvent - Tool event
     */
    int NewSymbol( const TOOL_EVENT& aEvent );

    /**
     * Handler for the new line command
     * @param aEvent - Tool event
     */
    int NewLine( const TOOL_EVENT& aEvent );

    /**
     * Handler for the new junction command
     * @param aEvent - Tool event
     */
    int NewJunction( const TOOL_EVENT& aEvent );

    /**
     * Handler for the new label command
     * @param aEvent - Tool event
     */
    int NewLabel( const TOOL_EVENT& aEvent );

    /**
     * Handler for the new screen command
     * @param aEvent - Tool event
     */
    int NewScreen( const TOOL_EVENT& aEvent );

    /**
     * Handler for the new sheet command
     * @param aEvent - Tool event
     */
    int NewSheet( const TOOL_EVENT& aEvent );

    /**
     * Handler for the delete command
     * @param aEvent - Tool event
     */
    int Delete( const TOOL_EVENT& aEvent );

    /**
     * Handler for the properties command
     * @param aEvent - Tool event
     */
    int Properties( const TOOL_EVENT& aEvent );

    /**
     * Handler for the rotate command
     * @param aEvent - Tool event
     */
    int Rotate( const TOOL_EVENT& aEvent );

private:
    /**
     * Setup and update the context menu
     */
    void SetupContextMenu();

    // Private members
    CHEM_EDIT_FRAME*      m_frame;
    CHEM_SELECTION_TOOL*  m_selectionTool;
    TOOL_MENU*            m_menu;
};

#endif // CHEM_EDITOR_CONTROL_H 