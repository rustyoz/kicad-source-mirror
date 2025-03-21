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
#include <eda_units.h>
#include <tool/tool_manager.h>
#include <tool/action_menu.h>
#include <tool/tool_event.h>
#include <tool/tool_action.h>
#include <tool/common_tools.h>
#include <tool/tool_interactive.h>
#include <tool/selection.h>
#include <tool/selection_conditions.h>
#include <tool/tool_menu.h>
#include <tool/actions.h>


#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <geometry/seg.h>
#include "../chem_includes.h"
#include <confirm.h>
#include <core/kicad_algo.h>

#include "chem_actions.h"
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

// Define CHEM_SCREEN_T if not defined
#ifndef CHEM_SCREEN_T
#define CHEM_SCREEN_T 0x4000
#endif

class SELECTION_BOX : public KIGFX::VIEW_ITEM
{
public:
    SELECTION_BOX( const BOX2I& aBox ) : m_box(aBox) {}
    const BOX2I& GetBox() const { return m_box; }
    
    // Implement required VIEW_ITEM methods
    const BOX2I ViewBBox() const override { return m_box; }
    std::vector<int> ViewGetLayers() const override { return { LAYER_DRAWINGSHEET }; }
    void ViewDraw( int aLayer, KIGFX::VIEW* aView ) const override
    {
        KIGFX::GAL* gal = aView->GetGAL();
        gal->SetIsFill( false );
        gal->SetIsStroke( true );
        gal->SetStrokeColor( COLOR4D( 1.0, 1.0, 1.0, 1.0 ) );
        gal->DrawRectangle( m_box.GetOrigin(), m_box.GetEnd() );
    }
private:
    BOX2I m_box;
};

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

bool CHEM_SELECTION_TOOL::ContainsPoint( const VECTOR2I& aPosition )
{
    for( EDA_ITEM* item : m_selection )
    {
        if( item && item->HitTest( aPosition ) )
            return true;
    }
    return false;
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
                m_menu->ShowContextMenu();
            }
            
            if( selectionModified )
                m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
        }
        
        // Handle selection drag
        else if( evt->IsDrag( BUT_LEFT ) )
        {
            // Begin drag selection
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            if( m_selection.Empty() || !ContainsPoint( pos ) )
            {
                // Start drag selection
                m_dragStartPos = pos;
                SelectMultiple( *evt );
            }
            else
            {
                // Begin moving selected items
                m_toolMgr->RunAction( "chemschema.InteractiveMove" );
            }
        }
        
        // Forward unhandled events to parent
        else
        {
            evt->SetPassEvent();
        }
        

    }
    
    return 0;
}

void CHEM_SELECTION_TOOL::InitMenu()
{
    if( !m_menu )
        return;

    CONDITIONAL_MENU& menu = m_menu->GetMenu();
    
    menu.AddItem( ACTIONS::selectAll, SELECTION_CONDITIONS::ShowAlways );
    menu.AddItem( ACTIONS::unselectAll, SELECTION_CONDITIONS::ShowAlways );
    menu.AddItem( CHEM_ACTIONS::deleteSelected, SELECTION_CONDITIONS::NotEmpty );
    menu.AddItem( CHEM_ACTIONS::duplicateSelected, SELECTION_CONDITIONS::NotEmpty );
    menu.AddItem( CHEM_ACTIONS::moveSelected, SELECTION_CONDITIONS::NotEmpty );
}

