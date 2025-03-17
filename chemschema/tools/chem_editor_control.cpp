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
#include <wx/bitmap.h>
#include <tool/tool_manager.h>
#include <tool/tool_menu.h>
#include <tool/tool_event.h>
#include <tool/tool_interactive.h>
#include "../chem_includes.h"
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <confirm.h>

#include "chem_editor_control.h"
#include "../chem_schematic.h"
#include "../chem_symbol.h"
#include "../chem_line.h"
#include "../chem_sheet.h"
#include "../chem_junction.h"
#include "../chem_label.h"
#include "../chem_screen.h"
#include "../chem_draw_panel.h"

using namespace std;

CHEM_EDITOR_CONTROL::CHEM_EDITOR_CONTROL() :
    TOOL_INTERACTIVE( "chemschema.EditorControl" ),
    m_frame( nullptr ),
    m_selectionTool( nullptr ),
    m_menu( nullptr )
{
}


CHEM_EDITOR_CONTROL::~CHEM_EDITOR_CONTROL()
{
    delete m_menu;
}


void CHEM_EDITOR_CONTROL::Reset( RESET_REASON aReason )
{
    if( aReason == MODEL_RELOAD )
    {
        // Nothing to do here
    }
}


int CHEM_EDITOR_CONTROL::Main( const TOOL_EVENT& aEvent )
{
    // Main state machine for the editor control tool
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();
    m_selectionTool = m_toolMgr->GetTool<CHEM_SELECTION_TOOL>();

    // Set up the selection tool's context menu
    SetupContextMenu();

    // Wait for the next event
    TOOL_EVENT evt = waitForEvent();

    if( evt.IsCancel() || evt.IsActivate() )
        return 0;

    return 0;
}


/**
 * Handler for the new symbol command.
 */
