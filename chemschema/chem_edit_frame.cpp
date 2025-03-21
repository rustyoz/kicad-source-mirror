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

#include <eda_units.h>

#include "chem_edit_frame.h"
#include "chem_schematic.h"
#include "chem_view.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/statusbr.h>

// For information, the main classes hierarchy for chemschema is:
//
// CHEM_EDIT_FRAME -> KIWAY_PLAYER
// CHEM_EDIT_FRAME has a TOOL_MANAGER
// CHEM_EDIT_FRAME has a CHEM_SCHEMATIC

CHEM_EDIT_FRAME::CHEM_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aFrameType,
                                 const wxString& aTitle, const wxPoint& aPos,
                                 const wxSize& aSize, long aStyle,
                                 const wxString& aFrameName, const EDA_IU_SCALE& aIuScale ) :
    KIWAY_PLAYER( aKiway, aParent, aFrameType, aTitle, aPos, aSize, aStyle, aFrameName, aIuScale )
{
    m_chemSchematic = nullptr;
    m_chemView = nullptr;
    m_displayOptions = nullptr;
    m_toolManager = nullptr;
    m_actions = nullptr;

    // Create the schematic
    m_chemSchematic = new CHEM_SCHEMATIC();
    
    // Create the view
    m_chemView = new CHEM_VIEW( true );
    m_chemView->SetChemSchematic( m_chemSchematic );
    
    // Create the display options
    m_displayOptions = new CHEM_DISPLAY_OPTIONS();

    // Create and initialize the tool manager
    m_toolManager = new TOOL_MANAGER;
    m_toolManager->SetEnvironment( this, nullptr, nullptr, nullptr );

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
    delete m_chemSchematic;
    delete m_chemView;
    delete m_displayOptions;
}


CHEM_SCHEMATIC* CHEM_EDIT_FRAME::GetChemSchematic() const
{
    return m_chemSchematic;
}


CHEM_VIEW* CHEM_EDIT_FRAME::GetChemView() const
{
    return m_chemView;
}


TOOL_MANAGER* CHEM_EDIT_FRAME::GetToolManager() const
{
    return m_toolManager;
}


CHEM_DISPLAY_OPTIONS* CHEM_EDIT_FRAME::GetDisplayOptions() const
{
    return m_displayOptions;
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


void CHEM_EDIT_FRAME::NewSchematic()
{
    // Check if current schematic has been modified
    if( m_chemSchematic->IsModified() )
    {
        wxMessageDialog dlg( this, _( "Save changes to the current schematic?" ),
                           _( "Save Changes" ), wxYES_NO | wxCANCEL | wxICON_QUESTION );
        
        int ret = dlg.ShowModal();
        
        if( ret == wxID_CANCEL )
            return;
        
        if( ret == wxID_YES )
        {
            if( !SaveSchematic( m_currentFile.GetFullPath() ) )
                return;
        }
    }
    
    // Clear the current schematic
    m_chemSchematic->Clear();
    
    // Reset the current file
    m_currentFile.Clear();
    
    // Update the view
    UpdateView();
    
    // Update the title bar
    UpdateTitleBar();
}


bool CHEM_EDIT_FRAME::OpenSchematic( const wxString& aFileName )
{
    // Check if current schematic has been modified
    if( m_chemSchematic->IsModified() )
    {
        wxMessageDialog dlg( this, _( "Save changes to the current schematic?" ),
                           _( "Save Changes" ), wxYES_NO | wxCANCEL | wxICON_QUESTION );
        
        int ret = dlg.ShowModal();
        
        if( ret == wxID_CANCEL )
            return false;
        
        if( ret == wxID_YES )
        {
            if( !SaveSchematic( m_currentFile.GetFullPath() ) )
                return false;
        }
    }
    
    // Clear the current schematic
    m_chemSchematic->Clear();
    
    // Set the current file
    wxFileName fileName( aFileName );
    SetCurrentFile( fileName );
    
    // TODO: Implement actual file loading
    // For now, just create a new schematic
    
    // Update the view
    UpdateView();
    
    // Update the title bar
    UpdateTitleBar();
    
    return true;
}


bool CHEM_EDIT_FRAME::SaveSchematic( const wxString& aFileName )
{
    // If no file name is provided, show a file dialog
    wxString fileName = aFileName;
    
    if( fileName.IsEmpty() )
    {
        wxFileDialog dlg( this, _( "Save Chemical Process Flow Diagram" ), wxEmptyString,
                        wxEmptyString, _( "Chemical Process Flow Diagram Files (*.chem)|*.chem" ),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
        
        if( dlg.ShowModal() == wxID_CANCEL )
            return false;
        
        fileName = dlg.GetPath();
    }
    
    // Set the current file
    wxFileName fn( fileName );
    SetCurrentFile( fn );
    
    // TODO: Implement actual file saving
    // For now, just mark the schematic as not modified
    m_chemSchematic->SetModified( false );
    
    // Update the title bar
    UpdateTitleBar();
    
    return true;
}


void CHEM_EDIT_FRAME::UpdateView()
{
    // Update the view with the current display options
    m_chemView->UpdateDisplayOptions( *m_displayOptions );
}


void CHEM_EDIT_FRAME::UpdateTitleBar()
{
    wxString title;
    
    if( m_currentFile.IsOk() )
    {
        title = m_currentFile.GetFullName();
        
        if( m_chemSchematic->IsModified() )
            title += wxT( " *" );
    }
    else
    {
        title = wxT( "Untitled" );
        
        if( m_chemSchematic->IsModified() )
            title += wxT( " *" );
    }
    
    title += wxT( " - Chemical Process Flow Diagram Editor" );
    
    SetTitle( title );
}


void CHEM_EDIT_FRAME::UpdateStatusBar()
{
    // Update the status bar with current information
    if( GetStatusBar() )
    {
        GetStatusBar()->SetStatusText( _( "Ready" ), 0 );
        
        if( m_currentFile.IsOk() )
            GetStatusBar()->SetStatusText( m_currentFile.GetFullPath(), 1 );
        else
            GetStatusBar()->SetStatusText( _( "Untitled" ), 1 );
    }
}


void CHEM_EDIT_FRAME::UpdateToolbar()
{
    // TODO: Implement toolbar update
}


void CHEM_EDIT_FRAME::UpdateMenuBar()
{
    // TODO: Implement menu bar update
}


void CHEM_EDIT_FRAME::SetCurrentFile( const wxFileName& aFileName )
{
    m_currentFile = aFileName;
    m_chemSchematic->SetFilename( aFileName );
}


const wxFileName& CHEM_EDIT_FRAME::GetCurrentFile() const
{
    return m_currentFile;
} 