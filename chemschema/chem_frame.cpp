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

#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/aui/auibar.h>
#include <eda_base_frame.h>
#include <tool/tool_manager.h>
#include <tool/action_toolbar.h>
#include <kiface_base.h>

#include "chem_frame.h"
#include "chem_schematic.h"
#include "chem_view.h"

CHEM_FRAME::CHEM_FRAME( KIWAY* aKiway, wxWindow* aParent ) :
        KIWAY_PLAYER( aKiway, aParent, FRAME_CHEM_SCHEMA, wxT( "ChemSchemaFrame" ),
                      wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS,
                      wxT( "ChemSchemaFrame" ) )
{
    m_chemSchematic = nullptr;
    m_view = nullptr;
    m_mainToolBar = nullptr;
    m_optionsToolBar = nullptr;
    m_drawToolBar = nullptr;
    
    // Create the window title
    SetTitle( wxT( "Chemical Process Flow Diagram Editor" ) );
    
    // Create a default schematic
    m_chemSchematic = new CHEM_SCHEMATIC();
    
    // Create the view
    m_view = new CHEM_VIEW( true );
    m_view->SetChemSchematic( m_chemSchematic );
    
    // Setup tools
    setupTools();
    
    // Create the toolbars
    ReCreateMenuBar();
    ReCreateHToolbar();
    ReCreateVToolbar();
    ReCreateOptToolbar();
    
    // Initial window size and position
    SetSizeHints( wxSize( 600, 400 ), wxDefaultSize );
    
    // Load configuration settings
    LoadSettings( nullptr );
}


CHEM_FRAME::~CHEM_FRAME()
{
    delete m_chemSchematic;
    delete m_view;
}


void CHEM_FRAME::setupTools()
{
    // Will be implemented with proper tools later
}


void CHEM_FRAME::ToggleToolbar( int aId )
{
    // Implementation will depend on the toolbars added later
}


int CHEM_FRAME::GetToolbarHeight() const
{
    // Implementation will depend on the toolbars added later
    return 0;
}


void CHEM_FRAME::ReCreateMenuBar()
{
    // Will create a comprehensive menu bar in a future implementation
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append( wxID_NEW, wxT( "&New" ) );
    fileMenu->Append( wxID_OPEN, wxT( "&Open" ) );
    fileMenu->Append( wxID_SAVE, wxT( "&Save" ) );
    fileMenu->Append( wxID_SAVEAS, wxT( "Save &As..." ) );
    fileMenu->AppendSeparator();
    fileMenu->Append( wxID_EXIT, wxT( "E&xit" ) );
    
    menuBar->Append( fileMenu, wxT( "&File" ) );
    
    SetMenuBar( menuBar );
}


void CHEM_FRAME::ReCreateHToolbar()
{
    // Will create a comprehensive toolbar in a future implementation
    if( m_mainToolBar )
        return;
    
    m_mainToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_TEXT );
    
    // Add simple tools for demonstration
    m_mainToolBar->AddTool( wxID_NEW, wxT( "New" ), wxNullBitmap );
    m_mainToolBar->AddTool( wxID_OPEN, wxT( "Open" ), wxNullBitmap );
    m_mainToolBar->AddTool( wxID_SAVE, wxT( "Save" ), wxNullBitmap );
    
    m_mainToolBar->Realize();
    
    // Add to the AUI manager
    m_auimgr.AddPane( m_mainToolBar, EDA_PANE().HToolbar().Name( "MainToolbar" ).Top().Position( 0 ) );
}


void CHEM_FRAME::ReCreateVToolbar()
{
    // Will create a comprehensive vertical toolbar in a future implementation
    m_drawToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                  wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_VERTICAL );
    
    // Add simple tools for demonstration
    m_drawToolBar->AddTool( wxID_ANY, wxT( "Draw Line" ), wxNullBitmap );
    m_drawToolBar->AddTool( wxID_ANY, wxT( "Add Vessel" ), wxNullBitmap );
    m_drawToolBar->AddTool( wxID_ANY, wxT( "Add Pump" ), wxNullBitmap );
    
    m_drawToolBar->Realize();
    
    // Add to the AUI manager
    m_auimgr.AddPane( m_drawToolBar, EDA_PANE().VToolbar().Name( "DrawToolbar" ).Left().Position( 0 ) );
}


void CHEM_FRAME::ReCreateOptToolbar()
{
    // Will create a comprehensive options toolbar in a future implementation
    m_optionsToolBar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                     wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_VERTICAL );
    
    // Add simple tools for demonstration
    m_optionsToolBar->AddTool( wxID_ANY, wxT( "Grid" ), wxNullBitmap );
    m_optionsToolBar->AddTool( wxID_ANY, wxT( "Units" ), wxNullBitmap );
    m_optionsToolBar->AddTool( wxID_ANY, wxT( "Zoom" ), wxNullBitmap );
    
    m_optionsToolBar->Realize();
    
    // Add to the AUI manager
    m_auimgr.AddPane( m_optionsToolBar, EDA_PANE().VToolbar().Name( "OptionsToolbar" ).Right().Position( 0 ) );
}


EDA_DRAW_PANEL_GAL::GAL_TYPE CHEM_FRAME::GetGalCanvas() const
{
    return EDA_DRAW_PANEL_GAL::GAL_TYPE_OPENGL;
}


void CHEM_FRAME::LoadSettings( APP_SETTINGS_BASE* aCfg )
{
    // Will load settings in a future implementation
    // For now, set some default window size and position
    wxSize defaultSize( 900, 700 );
    wxDisplay display( (unsigned) 0 );
    wxRect displaySize = display.GetClientArea();
    
    if( defaultSize.x > displaySize.width )
        defaultSize.x = displaySize.width;
    
    if( defaultSize.y > displaySize.height )
        defaultSize.y = displaySize.height;
    
    SetSize( defaultSize );
    Centre();
}


void CHEM_FRAME::SaveSettings( APP_SETTINGS_BASE* aCfg )
{
    // Will save settings in a future implementation
}


bool CHEM_FRAME::LoadChemSchematic( const wxString& aFileName )
{
    // Will load a chemical schematic from a file in a future implementation
    return false;
} 