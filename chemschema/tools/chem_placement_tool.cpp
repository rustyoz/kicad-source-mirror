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

#include "chem_placement_tool.h"
#include "chem_edit_frame.h"
#include "chem_schematic.h"
#include "chem_symbol.h"
#include "chem_label.h"
#include "chem_junction.h"
#include "chem_actions.h"
#include <tool/tool_manager.h>
#include <view/view.h>
#include <view/view_controls.h>

CHEM_PLACEMENT_TOOL::CHEM_PLACEMENT_TOOL() :
    TOOL_INTERACTIVE( "chemschema.PlacementTool" ),
    m_frame( nullptr ),
    m_inPlacement( false )
{
}

bool CHEM_PLACEMENT_TOOL::Init()
{
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();

    // Register tool actions
    Go( &CHEM_PLACEMENT_TOOL::PlaceChemItem,   ACTIONS::placeTool.MakeEvent() );
    Go( &CHEM_PLACEMENT_TOOL::PlaceSymbol,     CHEM_ACTIONS::placeSymbol.MakeEvent() );
    Go( &CHEM_PLACEMENT_TOOL::PlaceLabel,      CHEM_ACTIONS::placeLabel.MakeEvent() );
    Go( &CHEM_PLACEMENT_TOOL::PlaceJunction,   CHEM_ACTIONS::placeJunction.MakeEvent() );

    return true;
}

void CHEM_PLACEMENT_TOOL::Reset( RESET_REASON aReason )
{
    m_inPlacement = false;
}

int CHEM_PLACEMENT_TOOL::PlaceChemItem( const TOOL_EVENT& aEvent )
{
    // Generic placement handler
    return 0;
}

int CHEM_PLACEMENT_TOOL::PlaceSymbol( const TOOL_EVENT& aEvent )
{
    // Symbol placement handler
    return 0;
}

int CHEM_PLACEMENT_TOOL::PlaceLabel( const TOOL_EVENT& aEvent )
{
    // Label placement handler
    return 0;
}

int CHEM_PLACEMENT_TOOL::PlaceJunction( const TOOL_EVENT& aEvent )
{
    // Junction placement handler
    return 0;
} 