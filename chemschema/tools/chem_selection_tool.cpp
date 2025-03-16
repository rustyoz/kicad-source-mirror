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

#include <wx/panel.h>
#include <wx/menu.h>
#include <tool/tool_manager.h>
#include <tool/action_menu.h>
#include <tool/tool_event.h>
#include <tool/common_tools.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <geometry/seg.h>
#include <collectors.h>
#include <confirm.h>

#include "chem_selection_tool.h"
#include "chem_edit_frame.h"
#include "chem_schematic.h"
#include "chem_item.h"
#include "chem_symbol.h"
#include "chem_line.h"
#include "chem_sheet.h"
#include "chem_junction.h"
#include "chem_label.h"
#include "chem_screen.h"

using namespace std;

CHEM_SELECTION_TOOL::CHEM_SELECTION_TOOL() :
    TOOL_INTERACTIVE( "chemschema.InteractiveSelection" ),
    m_frame( nullptr ),
    m_menu( nullptr ),
    m_additive( false ),
    m_subtractive( false ),
    m_multiple( false ),
    m_skipMenuEvent( false )
{
}


CHEM_SELECTION_TOOL::~CHEM_SELECTION_TOOL()
{
}


void CHEM_SELECTION_TOOL::Reset( RESET_REASON aReason )
{
    if( aReason == MODEL_RELOAD )
    {
        // Clear selection when reloading the schematic
        m_selection.Clear();
    }
}


int CHEM_SELECTION_TOOL::Main( const TOOL_EVENT& aEvent )
{
    // Main loop: keep receiving events
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();
    
    // Reset flags
    m_multiple = false;
    m_additive = false;
    m_subtractive = false;
    
    // Set initial cursor
    getViewControls()->ShowCursor( true );
    getViewControls()->SetSnapping( true );
    
    while( TOOL_EVENT* evt = Wait() )
    {
        // Handle certain events specially
        if( evt->IsCancel() || evt->IsActivate() )
        {
            if( evt->IsCancel() )
                ClearSelection();
                
            if( evt->IsActivate() && !evt->IsMoveTool() )
                ClearSelection();
                
            break;
        }
        
        // Handle cursor movement and view controls
        else if( evt->IsMotion() )
        {
            // Update cursor position
            getViewControls()->ForceCursorPosition( false );
        }
        
        // Handle mouse buttons
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Handle left click for selection
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            // Check modifier keys
            m_additive = wxGetKeyState( WXK_SHIFT );
            m_subtractive = wxGetKeyState( WXK_CONTROL );
            
            if( !m_additive && !m_subtractive && !m_selection.Empty() )
                ClearSelection();
                
            // Handle the click on items
            HandleClick( pos, *evt, true );
        }
        
        // Handle right-click for context menu
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            // Select item if not yet selected
            bool selectionModified = false;
            
            if( m_selection.Empty() )
            {
                selectionModified = HandleClick( pos, *evt, true );
            }
            
            // Show context menu if we have a selection or if the module has one
            if( !m_selection.Empty() && m_menu )
            {
                m_skipMenuEvent = false;
                m_menu->SetDirty();
                m_menu->ShowContextMenu( m_selection );
                m_skipMenuEvent = true;
            }
            
            if( selectionModified )
                m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
        }
        
        // Handle selection drag
        else if( evt->IsDrag( BUT_LEFT ) )
        {
            // Begin drag selection
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            if( m_selection.Empty() || !m_selection.Contains( pos ) )
            {
                // Start drag selection
                m_dragStartPos = pos;
                SelectMultiple( *evt );
            }
            else
            {
                // Begin moving selected items
                m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveMove" ) );
            }
        }
        
        // Forward unhandled events to parent
        else
        {
            evt->SetPassEvent();
        }
        
        // Process the current event again
        if( evt->IsAction( &ACTIONS::cut ) ||
            evt->IsAction( &ACTIONS::copy ) ||
            evt->IsAction( &ACTIONS::paste ) ||
            evt->IsAction( &ACTIONS::selectAll ) ||
            evt->IsAction( &ACTIONS::doDelete ) ||
            evt->IsAction( &ACTIONS::duplicate ) )
        {
            m_toolMgr->RunAction( *evt );
        }
    }
    
    return 0;
}


