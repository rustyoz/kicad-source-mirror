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

#ifndef CHEM_PLACEMENT_TOOL_H
#define CHEM_PLACEMENT_TOOL_H

#include <tool/tool_interactive.h>
#include "chem_edit_frame.h"

class CHEM_EDIT_FRAME;

/**
 * Tool for placing chemical elements and symbols in the chemical schematic editor.
 */
class CHEM_PLACEMENT_TOOL : public TOOL_INTERACTIVE
{
public:
    CHEM_PLACEMENT_TOOL();
    ~CHEM_PLACEMENT_TOOL() override { }

    /// @copydoc TOOL_INTERACTIVE::Init()
    bool Init() override;

    /// @copydoc TOOL_INTERACTIVE::Reset()
    void Reset( RESET_REASON aReason ) override;

    /**
     * The main loop for the placement tool.
     */
    int PlaceChemItem( const TOOL_EVENT& aEvent );

    /**
     * Place a chemical symbol.
     */
    int PlaceSymbol( const TOOL_EVENT& aEvent );

    /**
     * Place a chemical label.
     */
    int PlaceLabel( const TOOL_EVENT& aEvent );

    /**
     * Place a chemical junction.
     */
    int PlaceJunction( const TOOL_EVENT& aEvent );

private:
    CHEM_EDIT_FRAME* m_frame;
    bool m_inPlacement;
};

#endif // CHEM_PLACEMENT_TOOL_H 