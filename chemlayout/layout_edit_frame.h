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

#ifndef LAYOUT_EDIT_FRAME_H
#define LAYOUT_EDIT_FRAME_H

#include <kiway_player.h>
#include <wx/filename.h>

class CHEM_LAYOUT;
class LAYOUT_VIEW;
class TOOL_MANAGER;
class LAYOUT_DISPLAY_OPTIONS;

/**
 * Main frame for the chemical reactor layout editor
 */
class LAYOUT_EDIT_FRAME : public KIWAY_PLAYER
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
    LAYOUT_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aFrameType,
                      const wxString& aTitle, const wxPoint& aPos = wxDefaultPosition,
                      const wxSize& aSize = wxDefaultSize,
                      long aStyle = KICAD_DEFAULT_DRAWFRAME_STYLE,
                      const wxString& aFrameName = wxT( "ChemLayoutFrame" ) );
    
    /**
     * Destructor
     */
    ~LAYOUT_EDIT_FRAME();
    
    /**
     * Get the chemical layout
     * @return Pointer to the chemical layout
     */
    CHEM_LAYOUT* GetChemLayout() const;
    
    /**
     * Get the layout view
     * @return Pointer to the layout view
     */
    LAYOUT_VIEW* GetLayoutView() const;
    
    /**
     * Get the tool manager
     * @return Pointer to the tool manager
     */
    TOOL_MANAGER* GetToolManager() const;
    
    /**
     * Get the display options
     * @return Pointer to the display options
     */
    LAYOUT_DISPLAY_OPTIONS* GetDisplayOptions() const;
    
    /**
     * Create a new chemical layout
     */
    void NewLayout();
    
    /**
     * Open a chemical layout from a file
     * @param aFileName - The file to open
     * @return True if successful, false otherwise
     */
    bool OpenLayout( const wxString& aFileName );
    
    /**
     * Save the current chemical layout to a file
     * @param aFileName - The file to save to
     * @return True if successful, false otherwise
     */
    bool SaveLayout( const wxString& aFileName );
    
    /**
     * Update the view
     */
    void UpdateView();
    
    /**
     * Update the title bar
     */
    void UpdateTitleBar();
    
    /**
     * Update the status bar
     */
    void UpdateStatusBar();
    
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
    CHEM_LAYOUT* m_chemLayout;
    LAYOUT_VIEW* m_layoutView;
    TOOL_MANAGER* m_toolManager;
    LAYOUT_DISPLAY_OPTIONS* m_displayOptions;
    wxFileName m_currentFile;
};

#endif // LAYOUT_EDIT_FRAME_H 