int CHEM_SELECTION_TOOL::SelectSingle( const TOOL_EVENT& aEvent )
{
    VECTOR2I pos = getViewControls()->GetCursorPosition();
    
    m_additive = wxGetKeyState( WXK_SHIFT );
    m_subtractive = wxGetKeyState( WXK_CONTROL );
    
    if( !m_additive && !m_subtractive && !m_selection.Empty() )
        ClearSelection();
        
    // Handle the click on an item
    bool modified = HandleClick( pos, aEvent, true );
    
    return modified ? 1 : 0;
}


int CHEM_SELECTION_TOOL::SelectMultiple( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    if( !schematic )
        return 0;
        
    // Reset flags
    m_multiple = true;
    m_additive = wxGetKeyState( WXK_SHIFT );
    m_subtractive = wxGetKeyState( WXK_CONTROL );
    
    if( !m_additive && !m_subtractive && !m_selection.Empty() )
        ClearSelection();
        
    // Get initial position
    VECTOR2I dragPos = m_dragStartPos;
    VECTOR2I originalPos = dragPos;
    
    // Set initial selection box
    getViewControls()->SetAutoPan( true );
    
    // Set initial cursor
    getViewControls()->ShowCursor( true );
    
    // Draw the selection box
    KIGFX::VIEW* view = getView();
    
    BOX2I selectionBox; // Current selection box
    std::vector<EDA_ITEM*> items; // Items to select
    
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() || evt->IsClick( BUT_RIGHT ) )
            break;
            
        // Handle mouse movement
        if( evt->IsMotion() || evt->IsDrag( BUT_LEFT ) )
        {
            // Update cursor position
            dragPos = getViewControls()->GetCursorPosition();
            
            // Update selection box
            selectionBox.SetOrigin( std::min( originalPos.x, dragPos.x ),
                                    std::min( originalPos.y, dragPos.y ) );
            selectionBox.SetEnd( std::max( originalPos.x, dragPos.x ),
                                  std::max( originalPos.y, dragPos.y ) );
            
            // Draw selection box
            view->SetVisible( &selectionBox, true );
            view->Update( &selectionBox );
        }
        
        // Handle button release to end selection
        else if( evt->IsMouseUp( BUT_LEFT ) )
        {
            // Find all items in the selection box
            items.clear();
            
            // Check all items in the schematic
            for( EDA_ITEM* item : schematic->Items() )
            {
                if( CHEM_ITEM* chemItem = dynamic_cast<CHEM_ITEM*>( item ) )
                {
                    if( chemItem->HitTest( selectionBox, false ) )
                        items.push_back( item );
                }
            }
            
            // Update the selection
            if( m_subtractive )
            {
                // Remove items from selection
                for( EDA_ITEM* item : items )
                    RemoveItemFromSel( item );
            }
            else
            {
                // Add items to selection
                for( EDA_ITEM* item : items )
                    AddItemToSel( item );
            }
            
            // Hide selection box
            view->SetVisible( &selectionBox, false );
            view->Update( &selectionBox );
            
            // Finish selection
            break;
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    if( !m_selection.Empty() )
        m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
        
    return 0;
}


int CHEM_SELECTION_TOOL::ClearSelection( const TOOL_EVENT& aEvent )
{
    ClearSelection();
    return 0;
}


int CHEM_SELECTION_TOOL::SelectAll( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    if( !schematic )
        return 0;
        
    ClearSelection();
    
    std::vector<EDA_ITEM*> items;
    
    // Get all items in the schematic
    for( EDA_ITEM* item : schematic->Items() )
    {
        items.push_back( item );
    }
    
    // Select all items
    SelectItems( items );
    
    m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
    
    return items.size();
}


