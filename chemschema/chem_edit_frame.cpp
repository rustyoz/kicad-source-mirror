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

#include <wx/menu.h>
#include <tool/tool_manager.h>
#include <tool/tool_dispatcher.h>
#include <tool/action_manager.h>
#include <tool/common_tools.h>

#include "chem_edit_frame.h"
#include "chem_schematic.h"

// For information, the main classes hierarchy for chemschema is:
//
// CHEM_EDIT_FRAME -> KIWAY_PLAYER
// CHEM_EDIT_FRAME has a TOOL_MANAGER
// CHEM_EDIT_FRAME has a CHEM_SCHEMATIC

CHEM_EDIT_FRAME::CHEM_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent ) :
        KIWAY_PLAYER( aKiway, aParent, CHEMSCHEMA::FRAME_CHEM_SCHEMA, wxT( "ChemSchema" ),
                     wxDefaultPosition, wxDefaultSize, 
                     wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS, GetCanvasType() )
{
    m_schematic = nullptr;
    m_toolManager = nullptr;
    m_actions = nullptr;

    // Create the schematic view
    m_schematic = new CHEM_SCHEMATIC( this );

    // Create and initialize the tool manager
    m_toolManager = new TOOL_MANAGER;
    m_toolManager->SetEnvironment( m_schematic, nullptr, nullptr, this );

    // Register common tools
    m_actions = new ACTIONS();
    m_toolManager->RegisterTool( new COMMON_TOOLS );
    
    // Create the main window, canvas, toolbars, etc.
    createCanvas();
    createToolbars();
    reCreateMenuBar();
    
    // Set up event handling
    Bind( wxEVT_CLOSE_WINDOW, &CHEM_EDIT_FRAME::OnCloseWindow, this );
    
    // Initialize hotkeys
    setupTools();
    
    // Load settings
    LoadSettings( config() );
    
    // Set initial size
    SetSizeHints( wxSize( 600, 400 ), wxDefaultSize );
    
    // Show window
    Show();
}


CHEM_EDIT_FRAME::~CHEM_EDIT_FRAME()
{
    SaveSettings( config() );
    
    delete m_actions;
    delete m_toolManager;
    delete m_schematic;
}


WINDOW_SETTINGS* CHEM_EDIT_FRAME::GetWindowSettings( APP_SETTINGS_BASE* aCfg )
{
    return &static_cast<CHEMSCHEMA_SETTINGS*>( aCfg )->m_Window;
}


CHEMSCHEMA_SETTINGS& CHEM_EDIT_FRAME::Settings() const
{
    return Kiway().CommonSettings<CHEMSCHEMA_SETTINGS>();
}


void CHEM_EDIT_FRAME::OnCloseWindow( wxCloseEvent& Event )
{
    Event.Skip();
}


void CHEM_EDIT_FRAME::LoadProjectSettings()
{
    // To be implemented
}


void CHEM_EDIT_FRAME::SaveProjectSettings()
{
    // To be implemented
}


void CHEM_EDIT_FRAME::setupUIConditions()
{
    // To be implemented with UI condition logic
}


void CHEM_EDIT_FRAME::reCreateMenuBar()
{
    // Create main menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    
    // File menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append( wxID_NEW, _( "&New Chemical Diagram...\tCtrl+N" ) );
    fileMenu->Append( wxID_OPEN, _( "&Open Chemical Diagram...\tCtrl+O" ) );
    fileMenu->Append( wxID_SAVE, _( "&Save\tCtrl+S" ) );
    fileMenu->Append( wxID_SAVEAS, _( "Save &As...\tCtrl+Shift+S" ) );
    fileMenu->AppendSeparator();
    fileMenu->Append( wxID_EXIT, _( "&Close" ) );
    
    // Edit menu
    wxMenu* editMenu = new wxMenu;
    editMenu->Append( wxID_UNDO, _( "&Undo\tCtrl+Z" ) );
    editMenu->Append( wxID_REDO, _( "&Redo\tCtrl+Y" ) );
    editMenu->AppendSeparator();
    editMenu->Append( wxID_CUT, _( "Cu&t\tCtrl+X" ) );
    editMenu->Append( wxID_COPY, _( "&Copy\tCtrl+C" ) );
    editMenu->Append( wxID_PASTE, _( "&Paste\tCtrl+V" ) );
    editMenu->Append( wxID_DELETE, _( "&Delete\tDel" ) );
    
    // View menu
    wxMenu* viewMenu = new wxMenu;
    viewMenu->Append( ID_ZOOM_IN, _( "Zoom &In\tCtrl++" ) );
    viewMenu->Append( ID_ZOOM_OUT, _( "Zoom &Out\tCtrl+-" ) );
    viewMenu->Append( ID_ZOOM_FIT, _( "Zoom &Fit\tCtrl+0" ) );
    
    // Place menu
    wxMenu* placeMenu = new wxMenu;
    placeMenu->Append( ID_PLACE_SYMBOL, _( "Place &Symbol...\tP" ) );
    placeMenu->Append( ID_PLACE_JUNCTION, _( "Place &Junction\tJ" ) );
    placeMenu->Append( ID_PLACE_LABEL, _( "Place &Label\tL" ) );
    placeMenu->Append( ID_PLACE_TEXT, _( "Place &Text\tT" ) );
    
    // Help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append( wxID_HELP, _( "Chemical Process Flow Diagram &Help" ) );
    helpMenu->Append( wxID_ABOUT, _( "&About ChemSchema" ) );
    
    // Add menus to menubar
    menuBar->Append( fileMenu, _( "&File" ) );
    menuBar->Append( editMenu, _( "&Edit" ) );
    menuBar->Append( viewMenu, _( "&View" ) );
    menuBar->Append( placeMenu, _( "&Place" ) );
    menuBar->Append( helpMenu, _( "&Help" ) );
    
    SetMenuBar( menuBar );
}


void CHEM_EDIT_FRAME::createToolbars()
{
    // To be implemented with toolbar creation logic
}


void CHEM_EDIT_FRAME::initToolbars()
{
    // To be implemented with toolbar initialization logic
}


void CHEM_EDIT_FRAME::setupTools()
{
    // To be implemented with tool setup logic
}


void CHEM_EDIT_FRAME::createCanvas()
{
    // To be implemented with canvas creation logic
} 