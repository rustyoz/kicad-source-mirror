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

#include <pgm_base.h>
#include <settings/settings_manager.h>
#include <chemschema_settings.h>
#include "chem_base_frame.h"
#include "chem_draw_panel.h"
#include "chem_view.h"
#include "chem_screen.h"

BEGIN_EVENT_TABLE( CHEM_BASE_FRAME, EDA_DRAW_FRAME )
END_EVENT_TABLE()

CHEM_BASE_FRAME::CHEM_BASE_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aWindowType,
                                 const wxString& aTitle, const wxPoint& aPosition,
                                 const wxSize& aSize, long aStyle, const wxString& aFrameName ) :
        EDA_DRAW_FRAME( aKiway, aParent, aWindowType, aTitle, aPosition, aSize,
                       aStyle, aFrameName )
{
    if( ( aStyle & wxFRAME_NO_TASKBAR ) == 0 )
        createCanvas();
}

CHEM_BASE_FRAME::~CHEM_BASE_FRAME()
{
}

void CHEM_BASE_FRAME::createCanvas()
{
    m_canvasType = loadCanvasTypeSetting();

    SetCanvas( new CHEM_DRAW_PANEL( this, wxID_ANY, wxPoint( 0, 0 ), m_frameSize,
                                   GetGalDisplayOptions(), m_canvasType ) );

    ActivateGalCanvas();
}

CHEM_DRAW_PANEL* CHEM_BASE_FRAME::GetCanvas() const
{
    return static_cast<CHEM_DRAW_PANEL*>( EDA_DRAW_FRAME::GetCanvas() );
}


CHEM_SCREEN* CHEM_BASE_FRAME::GetScreen() const
{
    return static_cast<CHEM_SCREEN*>( EDA_DRAW_FRAME::GetScreen() );
}

CHEMSCHEMA_SETTINGS* CHEM_BASE_FRAME::chemconfig() const
{
    return dynamic_cast<CHEMSCHEMA_SETTINGS*>( config() );
}

void CHEM_BASE_FRAME::LoadSettings( APP_SETTINGS_BASE* aCfg )
{
    EDA_DRAW_FRAME::LoadSettings( aCfg );
}

void CHEM_BASE_FRAME::SaveSettings( APP_SETTINGS_BASE* aCfg )
{
    EDA_DRAW_FRAME::SaveSettings( aCfg );
}

COLOR4D CHEM_BASE_FRAME::GetDrawBgColor() const
{
    return GetColorSettings()->GetColor( LAYER_CHEM_BACKGROUND );
}

void CHEM_BASE_FRAME::SetPageSettings( const PAGE_INFO& aPageSettings )
{
    if( GetScreen() )
        GetScreen()->SetPageSettings( aPageSettings );
}

const PAGE_INFO& CHEM_BASE_FRAME::GetPageSettings() const
{
    wxASSERT( GetScreen() );
    return GetScreen()->GetPageSettings();
}

const VECTOR2I CHEM_BASE_FRAME::GetPageSizeIU() const
{
    wxASSERT( GetScreen() );

    const PAGE_INFO& pageInfo = GetScreen()->GetPageSettings();
    return VECTOR2I( pageInfo.GetWidthIU(), pageInfo.GetHeightIU() );
} 