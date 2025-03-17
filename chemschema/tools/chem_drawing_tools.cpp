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
#include <tool/action_menu.h>
#include <tool/tool_event.h>
#include <tool/common_tools.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <geometry/seg.h>
#include <confirm.h>

#include "chem_drawing_tools.h"
#include "../chem_edit_frame.h"
#include "../chem_schematic.h"
#include "../chem_item.h"
#include "../chem_symbol.h"
#include "../chem_line.h"
#include "../chem_sheet.h"
#include "../chem_junction.h"
#include "../chem_label.h"
#include "../chem_screen.h"
#include "../chem_includes.h"

using namespace std;

CHEM_DRAWING_TOOLS::CHEM_DRAWING_TOOLS() :
    TOOL_INTERACTIVE( "chemschema.InteractiveDrawing" ),
    m_frame( nullptr ),
    m_drawingMode( CHEM_DRAWING_LINE ),
    m_currentItem( nullptr ),
    m_isAddingMultiple( false )
{
}


CHEM_DRAWING_TOOLS::~CHEM_DRAWING_TOOLS()
{
    delete m_currentItem;
}


void CHEM_DRAWING_TOOLS::Reset( RESET_REASON aReason )
{
    // Clean up any current item
    delete m_currentItem;
    m_currentItem = nullptr;
}


int CHEM_DRAWING_TOOLS::InteractiveDrawing( const TOOL_EVENT& aEvent )
{
    // Main loop for interactive drawing
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Check if a new item is being created or if we're continuing with the current one
    if( !m_currentItem )
    {
        // Determine what type of item we're drawing based on the last action
        if( aEvent.Parameter<CHEM_ITEM*>() )
        {
            // Continue with an existing item passed from another tool
            m_currentItem = aEvent.Parameter<CHEM_ITEM*>();
            
            // Set mode based on item type
            switch( m_currentItem->Type() )
            {
            case CHEM_ITEM::CHEM_LINE_T:
                SetDrawingMode( CHEM_DRAWING_LINE );
                break;
                
            case CHEM_ITEM::CHEM_SYMBOL_T:
                SetDrawingMode( CHEM_DRAWING_SYMBOL );
                break;
                
            case CHEM_ITEM::CHEM_JUNCTION_T:
                SetDrawingMode( CHEM_DRAWING_JUNCTION );
                break;
                
            case CHEM_ITEM::CHEM_LABEL_T:
                SetDrawingMode( CHEM_DRAWING_LABEL );
                break;
                
            case CHEM_ITEM::CHEM_SCREEN_T:
                SetDrawingMode( CHEM_DRAWING_SCREEN );
                break;
                
            case CHEM_ITEM::CHEM_SHEET_T:
                SetDrawingMode( CHEM_DRAWING_SHEET );
                break;
                
            default:
                return 0;  // Unsupported item type
            }
        }
        else
        {
            // No item parameter, start with a line by default
            SetDrawingMode( CHEM_DRAWING_LINE );
            m_currentItem = new CHEM_LINE();
        }
    }
    
    // Set up drawing environment
    getViewControls()->ShowCursor( true );
    getViewControls()->SetSnapping( true );
    getViewControls()->SetAutoPan( true );
    
    // Initial cursor position
    m_startPoint = GetCursorPosition();
    
    // Start drawing based on the current mode
    switch( m_drawingMode )
    {
    case CHEM_DRAWING_LINE:
        return DrawLine( aEvent );
        
    case CHEM_DRAWING_SYMBOL:
        return PlaceSymbol( aEvent );
        
    case CHEM_DRAWING_JUNCTION:
        return PlaceJunction( aEvent );
        
    case CHEM_DRAWING_LABEL:
        return PlaceLabel( aEvent );
        
    case CHEM_DRAWING_SCREEN:
        return PlaceScreen( aEvent );
        
    case CHEM_DRAWING_SHEET:
        return PlaceSheet( aEvent );
        
    default:
        return 0;
    }
    
    return 0;
}


