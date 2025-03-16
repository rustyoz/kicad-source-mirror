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

#ifndef CHEM_LAYOUT_FRAME_H
#define CHEM_LAYOUT_FRAME_H

#include <kiway_player.h>
#include <eda_base_frame.h>
#include <wx/gdicmn.h>

class CHEM_LAYOUT;
class CHEM_LAYOUT_VIEW;
class wxAuiToolBar;

/**
 * Main frame for the Chemical Reactor Layout editor
 */
class CHEM_LAYOUT_FRAME : public KIWAY_PLAYER
{
public:
    CHEM_LAYOUT_FRAME( KIWAY* aKiway, wxWindow* aParent );
    ~CHEM_LAYOUT_FRAME();

    /**
     * @return the nickname of the frame for the KIWAY.
     */
    wxString GetName() const override
    {
        return wxT( "ChemLayoutFrame" );
    }

    /**
     * Function ToggleToolbar
     * toggles the display state of a toolbar
     */
    void ToggleToolbar( int aId );

    /**
     * Function GetToolbarHeight
     * calculates the height of all the toolbars
     */
    int GetToolbarHeight() const;

    /**
     * Function ReCreateMenuBar
     * Creates the main menu bar
     */
    void ReCreateMenuBar() override;

    /**
     * Function ReCreateHToolbar
     * Creates the main horizontal toolbar
     */
    void ReCreateHToolbar() override;

    /**
     * Function ReCreateVToolbar
     * Creates the left vertical toolbar
     */
    void ReCreateVToolbar() override;

    /**
     * Function ReCreateOptToolbar
     * Creates the options toolbar
     */
    void ReCreateOptToolbar() override;

    /**
     * Function LoadSettings
     * loads common frame parameters from configuration
     */
    void LoadSettings( APP_SETTINGS_BASE* aCfg ) override;

    /**
     * Function SaveSettings
     * saves common frame parameters to configuration
     */
    void SaveSettings( APP_SETTINGS_BASE* aCfg ) override;

    /**
     * Load a chemical layout from a file
     */
    bool LoadChemLayout( const wxString& aFileName );

protected:
    /**
     * Function setupTools
     * creates and initializes the tool manager and tools
     */
    void setupTools();

    /**
     * Returns a default canvas type for the frame.
     */
    EDA_DRAW_PANEL_GAL::GAL_TYPE GetGalCanvas() const;

    // Stores a pointer to the chemical layout model
    CHEM_LAYOUT* m_chemLayout;

    // Stores a pointer to the view
    CHEM_LAYOUT_VIEW* m_view;

    // Toolbar handles
    wxAuiToolBar* m_mainToolBar;
    wxAuiToolBar* m_optionsToolBar;
    wxAuiToolBar* m_drawToolBar;
};

#endif // CHEM_LAYOUT_FRAME_H 