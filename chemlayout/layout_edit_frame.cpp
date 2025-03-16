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

#include "layout_edit_frame.h"
#include "chem_layout.h"
#include "layout_view.h"
#include <tool/tool_manager.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/statusbr.h>

LAYOUT_EDIT_FRAME::LAYOUT_EDIT_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aFrameType,
                                     const wxString& aTitle, const wxPoint& aPos,
                                     const wxSize& aSize, long aStyle,
                                     const wxString& aFrameName ) :
    KIWAY_PLAYER( aKiway, aParent, aFrameType, aTitle, aPos, aSize, aStyle, aFrameName )
{
    // Create the layout
    m_chemLayout = new CHEM_LAYOUT();
    
    // Create the view
    m_layoutView = new LAYOUT_VIEW( false );
    m_layoutView->SetChemLayout( m_chemLayout );
    
    // Create the display options
    m_displayOptions = new LAYOUT_DISPLAY_OPTIONS();
    
    // Create the tool manager
    m_toolManager = new TOOL_MANAGER;
    m_toolManager->SetEnvironment( this, nullptr, nullptr, nullptr );
    
    // Create status bar
    CreateStatusBar( 3 );
    
    // Set up the frame
    UpdateTitleBar();
    UpdateStatusBar();
    
    // Set minimum size
    SetMinSize( wxSize( 800, 600 ) );
    
    // Center on parent
    CenterOnParent();
}


LAYOUT_EDIT_FRAME::~LAYOUT_EDIT_FRAME()
{
    delete m_chemLayout;
    delete m_layoutView;
    delete m_displayOptions;
    delete m_toolManager;
}


CHEM_LAYOUT* LAYOUT_EDIT_FRAME::GetChemLayout() const
{
    return m_chemLayout;
}


LAYOUT_VIEW* LAYOUT_EDIT_FRAME::GetLayoutView() const
{
    return m_layoutView;
}


TOOL_MANAGER* LAYOUT_EDIT_FRAME::GetToolManager() const
{
    return m_toolManager;
}


LAYOUT_DISPLAY_OPTIONS* LAYOUT_EDIT_FRAME::GetDisplayOptions() const
{
    return m_displayOptions;
}


void LAYOUT_EDIT_FRAME::NewLayout()
{
    // Check if current layout has been modified
    if( m_chemLayout->IsModified() )
    {
        wxMessageDialog dlg( this, _( "Save changes to the current layout?" ),
                           _( "Save Changes" ), wxYES_NO | wxCANCEL | wxICON_QUESTION );
        
        int ret = dlg.ShowModal();
        
        if( ret == wxID_CANCEL )
            return;
        
        if( ret == wxID_YES )
        {
            if( !SaveLayout( m_currentFile.GetFullPath() ) )
                return;
        }
    }
    
    // Clear the current layout
    m_chemLayout->Clear();
    
    // Reset the current file
    m_currentFile.Clear();
    
    // Update the view
    UpdateView();
    
    // Update the title bar
    UpdateTitleBar();
}


bool LAYOUT_EDIT_FRAME::OpenLayout( const wxString& aFileName )
{
    // Check if current layout has been modified
    if( m_chemLayout->IsModified() )
    {
        wxMessageDialog dlg( this, _( "Save changes to the current layout?" ),
                           _( "Save Changes" ), wxYES_NO | wxCANCEL | wxICON_QUESTION );
        
        int ret = dlg.ShowModal();
        
        if( ret == wxID_CANCEL )
            return false;
        
        if( ret == wxID_YES )
        {
            if( !SaveLayout( m_currentFile.GetFullPath() ) )
                return false;
        }
    }
    
    // Clear the current layout
    m_chemLayout->Clear();
    
    // Set the current file
    wxFileName fileName( aFileName );
    SetCurrentFile( fileName );
    
    // TODO: Implement actual file loading
    // For now, just create a new layout
    
    // Update the view
    UpdateView();
    
    // Update the title bar
    UpdateTitleBar();
    
    return true;
}


bool LAYOUT_EDIT_FRAME::SaveLayout( const wxString& aFileName )
{
    // If no file name is provided, show a file dialog
    wxString fileName = aFileName;
    
    if( fileName.IsEmpty() )
    {
        wxFileDialog dlg( this, _( "Save Chemical Reactor Layout" ), wxEmptyString,
                        wxEmptyString, _( "Chemical Reactor Layout Files (*.lay)|*.lay" ),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
        
        if( dlg.ShowModal() == wxID_CANCEL )
            return false;
        
        fileName = dlg.GetPath();
    }
    
    // Set the current file
    wxFileName fn( fileName );
    SetCurrentFile( fn );
    
    // TODO: Implement actual file saving
    // For now, just mark the layout as not modified
    m_chemLayout->SetModified( false );
    
    // Update the title bar
    UpdateTitleBar();
    
    return true;
}


void LAYOUT_EDIT_FRAME::UpdateView()
{
    // Update the view with the current display options
    m_layoutView->UpdateDisplayOptions( *m_displayOptions );
}


void LAYOUT_EDIT_FRAME::UpdateTitleBar()
{
    wxString title;
    
    if( m_currentFile.IsOk() )
    {
        title = m_currentFile.GetFullName();
        
        if( m_chemLayout->IsModified() )
            title += wxT( " *" );
    }
    else
    {
        title = wxT( "Untitled" );
        
        if( m_chemLayout->IsModified() )
            title += wxT( " *" );
    }
    
    title += wxT( " - Chemical Reactor Layout Editor" );
    
    SetTitle( title );
}


void LAYOUT_EDIT_FRAME::UpdateStatusBar()
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


void LAYOUT_EDIT_FRAME::UpdateToolbar()
{
    // TODO: Implement toolbar update
}


void LAYOUT_EDIT_FRAME::UpdateMenuBar()
{
    // TODO: Implement menu bar update
}


void LAYOUT_EDIT_FRAME::SetCurrentFile( const wxFileName& aFileName )
{
    m_currentFile = aFileName;
    m_chemLayout->SetFilename( aFileName );
}


const wxFileName& LAYOUT_EDIT_FRAME::GetCurrentFile() const
{
    return m_currentFile;
} 