int CHEM_DRAWING_TOOLS::DrawLine( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid line
    CHEM_LINE* line = dynamic_cast<CHEM_LINE*>( m_currentItem );
    
    if( !line )
    {
        m_currentItem = new CHEM_LINE();
        line = static_cast<CHEM_LINE*>( m_currentItem );
    }
    
    // Check if we're creating a new line or continuing an existing one
    bool isNewLine = true;
    
    if( line->GetPoints().empty() )
    {
        // New line, add initial point
        line->AddPoint( wxPoint( m_startPoint.x, m_startPoint.y ) );
        isNewLine = true;
    }
    else
    {
        isNewLine = false;
    }
    
    // Add the current point
    wxPoint currentPoint( GetCursorPosition().x, GetCursorPosition().y );
    line->AddPoint( currentPoint );
    
    // Draw preview
    DrawPreview( line );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update the last point to follow the cursor
            VECTOR2I pos = GetCursorPosition();
            std::vector<wxPoint> points = line->GetPoints();
            
            if( !points.empty() )
            {
                points.back() = wxPoint( pos.x, pos.y );
                line->SetPoints( points );
                DrawPreview( line );
            }
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Add a new point at the current position
            VECTOR2I pos = GetCursorPosition();
            wxPoint pt( pos.x, pos.y );
            
            // Add the point to the line
            std::vector<wxPoint> points = line->GetPoints();
            
            if( !points.empty() && points.back() == pt )
            {
                // Same point as last, ignore
                continue;
            }
            
            // Add the new point
            line->AddPoint( pt );
            
            // Draw preview
            DrawPreview( line );
        }
        
        else if( evt->IsClick( BUT_RIGHT ) || evt->IsDblClick( BUT_LEFT ) )
        {
            // Finish the line
            std::vector<wxPoint> points = line->GetPoints();
            
            if( points.size() < 2 )
            {
                // Line needs at least 2 points
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            
            // Save the line to the schematic
            schematic->Add( line );
            
            // Update the view
            getView()->Update( line );
            m_frame->OnModify();
            
            // Start a new line if double-click
            if( evt->IsDblClick( BUT_LEFT ) )
            {
                // Create a new line starting from the last point
                wxPoint lastPoint = points.back();
                m_currentItem = new CHEM_LINE();
                line = static_cast<CHEM_LINE*>( m_currentItem );
                line->AddPoint( lastPoint );
                
                // Preview the new line
                DrawPreview( line );
            }
            else
            {
                // Finish drawing
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


int CHEM_DRAWING_TOOLS::PlaceSymbol( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid symbol
    CHEM_SYMBOL* symbol = dynamic_cast<CHEM_SYMBOL*>( m_currentItem );
    
    if( !symbol )
    {
        m_currentItem = new CHEM_SYMBOL();
        symbol = static_cast<CHEM_SYMBOL*>( m_currentItem );
    }
    
    // Set initial position
    UpdateItemPosition( symbol, GetCursorPosition() );
    
    // Draw preview
    DrawPreview( symbol );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update position to follow the cursor
            UpdateItemPosition( symbol, GetCursorPosition() );
            DrawPreview( symbol );
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Place the symbol at the current position
            VECTOR2I pos = GetCursorPosition();
            UpdateItemPosition( symbol, pos );
            
            // Add to schematic
            schematic->Add( symbol );
            
            // Update the view
            getView()->Update( symbol );
            m_frame->OnModify();
            
            // Prepare for next placement if adding multiple
            if( m_isAddingMultiple )
            {
                m_currentItem = static_cast<CHEM_SYMBOL*>( symbol->Clone() );
                symbol = static_cast<CHEM_SYMBOL*>( m_currentItem );
                UpdateItemPosition( symbol, GetCursorPosition() );
                DrawPreview( symbol );
            }
            else
            {
                // Finish placement
                m_currentItem = nullptr;
                break;
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Cancel or finish placement
            if( m_isAddingMultiple )
            {
                // Stop adding multiple
                m_isAddingMultiple = false;
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            else
            {
                // Cancel placement
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


int CHEM_DRAWING_TOOLS::PlaceJunction( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid junction
    CHEM_JUNCTION* junction = dynamic_cast<CHEM_JUNCTION*>( m_currentItem );
    
    if( !junction )
    {
        m_currentItem = new CHEM_JUNCTION();
        junction = static_cast<CHEM_JUNCTION*>( m_currentItem );
    }
    
    // Set initial position
    UpdateItemPosition( junction, GetCursorPosition() );
    
    // Draw preview
    DrawPreview( junction );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update position to follow the cursor
            UpdateItemPosition( junction, GetCursorPosition() );
            DrawPreview( junction );
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Place the junction at the current position
            VECTOR2I pos = GetCursorPosition();
            UpdateItemPosition( junction, pos );
            
            // Add to schematic
            schematic->Add( junction );
            
            // Update the view
            getView()->Update( junction );
            m_frame->OnModify();
            
            // Prepare for next placement if adding multiple
            if( m_isAddingMultiple )
            {
                m_currentItem = static_cast<CHEM_JUNCTION*>( junction->Clone() );
                junction = static_cast<CHEM_JUNCTION*>( m_currentItem );
                UpdateItemPosition( junction, GetCursorPosition() );
                DrawPreview( junction );
            }
            else
            {
                // Finish placement
                m_currentItem = nullptr;
                break;
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Cancel or finish placement
            if( m_isAddingMultiple )
            {
                // Stop adding multiple
                m_isAddingMultiple = false;
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            else
            {
                // Cancel placement
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


int CHEM_DRAWING_TOOLS::PlaceLabel( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid label
    CHEM_LABEL* label = dynamic_cast<CHEM_LABEL*>( m_currentItem );
    
    if( !label )
    {
        m_currentItem = new CHEM_LABEL();
        label = static_cast<CHEM_LABEL*>( m_currentItem );
        
        // Set default properties
        label->SetText( wxT( "Label" ) );
    }
    
    // Set initial position
    UpdateItemPosition( label, GetCursorPosition() );
    
    // Draw preview
    DrawPreview( label );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update position to follow the cursor
            UpdateItemPosition( label, GetCursorPosition() );
            DrawPreview( label );
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Place the label at the current position
            VECTOR2I pos = GetCursorPosition();
            UpdateItemPosition( label, pos );
            
            // Add to schematic
            schematic->Add( label );
            
            // Update the view
            getView()->Update( label );
            m_frame->OnModify();
            
            // Prepare for next placement if adding multiple
            if( m_isAddingMultiple )
            {
                m_currentItem = static_cast<CHEM_LABEL*>( label->Clone() );
                label = static_cast<CHEM_LABEL*>( m_currentItem );
                UpdateItemPosition( label, GetCursorPosition() );
                DrawPreview( label );
            }
            else
            {
                // Finish placement
                m_currentItem = nullptr;
                break;
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Cancel or finish placement
            if( m_isAddingMultiple )
            {
                // Stop adding multiple
                m_isAddingMultiple = false;
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            else
            {
                // Cancel placement
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


int CHEM_DRAWING_TOOLS::PlaceScreen( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid screen
    CHEM_SCREEN* screen = dynamic_cast<CHEM_SCREEN*>( m_currentItem );
    
    if( !screen )
    {
        m_currentItem = new CHEM_SCREEN();
        screen = static_cast<CHEM_SCREEN*>( m_currentItem );
    }
    
    // Set initial position
    UpdateItemPosition( screen, GetCursorPosition() );
    
    // Draw preview
    DrawPreview( screen );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update position to follow the cursor
            UpdateItemPosition( screen, GetCursorPosition() );
            DrawPreview( screen );
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Place the screen at the current position
            VECTOR2I pos = GetCursorPosition();
            UpdateItemPosition( screen, pos );
            
            // Add to schematic
            schematic->Add( screen );
            
            // Update the view
            getView()->Update( screen );
            m_frame->OnModify();
            
            // Prepare for next placement if adding multiple
            if( m_isAddingMultiple )
            {
                m_currentItem = static_cast<CHEM_SCREEN*>( screen->Clone() );
                screen = static_cast<CHEM_SCREEN*>( m_currentItem );
                UpdateItemPosition( screen, GetCursorPosition() );
                DrawPreview( screen );
            }
            else
            {
                // Finish placement
                m_currentItem = nullptr;
                break;
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Cancel or finish placement
            if( m_isAddingMultiple )
            {
                // Stop adding multiple
                m_isAddingMultiple = false;
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            else
            {
                // Cancel placement
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


int CHEM_DRAWING_TOOLS::PlaceSheet( const TOOL_EVENT& aEvent )
{
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Make sure we have a valid sheet
    CHEM_SHEET* sheet = dynamic_cast<CHEM_SHEET*>( m_currentItem );
    
    if( !sheet )
    {
        m_currentItem = new CHEM_SHEET();
        sheet = static_cast<CHEM_SHEET*>( m_currentItem );
    }
    
    // Set initial position
    UpdateItemPosition( sheet, GetCursorPosition() );
    
    // Draw preview
    DrawPreview( sheet );
    
    // Main drawing loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            delete m_currentItem;
            m_currentItem = nullptr;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // Update position to follow the cursor
            UpdateItemPosition( sheet, GetCursorPosition() );
            DrawPreview( sheet );
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Place the sheet at the current position
            VECTOR2I pos = GetCursorPosition();
            UpdateItemPosition( sheet, pos );
            
            // Add to schematic
            schematic->Add( sheet );
            
            // Update the view
            getView()->Update( sheet );
            m_frame->OnModify();
            
            // Prepare for next placement if adding multiple
            if( m_isAddingMultiple )
            {
                m_currentItem = static_cast<CHEM_SHEET*>( sheet->Clone() );
                sheet = static_cast<CHEM_SHEET*>( m_currentItem );
                UpdateItemPosition( sheet, GetCursorPosition() );
                DrawPreview( sheet );
            }
            else
            {
                // Finish placement
                m_currentItem = nullptr;
                break;
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Cancel or finish placement
            if( m_isAddingMultiple )
            {
                // Stop adding multiple
                m_isAddingMultiple = false;
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
            else
            {
                // Cancel placement
                delete m_currentItem;
                m_currentItem = nullptr;
                break;
            }
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    
    return 0;
}


void CHEM_DRAWING_TOOLS::DrawPreview( CHEM_ITEM* aItem )
{
    // Highlight the current item for preview
    KIGFX::VIEW* view = getView();
    
    // Show the item in preview mode
    view->SetVisible( aItem, true );
    view->Update( aItem, KIGFX::GEOMETRY );
}


void CHEM_DRAWING_TOOLS::UpdateItemPosition( CHEM_ITEM* aItem, const VECTOR2I& aPosition )
{
    switch( aItem->Type() )
    {
    case CHEM_ITEM::CHEM_SYMBOL_T:
        {
            CHEM_SYMBOL* symbol = static_cast<CHEM_SYMBOL*>( aItem );
            symbol->SetPosition( wxPoint( aPosition.x, aPosition.y ) );
        }
        break;
        
    case CHEM_ITEM::CHEM_JUNCTION_T:
        {
            CHEM_JUNCTION* junction = static_cast<CHEM_JUNCTION*>( aItem );
            junction->SetPosition( wxPoint( aPosition.x, aPosition.y ) );
        }
        break;
        
    case CHEM_ITEM::CHEM_LABEL_T:
        {
            CHEM_LABEL* label = static_cast<CHEM_LABEL*>( aItem );
            label->SetTextPos( VECTOR2I( aPosition.x, aPosition.y ) );
        }
        break;
        
    case CHEM_ITEM::CHEM_SCREEN_T:
        {
            CHEM_SCREEN* screen = static_cast<CHEM_SCREEN*>( aItem );
            screen->SetPosition( aPosition );
        }
        break;
        
    case CHEM_ITEM::CHEM_SHEET_T:
        {
            CHEM_SHEET* sheet = static_cast<CHEM_SHEET*>( aItem );
            sheet->SetPosition( wxPoint( aPosition.x, aPosition.y ) );
        }
        break;
        
    default:
        break;
    }
}


VECTOR2I CHEM_DRAWING_TOOLS::GetCursorPosition()
{
    return getViewControls()->GetCursorPosition();
}


void CHEM_DRAWING_TOOLS::SetDrawingMode( int aMode )
{
    m_drawingMode = aMode;
}


bool CHEM_DRAWING_TOOLS::IsPositionValid( CHEM_ITEM* aItem, const VECTOR2I& aPosition )
{
    // For now, assume all positions are valid
    return true;
}


void CHEM_DRAWING_TOOLS::setTransitions()
{
    Go( &CHEM_DRAWING_TOOLS::InteractiveDrawing,      TC_COMMAND, TA_ACTIVATE, "chemschema.InteractiveDrawing" );
    Go( &CHEM_DRAWING_TOOLS::DrawLine,                CHEMSCHEMA_ACTIONS::newLine.MakeEvent() );
    Go( &CHEM_DRAWING_TOOLS::PlaceSymbol,             CHEMSCHEMA_ACTIONS::newSymbol.MakeEvent() );
    Go( &CHEM_DRAWING_TOOLS::PlaceJunction,           CHEMSCHEMA_ACTIONS::newJunction.MakeEvent() );
    Go( &CHEM_DRAWING_TOOLS::PlaceLabel,              CHEMSCHEMA_ACTIONS::newLabel.MakeEvent() );
    Go( &CHEM_DRAWING_TOOLS::PlaceScreen,             CHEMSCHEMA_ACTIONS::newScreen.MakeEvent() );
    Go( &CHEM_DRAWING_TOOLS::PlaceSheet,              CHEMSCHEMA_ACTIONS::newSheet.MakeEvent() );
} 