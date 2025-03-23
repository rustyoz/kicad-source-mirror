/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2024 KiCad Developers.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <chem_view.h>
#include <chem_base_frame.h>
#include <chem_schematic.h>
#include <chem_screen.h>
#include <chem_edit_frame.h>
#include <chem_item.h>
#include <lib_symbol.h>
#include <drawing_sheet/ds_proxy_view_item.h>
#include <view/view_group.h>
#include <view/view_item.h>
#include <preview_items/selection_area.h>
#include <math/vector2d.h>
#include <chem_units.h>

using namespace KIGFX;

CHEM_VIEW::CHEM_VIEW( CHEM_BASE_FRAME* aFrame ) :
    VIEW( true ),
    m_frame( aFrame ),
    m_chemSchematic( nullptr )
{
}


CHEM_VIEW::~CHEM_VIEW()
{
    Cleanup();
}


void CHEM_VIEW::Cleanup()
{
    Clear();
    m_drawingSheet.reset();
}


void CHEM_VIEW::DisplaySheet( const CHEM_SCREEN* aScreen )
{
    Clear();

    if( !aScreen )
        return;

    for( CHEM_ITEM* item : aScreen->Items() )
        Add( item );

    m_drawingSheet.reset( new DS_PROXY_VIEW_ITEM( chemIUScale, &aScreen->GetPageSettings(),
                                                 &aScreen->Schematic()->Prj(),
                                                 &aScreen->GetTitleBlock(),
                                                 aScreen->Schematic()->GetProperties() ) );
    m_drawingSheet->SetPageNumber( TO_UTF8( aScreen->GetPageNumber() ) );
    m_drawingSheet->SetSheetCount( aScreen->GetPageCount() );
    m_drawingSheet->SetFileName( TO_UTF8( aScreen->GetFileName() ) );
    m_drawingSheet->SetColorLayer( LAYER_CHEMSCHEMA_DRAWINGSHEET );
    m_drawingSheet->SetPageBorderColorLayer( LAYER_CHEMSCHEMA_PAGE_LIMITS );
    m_drawingSheet->SetIsFirstPage( aScreen->GetVirtualPageNumber() == 1 );

    if( m_frame && m_frame->IsType( FRAME_CHEM ) )
    {
        CHEM_EDIT_FRAME* editFrame = dynamic_cast<CHEM_EDIT_FRAME*>( m_frame );

        wxCHECK( editFrame, /* void */ );

        m_drawingSheet->SetSheetName( TO_UTF8( editFrame->GetScreenDesc() ) );
        m_drawingSheet->SetSheetPath( TO_UTF8( editFrame->GetFullScreenDesc() ) );
    }
    else
    {
        m_drawingSheet->SetSheetName( "" );
        m_drawingSheet->SetSheetPath( "" );
    }

    ResizeSheetWorkingArea( aScreen );

    Add( m_drawingSheet.get() );

    InitPreview();

    // Allow tools to add anything they require to the view (such as the selection VIEW_GROUP)
    if( m_frame && m_frame->GetToolManager() )
        m_frame->GetToolManager()->ResetTools( TOOL_BASE::REDRAW );
}


void CHEM_VIEW::DisplaySymbol( LIB_SYMBOL* aSymbol )
{
    Clear();

    if( !aSymbol )
        return;

    // Draw the fields.
    for( SCH_ITEM& item : aSymbol->GetDrawItems() )
    {
        if( item.Type() == SCH_FIELD_T )
            Add( &item );
    }

    LIB_SYMBOL* drawnSymbol = aSymbol;

    // Draw the parent items if the symbol is inherited from another symbol.
    if( aSymbol->IsDerived() )
    {
        if( std::shared_ptr< LIB_SYMBOL > parent = aSymbol->GetRootSymbol() )
            drawnSymbol = parent.get();
        else
        {
            wxCHECK( false, /* void */ );
        }
    }

    for( SCH_ITEM& item : drawnSymbol->GetDrawItems() )
    {
        // Fields already drawn above.  (Besides, we don't want to show parent symbol fields as
        // users may be confused by shown fields that can not be edited.)
        if( item.Type() == SCH_FIELD_T )
            continue;

        Add( &item );
    }

    InitPreview();
}


void CHEM_VIEW::ResizeSheetWorkingArea( const CHEM_SCREEN* aScreen )
{
    if( !aScreen )
        return;

    BOX2I bbox = aScreen->GetBoundingBox();

    if( bbox.GetWidth() == 0 || bbox.GetHeight() == 0 )
        bbox = BOX2I( VECTOR2I( 0, 0 ), VECTOR2I( chemIUScale.MilsToIU( 11000 ),
                                                 chemIUScale.MilsToIU( 8500 ) ) );

    SetBoundary( bbox );
}


void CHEM_VIEW::SetScale( double aScale, VECTOR2D aAnchor )
{
    VIEW::SetScale( aScale, aAnchor );

    // Redraw items that have to be displayed in a different way when scale changes
    // (text size for instance)
    for( VIEW_ITEM* item : *m_allItems )
    {
        if( !item )
            continue;

        if( item->RenderAsBitmap( GetGAL()->GetWorldScale() ) != item->IsShownAsBitmap() )
            Update( item, KIGFX::REPAINT );
    }
}


void CHEM_VIEW::ClearHiddenFlags()
{
    for( VIEW_ITEM* item : *m_allItems )
    {
        if( !item )
            continue;

        Hide( item, false );
    }
}


void CHEM_VIEW::HideDrawingSheet()
{
    //    SetVisible( m_drawingSheet.get(), false );
}

void CHEM_VIEW::SetChemSchematic( CHEM_SCHEMATIC* aChemSchematic )
{
    m_chemSchematic = aChemSchematic;
}

CHEM_SCHEMATIC* CHEM_VIEW::GetChemSchematic() const
{
    return m_chemSchematic;
}

void CHEM_VIEW::UpdateAllItems()
{
    for( VIEW_ITEM* item : *m_allItems )
    {
        if( !item )
            continue;

        Update( item, KIGFX::REPAINT );
    }
}

void CHEM_VIEW::UpdateDisplayOptions( const CHEM_DISPLAY_OPTIONS& aOptions )
{
    // Update visibility based on display options
    for( VIEW_ITEM* item : *m_allItems )
    {
        if( !item )
            continue;

        // Update item visibility based on options
        // This is a basic implementation - expand based on item types
        bool visible = true;
        
        if( item->Type() == CHEM_GRID_T && !aOptions.m_showGrid )
            visible = false;
        else if( item->Type() == CHEM_LABEL_T && !aOptions.m_showLabels )
            visible = false;
        else if( item->Type() == CHEM_CONNECTION_T && !aOptions.m_showConnections )
            visible = false;
        else if( item->Type() == CHEM_SYMBOL_T && !aOptions.m_showSymbols )
            visible = false;

        Hide( item, !visible );
    }

    // Request screen refresh
    UpdateAllItems();
} 