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

#ifndef CHEM_EDIT_FRAME_H
#define CHEM_EDIT_FRAME_H

#include <kiway_player.h>
#include "chemschema_settings.h"

class ACTION_MENU;
class TOOL_MANAGER;
class TOOL_DISPATCHER;
class ACTIONS;
class CHEM_SCHEMATIC;

/**
 * Main editing frame for chemical process flow diagrams
 */
class CHEM_EDIT_FRAME : public KIWAY_PLAYER
{
public:
    CHEM_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent );
    ~CHEM_EDIT_FRAME();

    // Frame identification
    static const wxChar* GetCanvasType() { return wxT( "chem_canvas" ); }
    
    WINDOW_SETTINGS* GetWindowSettings( APP_SETTINGS_BASE* aCfg ) override;

    /**
     * Load project settings from project (.pro) file
     */
    void LoadProjectSettings();

    /**
     * Save project settings to project (.pro) file
     */
    void SaveProjectSettings();

    /**
     * Return the CHEMSCHEMA_SETTINGS for this project
     */
    CHEMSCHEMA_SETTINGS& Settings() const;

    void OnCloseWindow( wxCloseEvent& Event );

protected:
    /**
     * Create the main toolbar
     */
    void setupUIConditions() override;
    void reCreateMenuBar() override;
    void createToolbars();
    
private:
    /**
     * Initialize the toolbar without actually recreating it
     */
    void initToolbars();

    /**
     * Set up the application-wide hotkeys and tool bindings
     */
    void setupTools();

    /**
     * Create and show the chemical process flow diagram canvas
     */
    void createCanvas();

    CHEM_SCHEMATIC*  m_schematic;
    TOOL_MANAGER*    m_toolManager;
    ACTIONS*         m_actions;
};

#endif // CHEM_EDIT_FRAME_H 