void CHEM_SELECTION_TOOL::setTransitions()
{
    Go( &CHEM_SELECTION_TOOL::Main, TOOL_EVENT( TC_COMMAND, TA_ANY ) );
    Go( &CHEM_SELECTION_TOOL::ClearSelection, TOOL_EVENT( TC_COMMAND, TA_CANCEL_TOOL ) );
    Go( &CHEM_SELECTION_TOOL::ContextMenu, TOOL_EVENT( TC_COMMAND, TA_CHOICE_MENU_CHOICE ) );
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
    std::unique_ptr<SELECTION_BOX> selectionBox( new SELECTION_BOX( BOX2I() ) );
    view->Add( selectionBox.get() );
    
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
            BOX2I box;
            box.SetOrigin( std::min( originalPos.x, dragPos.x ),
                          std::min( originalPos.y, dragPos.y ) );
            box.SetEnd( std::max( originalPos.x, dragPos.x ),
                       std::max( originalPos.y, dragPos.y ) );
            selectionBox.reset( new SELECTION_BOX( box ) );
            view->Update( selectionBox.get() );
        }
        
        // Handle button release to end selection
        else if( evt->IsMouseUp( BUT_LEFT ) )
        {
            // Find all items in the selection box
            std::vector<EDA_ITEM*> items;
            
            // Check all items in the schematic
            for( CHEM_SYMBOL* symbol : schematic->GetSymbols() )
            {
                if( symbol->HitTest( selectionBox->GetBox(), false ) )
                    items.push_back( symbol );
            }
            
            for( CHEM_CONNECTION* conn : schematic->GetConnections() )
            {
                if( conn->HitTest( selectionBox->GetBox(), false ) )
                    items.push_back( conn );
            }
            
            for( CHEM_LABEL* label : schematic->GetLabels() )
            {
                if( label->HitTest( selectionBox->GetBox(), false ) )
                    items.push_back( label );
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
            
            // Remove selection box
            view->Remove( selectionBox.get() );
            
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
    for( CHEM_SYMBOL* symbol : schematic->GetSymbols() )
    {
        items.push_back( symbol );
    }
    
    for( CHEM_CONNECTION* conn : schematic->GetConnections() )
    {
        items.push_back( conn );
    }
    
    for( CHEM_LABEL* label : schematic->GetLabels() )
    {
        items.push_back( label );
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
    if( !view )
        return;

    for( EDA_ITEM* item : m_selection )
    {
        if( item )
        {
            view->Update( item );
        }
    }
    view->UpdateAllItems( LAYER_DRAWINGSHEET );
}

bool CHEM_SELECTION_TOOL::HandleClick( const VECTOR2I& aPosition, const TOOL_EVENT& aEvent, bool aAllowDisambiguation )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    if( !schematic )
        return false;

    std::vector<EDA_ITEM*> items;
    
    // Check symbols
    for( CHEM_SYMBOL* symbol : schematic->GetSymbols() )
    {
        if( symbol->HitTest( aPosition ) )
            items.push_back( symbol );
    }
    
    // Check connections
    for( CHEM_CONNECTION* conn : schematic->GetConnections() )
    {
        if( conn->HitTest( aPosition ) )
            items.push_back( conn );
    }
    
    // Check labels
    for( CHEM_LABEL* label : schematic->GetLabels() )
    {
        if( label->HitTest( aPosition ) )
            items.push_back( label );
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

EDA_ITEM* CHEM_SELECTION_TOOL::disambiguateItem( const std::vector<EDA_ITEM*>& aItems, const VECTOR2I& aPosition )
{
    if( aItems.empty() )
        return nullptr;

    if( aItems.size() == 1 )
        return aItems[0];

    wxMenu menu;
    menu.SetTitle( _( "Disambiguation" ) );

    int i = 0;
    for( EDA_ITEM* item : aItems )
    {
        wxString text;
        CHEM_ITEM* chemItem = dynamic_cast<CHEM_ITEM*>( item );
        
        if( !chemItem )
            continue;

        switch( chemItem->Type() )
        {
            case CHEM_SYMBOL_T:
                text = wxString::Format( _( "Symbol: %s" ), 
                    static_cast<CHEM_SYMBOL*>( chemItem )->GetName() );
                break;
            case CHEM_LINE_T:
                text = _( "Line" );
                break;
            case CHEM_JUNCTION_T:
                text = _( "Junction" );
                break;
            case CHEM_LABEL_T:
                text = wxString::Format( _( "Label: %s" ), 
                    static_cast<CHEM_LABEL*>( chemItem )->GetText() );
                break;
            case CHEM_SCREEN_T:
                text = _( "Screen" );
                break;
            case CHEM_SHEET_T:
                text = _( "Sheet" );
                break;
            default:
                text = _( "Unknown" );
                break;
        }

        wxMenuItem* menuItem = menu.Append( i, text );
        if( menuItem )
        {
            menuItem->SetRefData( new wxObjectDataPtr<EDA_ITEM>( item ) );
        }
        i++;
    }

    wxPoint pos( aPosition.x, aPosition.y );
    int selectedId = menu.GetPopupMenuSelection( pos );
    wxMenuItem* selectedMenuItem = menu.FindItem( selectedId );

    if( selectedMenuItem )
    {
        wxObjectDataPtr<EDA_ITEM>* data = static_cast<wxObjectDataPtr<EDA_ITEM>*>( selectedMenuItem->GetRefData() );
        return data->get();
    }

    return nullptr;
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
        m_menu->UpdateMenu();
        m_menu->ShowContextMenu( m_selection );
    }
    
    return 0;
} 