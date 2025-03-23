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

/**
 * A helper wxWidgets control client data object to store item pointers.
 */
class CHEM_ITEM_CLIENT_DATA : public wxClientData
{
public:
    CHEM_ITEM_CLIENT_DATA( EDA_ITEM* aItem ) : m_item(aItem) {}
    EDA_ITEM* GetItem() const { return m_item; }

private:
    EDA_ITEM* m_item;
};

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
    SELECTION_TOOL( "chemschema.InteractiveSelection" ),
    m_frame( nullptr ),
    m_menu( nullptr ),
    m_skipMenuEvent( false ),
    m_selection( nullptr )
{
}

CHEM_SELECTION_TOOL::~CHEM_SELECTION_TOOL()
{
}

void CHEM_SELECTION_TOOL::Reset( RESET_REASON aReason )
{
    SELECTION_TOOL::Reset( aReason );
    
    if( aReason == MODEL_RELOAD )
    {
        // Clear selection when reloading the schematic
        selection().Clear();
    }
}

int CHEM_SELECTION_TOOL::Main( const TOOL_EVENT& aEvent )
{
    // Main loop: keep receiving events
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();
    
    // Set initial cursor
    getViewControls()->ShowCursor( true );
    
    while( TOOL_EVENT* evt = Wait() )
    {
        // Handle certain events specially
        if( evt->IsCancel() || evt->IsActivate() )
        {
            if( evt->IsCancel() )
                selection().Clear();
                
            if( evt->IsActivate() && !evt->IsMoveTool() )
                selection().Clear();
                
            break;
        }
        
        // Handle cursor movement and view controls
        else if( evt->IsMotion() )
        {
            // Update cursor position
            getViewControls()->ForceCursorPosition( false );

            // Update cursor based on selection state
            if( !selection().Empty() && !hasModifier() && selectionContains( evt->Position() ) )
            {
                m_frame->GetCanvas()->SetCurrentCursor( KICURSOR::MOVING );
            }
            else
            {
                m_frame->GetCanvas()->SetCurrentCursor( KICURSOR::ARROW );
            }
        }
        
        // Handle mouse buttons
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Handle left click for selection
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            // Check modifier keys
            setModifiersState( wxGetKeyState( WXK_SHIFT ), 
                             wxGetKeyState( WXK_CONTROL ),
                             wxGetKeyState( WXK_ALT ) );
            
            if( !hasModifier() && !selection().Empty() )
                selection().Clear();
                
            // Handle the click on items
            HandleClick( pos, *evt, true );
        }
        
        // Handle right-click for context menu
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            VECTOR2I pos = getViewControls()->GetCursorPosition();
            
            // Select item if not yet selected
            bool selectionModified = false;
            
            if( selection().Empty() )
            {
                selectionModified = HandleClick( pos, *evt, true );
            }
            
            // Show context menu if we have a selection or if the module has one
            if( !selection().Empty() && m_menu )
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
            
            if( selection().Empty() || !selectionContains( pos ) )
            {
                // Start drag selection
                m_dragStartPos = pos;
                SelectMultiple( *evt );
            }
            else
            {
                // Check if dragging has started within any of selected items bounding box
                if( selectionContains( evt->DragOrigin() ) )
                {
                    // Begin moving selected items
                    m_toolMgr->RunAction( CHEM_ACTIONS::moveSelected );
                }
                else
                {
                    // No -> drag a selection box
                    m_dragStartPos = pos;
                    SelectMultiple( *evt );
                }
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

}

int CHEM_SELECTION_TOOL::SelectSingle( const TOOL_EVENT& aEvent )
{
    VECTOR2I pos = getViewControls()->GetCursorPosition();
    
    setModifiersState( wxGetKeyState( WXK_SHIFT ), 
                      wxGetKeyState( WXK_CONTROL ),
                      wxGetKeyState( WXK_ALT ) );
    
    if( !hasModifier() && !selection().Empty() )
        selection().Clear();
        
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
    setModifiersState( wxGetKeyState( WXK_SHIFT ), 
                      wxGetKeyState( WXK_CONTROL ),
                      wxGetKeyState( WXK_ALT ) );
    
    if( !hasModifier() && !selection().Empty() )
        selection().Clear();
        
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
    
    if( !selection().Empty() )
        m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
        
    return 0;
}

int CHEM_SELECTION_TOOL::SelectAll( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    if( !schematic )
        return 0;
        
    selection().Clear();
    
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
    for( EDA_ITEM* item : items )
    {
        AddItemToSel( item, true );
    }
    
    m_toolMgr->ProcessEvent( EVENTS::SelectedEvent );
    
    return items.size();
}

bool CHEM_SELECTION_TOOL::HandleClick( const VECTOR2I& aPosition, const TOOL_EVENT& aEvent, bool aAllowDisambiguation )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    if( !schematic )
        return false;

    CHEM_COLLECTOR collector;
    collector.Collect( schematic->GetCurrentSheet(), aPosition );

    // No items found
    if( collector.GetCount() == 0 )
        return false;

    // Handle case with multiple items
    EDA_ITEM* item = nullptr;

    if( collector.GetCount() > 1 && aAllowDisambiguation )
    {
        collector.m_MenuTitle = _( "Select Item:" );
        if( doSelectionMenu( &collector ) )
        {
            if( collector.GetCount() == 1 )
            {
                // Single item selected from menu
                item = collector[0];
            }
            else
            {
                // "Select All" was chosen
                for( int i = 0; i < collector.GetCount(); ++i )
                {
                    if( m_subtractive || ( m_additive && selection().Contains( collector[i] ) ) )
                        RemoveItemFromSel( collector[i] );
                    else
                        AddItemToSel( collector[i] );
                }
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        item = collector[0];
    }

    if( !item )
        return false;

    // Toggle item selection
    if( m_subtractive || ( m_additive && selection().Contains( item ) ) )
        RemoveItemFromSel( item );
    else
        AddItemToSel( item );

    return true;
}

int CHEM_SELECTION_TOOL::MoveSelected( const TOOL_EVENT& aEvent )
{
    if( selection().Empty() )
        return 0;
        
    // Forward to the move tool
    m_toolMgr->RunAction( CHEM_ACTIONS::moveSelected );
    
    return 0;
}

int CHEM_SELECTION_TOOL::ContextMenu( const TOOL_EVENT& aEvent )
{
    if( m_skipMenuEvent )
        return 0;
        
    if( selection().Empty() )
        return 0;
        
    if( m_menu )
    {
        m_menu->ShowContextMenu( selection() );
    }
    
    return 0;
}

bool CHEM_SELECTION_TOOL::selectionContains( const VECTOR2I& aPoint ) const
{
    const unsigned GRIP_MARGIN = 20;
    int margin = KiROUND( getView()->ToWorld( GRIP_MARGIN ) );

    // Check if the point is located within any of the currently selected items bounding boxes
    for( EDA_ITEM* item : m_selection )
    {
        BOX2I itemBox = item->ViewBBox();
        itemBox.Inflate( margin ); // Give some margin for gripping an item

        if( itemBox.Contains( aPoint ) )
            return true;
    }

    return false;
} 