int CHEM_EDITOR_CONTROL::NewSymbol( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_SYMBOL* symbol = new CHEM_SYMBOL();
    
    // Set default properties for the new symbol
    symbol->SetPosition( wxPoint( 0, 0 ) );
    symbol->SetSymbolType( CHEM_SYMBOL::CUSTOM );
    symbol->SetName( wxT( "New Symbol" ) );
    
    // Add the new symbol to the schematic
    schematic->Add( symbol );
    
    // Start the interactive placement of the symbol
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the new line command.
 */
int CHEM_EDITOR_CONTROL::NewLine( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_LINE* line = new CHEM_LINE();
    
    // Set default properties for the new line
    line->SetWidth( 10 );
    line->SetLineStyle( CHEM_LINE::SOLID );
    line->SetColor( COLOR4D::BLACK );
    
    // Add the new line to the schematic
    schematic->Add( line );
    
    // Start the interactive placement of the line
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the new junction command.
 */
int CHEM_EDITOR_CONTROL::NewJunction( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_JUNCTION* junction = new CHEM_JUNCTION();
    
    // Set default properties for the new junction
    junction->SetPosition( wxPoint( 0, 0 ) );
    junction->SetJunctionType( CHEM_JUNCTION::NORMAL );
    junction->SetDiameter( 20 );
    junction->SetColor( COLOR4D::BLACK );
    
    // Add the new junction to the schematic
    schematic->Add( junction );
    
    // Start the interactive placement of the junction
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the new label command.
 */
int CHEM_EDITOR_CONTROL::NewLabel( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_LABEL* label = new CHEM_LABEL();
    
    // Set default properties for the new label
    label->SetText( wxT( "Label" ) );
    label->SetLabelType( CHEM_LABEL::INLINE );
    
    // Add the new label to the schematic
    schematic->Add( label );
    
    // Start the interactive placement of the label
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the new screen command.
 */
int CHEM_EDITOR_CONTROL::NewScreen( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_SCREEN* screen = new CHEM_SCREEN();
    
    // Set default properties for the new screen
    screen->SetPosition( VECTOR2I( 0, 0 ) );
    screen->SetScreenType( CHEM_SCREEN::FILTER );
    
    // Add the new screen to the schematic
    schematic->Add( screen );
    
    // Start the interactive placement of the screen
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the new sheet command.
 */
int CHEM_EDITOR_CONTROL::NewSheet( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    CHEM_SHEET* sheet = new CHEM_SHEET();
    
    // Set default properties for the new sheet
    sheet->SetPosition( wxPoint( 0, 0 ) );
    sheet->SetSize( wxSize( 2000, 1500 ) );
    sheet->SetName( wxT( "New Sheet" ) );
    
    // Add the new sheet to the schematic
    schematic->Add( sheet );
    
    // Start the interactive placement of the sheet
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" ) );
    
    return 0;
}


/**
 * Handler for the delete command.
 */
int CHEM_EDITOR_CONTROL::Delete( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Get the selected items
    SELECTION& selection = m_selectionTool->GetSelection();
    
    if( selection.Empty() )
        return 0;
    
    // Ask for confirmation
    if( !IsOK( m_frame, _( "Delete selected items?" ) ) )
        return 0;
    
    // Save the current schematic state for undo
    schematic->SaveCopyInUndoList();
    
    // Delete the selected items
    for( EDA_ITEM* item : selection )
    {
        schematic->Remove( item );
    }
    
    // Clear the selection
    m_toolMgr->RunAction( "chemschema.InteractiveSelection.ClearSelection" );
    
    // Update the view
    getView()->UpdateAllItems( KIGFX::ALL );
    m_frame->OnModify();
    
    return 0;
}


/**
 * Handler for the properties command.
 */
int CHEM_EDITOR_CONTROL::Properties( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Get the selected items
    SELECTION& selection = m_selectionTool->GetSelection();
    
    if( selection.Empty() )
        return 0;
    
    // Only handle one item at a time for properties
    EDA_ITEM* item = selection.Front();
    
    if( !item )
        return 0;
    
    // Save the current schematic state for undo
    schematic->SaveCopyInUndoList();
    
    // Open the appropriate properties dialog based on item type
    bool changed = false;
    
    switch( item->Type() )
    {
    case CHEM_ITEM::CHEM_SYMBOL_T:
        changed = m_frame->EditSymbolProperties( static_cast<CHEM_SYMBOL*>( item ) );
        break;
        
    case CHEM_ITEM::CHEM_LINE_T:
        changed = m_frame->EditLineProperties( static_cast<CHEM_LINE*>( item ) );
        break;
        
    case CHEM_ITEM::CHEM_JUNCTION_T:
        changed = m_frame->EditJunctionProperties( static_cast<CHEM_JUNCTION*>( item ) );
        break;
        
    case CHEM_ITEM::CHEM_LABEL_T:
        changed = m_frame->EditLabelProperties( static_cast<CHEM_LABEL*>( item ) );
        break;
        
    case CHEM_ITEM::CHEM_SCREEN_T:
        changed = m_frame->EditScreenProperties( static_cast<CHEM_SCREEN*>( item ) );
        break;
        
    case CHEM_ITEM::CHEM_SHEET_T:
        changed = m_frame->EditSheetProperties( static_cast<CHEM_SHEET*>( item ) );
        break;
        
    default:
        break;
    }
    
    if( changed )
    {
        // Update the view
        getView()->UpdateAllItems( KIGFX::ALL );
        m_frame->OnModify();
    }
    else
    {
        // If no changes were made, remove the last undo point
        schematic->RollbackFromUndo();
    }
    
    return 0;
}


/**
 * Handler for the rotate command.
 */
int CHEM_EDITOR_CONTROL::Rotate( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Get the selected items
    SELECTION& selection = m_selectionTool->GetSelection();
    
    if( selection.Empty() )
        return 0;
    
    // Save the current schematic state for undo
    schematic->SaveCopyInUndoList();
    
    // Rotate each selected item
    for( EDA_ITEM* item : selection )
    {
        CHEM_ITEM* chemItem = static_cast<CHEM_ITEM*>( item );
        
        switch( chemItem->Type() )
        {
        case CHEM_ITEM::CHEM_SYMBOL_T:
            {
                CHEM_SYMBOL* symbol = static_cast<CHEM_SYMBOL*>( chemItem );
                double angle = symbol->GetRotationAngle();
                symbol->SetRotationAngle( angle + 90.0 );
            }
            break;
            
        case CHEM_ITEM::CHEM_SCREEN_T:
            {
                CHEM_SCREEN* screen = static_cast<CHEM_SCREEN*>( chemItem );
                int angle = screen->GetRotationAngle();
                screen->SetRotationAngle( angle + 900 ); // 90 degrees in tenths of a degree
            }
            break;
            
        case CHEM_ITEM::CHEM_LABEL_T:
            {
                CHEM_LABEL* label = static_cast<CHEM_LABEL*>( chemItem );
                EDA_ANGLE angle = label->GetTextAngle();
                label->SetTextAngle( angle + ANGLE_90 );
            }
            break;
            
        default:
            // Item doesn't support rotation
            break;
        }
    }
    
    // Update the view
    getView()->UpdateAllItems( KIGFX::ALL );
    m_frame->OnModify();
    
    return 0;
}


void CHEM_EDITOR_CONTROL::SetupContextMenu()
{
    // Create a new context menu and add all the menu entries
    if( m_menu )
        delete m_menu;
    
    m_menu = new TOOL_MENU;
    
    // Add the selection tool actions
    m_menu->Add( m_selectionTool->GetToolMenuItems() );
    
    // Add separator
    m_menu->AddSeparator();
    
    // Add our own actions
    m_menu->Add( ACTIONS::copy );
    m_menu->Add( ACTIONS::cut );
    m_menu->Add( ACTIONS::paste );
    
    m_menu->AddSeparator();
    
    m_menu->Add( ACTIONS::rotate, SELECTION_CONDITIONS::NotEmpty );
    m_menu->Add( ACTIONS::flip, SELECTION_CONDITIONS::NotEmpty );
    
    m_menu->AddSeparator();
    
    m_menu->Add( ACTIONS::properties, SELECTION_CONDITIONS::NotEmpty );
    m_menu->Add( ACTIONS::deleteItemCursor, SELECTION_CONDITIONS::NotEmpty );
    
    // Set the selection tool's context menu
    m_selectionTool->SetContextMenu( m_menu );
}


void CHEM_EDITOR_CONTROL::setTransitions()
{
    Go( &CHEM_EDITOR_CONTROL::Main,        EVENTS::SelectedEvent );
    Go( &CHEM_EDITOR_CONTROL::NewSymbol,   CHEMSCHEMA_ACTIONS::newSymbol.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::NewLine,     CHEMSCHEMA_ACTIONS::newLine.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::NewJunction, CHEMSCHEMA_ACTIONS::newJunction.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::NewLabel,    CHEMSCHEMA_ACTIONS::newLabel.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::NewScreen,   CHEMSCHEMA_ACTIONS::newScreen.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::NewSheet,    CHEMSCHEMA_ACTIONS::newSheet.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::Delete,      ACTIONS::doDelete.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::Properties,  ACTIONS::properties.MakeEvent() );
    Go( &CHEM_EDITOR_CONTROL::Rotate,      ACTIONS::rotate.MakeEvent() );
} 