void CHEM_SELECTION_TOOL::AddItemToSel( EDA_ITEM* aItem, bool aQuietMode )
{
    if( aItem && !m_selection.Contains( aItem ) )
    {
        m_selection.Add( aItem );
        
        if( !aQuietMode )
            HighlightSelection();
    }
}


void CHEM_SELECTION_TOOL::RemoveItemFromSel( EDA_ITEM* aItem, bool aQuietMode )
{
    if( aItem && m_selection.Contains( aItem ) )
    {
        m_selection.Remove( aItem );
        
        if( !aQuietMode )
            HighlightSelection();
    }
}


void CHEM_SELECTION_TOOL::ClearSelection( bool aQuietMode )
{
    if( m_selection.Empty() )
        return;
        
    m_selection.Clear();
    
    if( !aQuietMode )
        HighlightSelection();
}


void CHEM_SELECTION_TOOL::SelectItems( std::vector<EDA_ITEM*>& aItems )
{
    for( EDA_ITEM* item : aItems )
    {
        AddItemToSel( item, true );
    }
    
    HighlightSelection();
}


void CHEM_SELECTION_TOOL::HighlightSelection()
{
    KIGFX::VIEW* view = getView();
    
    // Highlight selected items
    for( EDA_ITEM* item : m_selection )
    {
        view->SetSelected( item );
    }
    
    view->UpdateAllItems( KIGFX::SELECTION );
}


bool CHEM_SELECTION_TOOL::HandleClick( const VECTOR2I& aPosition, const TOOL_EVENT& aEvent, 
                                       bool aAllowDisambiguation )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    if( !schematic )
        return false;
        
    std::vector<EDA_ITEM*> items;
    
    // Check all items in the schematic
    for( EDA_ITEM* item : schematic->Items() )
    {
        if( CHEM_ITEM* chemItem = dynamic_cast<CHEM_ITEM*>( item ) )
        {
            if( chemItem->HitTest( aPosition, getView()->ToScreen( 5.0 ) ) )
                items.push_back( item );
        }
    }
    
    // No items found
    if( items.empty() )
        return false;
        
    // Handle case with multiple items
    EDA_ITEM* item = nullptr;
    
    if( items.size() > 1 && aAllowDisambiguation )
    {
        item = disambiguateItem( items, aPosition );
    }
    else
    {
        item = items[0];
    }
    
    if( !item )
        return false;
        
    // Toggle item selection
    if( m_subtractive || ( m_additive && m_selection.Contains( item ) ) )
    {
        RemoveItemFromSel( item );
    }
    else
    {
        AddItemToSel( item );
    }
    
    return true;
}


