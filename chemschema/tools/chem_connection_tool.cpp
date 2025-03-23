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

#include "chem_connection_tool.h"
#include "chem_edit_frame.h"
#include "chem_schematic.h"
#include "chem_connection.h"
#include "chem_actions.h"
#include <tool/tool_manager.h>
#include <view/view.h>
#include <view/view_controls.h>

CHEM_CONNECTION_TOOL::CHEM_CONNECTION_TOOL() :
    TOOL_INTERACTIVE( "chemschema.ConnectionTool" ),
    m_frame( nullptr ),
    m_currentConnection( nullptr ),
    m_inConnection( false )
{
}

bool CHEM_CONNECTION_TOOL::Init()
{
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();

    // Register connection tool actions
    Go( &CHEM_CONNECTION_TOOL::StartConnection,      CHEM_ACTIONS::startConnection.MakeEvent() );
    Go( &CHEM_CONNECTION_TOOL::AddConnectionSegment, CHEM_ACTIONS::addSegment.MakeEvent() );
    Go( &CHEM_CONNECTION_TOOL::FinishConnection,     CHEM_ACTIONS::finishConnection.MakeEvent() );
    Go( &CHEM_CONNECTION_TOOL::CancelConnection,     ACTIONS::cancelTool.MakeEvent() );

    return true;
}

void CHEM_CONNECTION_TOOL::Reset( RESET_REASON aReason )
{
    if( m_currentConnection )
    {
        delete m_currentConnection;
        m_currentConnection = nullptr;
    }
    m_inConnection = false;
}

int CHEM_CONNECTION_TOOL::StartConnection( const TOOL_EVENT& aEvent )
{
    // Start connection handler
    return 0;
}

int CHEM_CONNECTION_TOOL::AddConnectionSegment( const TOOL_EVENT& aEvent )
{
    // Add segment handler
    return 0;
}

int CHEM_CONNECTION_TOOL::FinishConnection( const TOOL_EVENT& aEvent )
{
    // Finish connection handler
    return 0;
}

int CHEM_CONNECTION_TOOL::CancelConnection( const TOOL_EVENT& aEvent )
{
    // Cancel connection handler
    Reset( TOOL_BASE::CANCELLED );
    return 0;
} 