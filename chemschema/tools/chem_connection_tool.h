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

#ifndef CHEM_CONNECTION_TOOL_H
#define CHEM_CONNECTION_TOOL_H

#include <tool/tool_interactive.h>
#include "chem_edit_frame.h"

class CHEM_EDIT_FRAME;
class CHEM_CONNECTION;

/**
 * Tool for creating and editing chemical connections in the chemical schematic editor.
 */
class CHEM_CONNECTION_TOOL : public TOOL_INTERACTIVE
{
public:
    CHEM_CONNECTION_TOOL();
    ~CHEM_CONNECTION_TOOL() override { }

    /// @copydoc TOOL_INTERACTIVE::Init()
    bool Init() override;

    /// @copydoc TOOL_INTERACTIVE::Reset()
    void Reset( RESET_REASON aReason ) override;

    /**
     * Start a new connection.
     */
    int StartConnection( const TOOL_EVENT& aEvent );

    /**
     * Add a segment to the current connection.
     */
    int AddConnectionSegment( const TOOL_EVENT& aEvent );

    /**
     * Finish the current connection.
     */
    int FinishConnection( const TOOL_EVENT& aEvent );

    /**
     * Cancel the current connection.
     */
    int CancelConnection( const TOOL_EVENT& aEvent );

private:
    CHEM_EDIT_FRAME* m_frame;
    CHEM_CONNECTION* m_currentConnection;
    bool m_inConnection;
};

#endif // CHEM_CONNECTION_TOOL_H 