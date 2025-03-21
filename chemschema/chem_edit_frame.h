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
#include <wx/filename.h>

class CHEM_SCHEMATIC;
class CHEM_VIEW;
class TOOL_MANAGER;
class CHEM_DISPLAY_OPTIONS;

/**
 * Main frame for the chemical process flow diagram editor
 */
class CHEM_EDIT_FRAME : public KIWAY_PLAYER
{
public:
    /**
     * Constructor
     * @param aKiway - The Kiway instance
     * @param aParent - Parent window
     * @param aFrameType - Frame type
     * @param aTitle - Window title
     * @param aPos - Window position
     * @param aSize - Window size
     * @param aStyle - Window style
     * @param aFrameName - Frame name
     */
    CHEM_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aFrameType,
                    const wxString& aTitle, const wxPoint& aPos = wxDefaultPosition,
                    const wxSize& aSize = wxDefaultSize,
                    long aStyle = KICAD_DEFAULT_DRAWFRAME_STYLE,
                    const wxString& aFrameName = wxT( "ChemSchemaFrame" ) );
    
    /**
     * Destructor
     */
    ~CHEM_EDIT_FRAME();
    
    /**
     * Get the chemical schematic
     * @return Pointer to the chemical schematic
     */
    CHEM_SCHEMATIC* GetChemSchematic() const;
    
    /**
     * Get the chemical view
     * @return Pointer to the chemical view
     */
    CHEM_VIEW* GetChemView() const;
    
    /**
     * Get the tool manager
     * @return Pointer to the tool manager
     */
    TOOL_MANAGER* GetToolManager() const;
    
    /**
     * Get the display options
     * @return Pointer to the display options
     */
    CHEM_DISPLAY_OPTIONS* GetDisplayOptions() const;
    
    /**
     * Create a new chemical schematic
     */
    void NewSchematic();
    
    /**
     * Open a chemical schematic from a file
     * @param aFileName - The file to open
     * @return True if successful, false otherwise
     */
    bool OpenSchematic( const wxString& aFileName );
    
    /**
     * Save the current chemical schematic to a file
     * @param aFileName - The file to save to
     * @return True if successful, false otherwise
     */
    bool SaveSchematic( const wxString& aFileName );
    
    /**
     * Update the view
     */
    void UpdateView();
    
    /**
     * Update the title bar
     */
    void UpdateTitleBar();
    
    /**
     * Update the status bar with current information
     */
    void UpdateStatusBar() override;
    
    /**
     * Update the toolbar
     */
    void UpdateToolbar();
    
    /**
     * Update the menu bar
     */
    void UpdateMenuBar();
    
    /**
     * Set the current file
     * @param aFileName - The file name
     */
    void SetCurrentFile( const wxFileName& aFileName );
    
    /**
     * Get the current file
     * @return The current file
     */
    const wxFileName& GetCurrentFile() const;
    
private:
    CHEM_SCHEMATIC* m_chemSchematic;
    CHEM_VIEW* m_chemView;
    TOOL_MANAGER* m_toolManager;
    CHEM_DISPLAY_OPTIONS* m_displayOptions;
    wxFileName m_currentFile;
};

#endif // CHEM_EDIT_FRAME_H 