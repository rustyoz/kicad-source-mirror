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

#include <chem_draw_panel.h>
#include <chem_base_frame.h>
#include <chem_painter.h>
#include <settings/color_settings.h>
#include <pgm_base.h>
#include <settings/settings_manager.h>
#include <view/wx_view_controls.h>
#include <zoom_defines.h>

CHEM_DRAW_PANEL::CHEM_DRAW_PANEL( wxWindow* aParentWindow, wxWindowID aWindowId,
                                 const wxPoint& aPosition, const wxSize& aSize,
                                 KIGFX::GAL_DISPLAY_OPTIONS& aOptions, GAL_TYPE aGalType )
        : EDA_DRAW_PANEL_GAL( aParentWindow, aWindowId, aPosition, aSize, aOptions, aGalType )
{
    m_view = new KIGFX::CHEM_VIEW( dynamic_cast<CHEM_BASE_FRAME*>( GetParentEDAFrame() ) );
    m_view->SetGAL( m_gal );

    m_gal->SetWorldUnitLength( CHEM_WORLD_UNIT );

    m_painter.reset( new KIGFX::CHEM_PAINTER( m_gal ) );

    COLOR_SETTINGS* cs = nullptr;

    if( auto frame = dynamic_cast<CHEM_BASE_FRAME*>( GetParentEDAFrame() ) )
        cs = frame->GetColorSettings();
    else
        cs = Pgm().GetSettingsManager().GetColorSettings();

    wxASSERT( cs );
    m_painter->GetSettings()->LoadColors( cs );

    m_view->SetPainter( m_painter.get() );

    // Set zoom limits
    m_view->SetScaleLimits( ZOOM_MAX_LIMIT_CHEMSCHEMA, ZOOM_MIN_LIMIT_CHEMSCHEMA );
    m_view->SetMirror( false, false );

    // Initialize canvas background color
    auto settings = m_painter->GetSettings();
    m_gal->SetClearColor( settings->GetBackgroundColor() );

    setDefaultLayerOrder();
    setDefaultLayerDeps();

    GetView()->UpdateAllLayersOrder();

    // View controls is the first in the event handler chain
    m_viewControls = new KIGFX::WX_VIEW_CONTROLS( m_view, this );

    SetEvtHandlerEnabled( true );
    SetFocus();
    Show( true );
    Raise();
    StartDrawing();
}


CHEM_DRAW_PANEL::~CHEM_DRAW_PANEL()
{
}


void CHEM_DRAW_PANEL::DisplaySymbol( LIB_SYMBOL* aSymbol )
{
    GetView()->DisplaySymbol( aSymbol );
}


void CHEM_DRAW_PANEL::DisplaySheet( CHEM_SCREEN* aScreen )
{
    GetView()->Clear();

    if( aScreen )
        GetView()->DisplaySheet( aScreen );
    else
        GetView()->Cleanup();
}


bool CHEM_DRAW_PANEL::SwitchBackend( GAL_TYPE aGalType )
{
    bool rv = EDA_DRAW_PANEL_GAL::SwitchBackend( aGalType );
    setDefaultLayerDeps();
    m_gal->SetWorldUnitLength( CHEM_WORLD_UNIT );

    Refresh();

    return rv;
}


KIGFX::CHEM_VIEW* CHEM_DRAW_PANEL::GetView() const
{
    return static_cast<KIGFX::CHEM_VIEW*>( m_view );
}


void CHEM_DRAW_PANEL::onPaint( wxPaintEvent& aEvent )
{
    if( IsShownOnScreen() )
        EDA_DRAW_PANEL_GAL::onPaint( aEvent );
}


void CHEM_DRAW_PANEL::OnShow()
{
    EDA_DRAW_PANEL_GAL::OnShow();
}


void CHEM_DRAW_PANEL::setDefaultLayerOrder()
{
    for( int i = 0; (unsigned) i < sizeof( CHEM_LAYER_ORDER ) / sizeof( int ); ++i )
    {
        int layer = CHEM_LAYER_ORDER[i];
        wxASSERT( layer < KIGFX::VIEW::VIEW_MAX_LAYERS );

        m_view->SetLayerOrder( layer, i );
    }
}


void CHEM_DRAW_PANEL::setDefaultLayerDeps()
{
    // caching makes no sense for Cairo and other software renderers
    auto target = m_backend == GAL_TYPE_OPENGL ? KIGFX::TARGET_CACHED : KIGFX::TARGET_NONCACHED;

    for( int i = 0; i < KIGFX::VIEW::VIEW_MAX_LAYERS; i++ )
        m_view->SetLayerTarget( i, target );

    // Bitmaps are drawn on a non cached GAL layer:
    m_view->SetLayerTarget( LAYER_DRAW_BITMAPS, KIGFX::TARGET_NONCACHED );

    // Some draw layers need specific settings
    m_view->SetLayerTarget( LAYER_GP_OVERLAY, KIGFX::TARGET_OVERLAY );
    m_view->SetLayerDisplayOnly( LAYER_GP_OVERLAY );

    m_view->SetLayerTarget( LAYER_SELECT_OVERLAY, KIGFX::TARGET_OVERLAY );
    m_view->SetLayerDisplayOnly( LAYER_SELECT_OVERLAY );

    m_view->SetLayerTarget( LAYER_DRAWINGSHEET, KIGFX::TARGET_NONCACHED );
    m_view->SetLayerDisplayOnly( LAYER_DRAWINGSHEET );

    m_view->SetLayerTarget( LAYER_SELECTION_SHADOWS, KIGFX::TARGET_OVERLAY );
}