EDA_ITEM* CHEM_SELECTION_TOOL::disambiguateItem( const std::vector<EDA_ITEM*>& aItems, 
                                                const VECTOR2I& aPosition )
{
    // If there's only one item, return it directly
    if( aItems.size() <= 1 )
        return aItems.empty() ? nullptr : aItems[0];
        
    // Create a disambiguation menu
    ACTION_MENU disambiguationMenu;
    disambiguationMenu.SetTitle( _( "Disambiguation" ) );
    
    for( size_t i = 0; i < aItems.size(); ++i )
    {
        EDA_ITEM* item = aItems[i];
        CHEM_ITEM* chemItem = dynamic_cast<CHEM_ITEM*>( item );
        
        if( !chemItem )
            continue;
            
        wxString text;
        BITMAPS icon = BITMAPS::add_component;
        
        // Get specific text and icon for each item type
        switch( chemItem->Type() )
        {
        case CHEM_ITEM::CHEM_SYMBOL_T:
            {
                CHEM_SYMBOL* symbol = static_cast<CHEM_SYMBOL*>( chemItem );
                text = wxString::Format( _( "Symbol: %s" ), symbol->GetName() );
                icon = BITMAPS::symbol;
            }
            break;
            
        case CHEM_ITEM::CHEM_LINE_T:
            text = _( "Line" );
            icon = BITMAPS::add_line;
            break;
            
        case CHEM_ITEM::CHEM_JUNCTION_T:
            text = _( "Junction" );
            icon = BITMAPS::add_junction;
            break;
            
        case CHEM_ITEM::CHEM_LABEL_T:
            {
                CHEM_LABEL* label = static_cast<CHEM_LABEL*>( chemItem );
                text = wxString::Format( _( "Label: %s" ), label->ShortenedText() );
                icon = BITMAPS::text;
            }
            break;
            
        case CHEM_ITEM::CHEM_SCREEN_T:
            {
                CHEM_SCREEN* screen = static_cast<CHEM_SCREEN*>( chemItem );
                text = wxString::Format( _( "Filter: %s" ), screen->GetName() );
                icon = BITMAPS::filter;
            }
            break;
            
        case CHEM_ITEM::CHEM_SHEET_T:
            {
                CHEM_SHEET* sheet = static_cast<CHEM_SHEET*>( chemItem );
                text = wxString::Format( _( "Sheet: %s" ), sheet->GetName() );
                icon = BITMAPS::sheet;
            }
            break;
            
        default:
            text = _( "Item" );
            break;
        }
        
        // Add menu item with lambda to capture the selected item
        disambiguationMenu.Add( text, "", -1, icon, [item]( const TOOL_EVENT& ) { return item; } );
    }
    
    // Run the disambiguation menu
    disambiguationMenu.SetIcon( BITMAPS::info );
    disambiguationMenu.DisplayTitle( true );
    disambiguationMenu.SetHandler( [this]( const TOOL_EVENT& evt ) {
        // Handler for selection from menu
        if( evt.IsSelected() )
            return (EDA_ITEM*) evt.Parameter<intptr_t>();
            
        return (EDA_ITEM*) nullptr;
    } );
    
    EDA_ITEM* chosen = disambiguationMenu.ShowContextMenu( aPosition );
    return chosen;
}


int CHEM_SELECTION_TOOL::MoveSelected( const TOOL_EVENT& aEvent )
{
    if( m_selection.Empty() )
        return 0;
        
    // This would be implemented in a separate interactive move tool
    // Here we just forward to that tool
    m_toolMgr->ProcessEvent( TOOL_EVENT( TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveMove" ) );
    
    return 0;
}


int CHEM_SELECTION_TOOL::ContextMenu( const TOOL_EVENT& aEvent )
{
    if( m_skipMenuEvent )
        return 0;
        
    if( m_selection.Empty() )
        return 0;
        
    if( m_menu )
    {
        m_menu->ShowContextMenu( m_selection );
    }
    
    return 0;
}


TOOL_MENU_ITEMS CHEM_SELECTION_TOOL::GetToolMenuItems()
{
    TOOL_MENU_ITEMS items;
    
    items.push_back( ACTIONS::selectAll );
    items.push_back( ACTIONS::unselectAll );
    
    return items;
}


void CHEM_SELECTION_TOOL::setTransitions()
{
    Go( &CHEM_SELECTION_TOOL::Main,                  EVENTS::SelectedEvent );
    Go( &CHEM_SELECTION_TOOL::Main,                  EVENTS::UnselectedEvent );
    Go( &CHEM_SELECTION_TOOL::Main,                  EVENTS::ClearedEvent );
    Go( &CHEM_SELECTION_TOOL::SelectSingle,          EVENTS::SelectedEvent );
    Go( &CHEM_SELECTION_TOOL::SelectMultiple,        EVENTS::SelectedEvent );
    
    Go( &CHEM_SELECTION_TOOL::SelectAll,            ACTIONS::selectAll.MakeEvent() );
    Go( &CHEM_SELECTION_TOOL::ClearSelection,       ACTIONS::unselectAll.MakeEvent() );
    Go( &CHEM_SELECTION_TOOL::ClearSelection,        TC_COMMAND, TA_CANCEL, "" );
    
    Go( &CHEM_SELECTION_TOOL::ContextMenu,           TC_COMMAND, TA_CONTEXT_MENU, "" );
} 