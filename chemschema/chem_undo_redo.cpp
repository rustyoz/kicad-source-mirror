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

#include "chem_undo_redo.h"
#include "chem_schematic.h"

CHEM_UNDO_REDO::CHEM_UNDO_REDO( CHEM_SCHEMATIC* aSchematic ) :
    m_schematic( aSchematic )
{
}

void CHEM_UNDO_REDO::SaveState( const wxString& aCommand )
{
    // Create a copy of the current state
    std::unique_ptr<CHEM_ITEM> stateCopy( m_schematic->Clone() );

    // Add to undo stack
    m_undoStack.emplace_back( aCommand, std::move( stateCopy ) );

    // Clear redo stack since we can't redo after a new action
    m_redoStack.clear();

    // Limit undo stack size
    while( m_undoStack.size() > MAX_UNDO_ITEMS )
        m_undoStack.pop_front();
}

bool CHEM_UNDO_REDO::Undo()
{
    if( !CanUndo() )
        return false;

    // Save current state to redo stack
    std::unique_ptr<CHEM_ITEM> currentState( m_schematic->Clone() );
    m_redoStack.emplace_back( m_undoStack.back().first, std::move( currentState ) );

    // Restore previous state
    m_schematic->Copy( *m_undoStack.back().second );
    m_undoStack.pop_back();

    return true;
}

bool CHEM_UNDO_REDO::Redo()
{
    if( !CanRedo() )
        return false;

    // Save current state to undo stack
    std::unique_ptr<CHEM_ITEM> currentState( m_schematic->Clone() );
    m_undoStack.emplace_back( m_redoStack.back().first, std::move( currentState ) );

    // Restore next state
    m_schematic->Copy( *m_redoStack.back().second );
    m_redoStack.pop_back();

    return true;
}

void CHEM_UNDO_REDO::ClearCommandHistory()
{
    m_undoStack.clear();
    m_redoStack.clear();
}

wxString CHEM_UNDO_REDO::GetUndoCommandDescription() const
{
    if( CanUndo() )
        return m_undoStack.back().first;
    return wxEmptyString;
}

wxString CHEM_UNDO_REDO::GetRedoCommandDescription() const
{
    if( CanRedo() )
        return m_redoStack.back().first;
    return wxEmptyString;
} 