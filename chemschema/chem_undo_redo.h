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

#ifndef CHEM_UNDO_REDO_H
#define CHEM_UNDO_REDO_H

#include <deque>
#include <memory>
#include "chem_item.h"

class CHEM_SCHEMATIC;

/**
 * Class to handle undo/redo operations in the chemical schematic editor.
 */
class CHEM_UNDO_REDO
{
public:
    /**
     * Constructor
     * @param aSchematic - The chemical schematic to operate on
     */
    CHEM_UNDO_REDO( CHEM_SCHEMATIC* aSchematic );

    /**
     * Save the current state for undo
     * @param aCommand - The command description
     */
    void SaveState( const wxString& aCommand );

    /**
     * Undo the last operation
     * @return True if successful
     */
    bool Undo();

    /**
     * Redo the last undone operation
     * @return True if successful
     */
    bool Redo();

    /**
     * Clear all undo/redo history
     */
    void ClearCommandHistory();

    /**
     * Check if undo is available
     * @return True if undo is available
     */
    bool CanUndo() const { return !m_undoStack.empty(); }

    /**
     * Check if redo is available
     * @return True if redo is available
     */
    bool CanRedo() const { return !m_redoStack.empty(); }

    /**
     * Get the description of the next undo command
     * @return Command description
     */
    wxString GetUndoCommandDescription() const;

    /**
     * Get the description of the next redo command
     * @return Command description
     */
    wxString GetRedoCommandDescription() const;

private:
    CHEM_SCHEMATIC* m_schematic;
    std::deque<std::pair<wxString, std::unique_ptr<CHEM_ITEM>>> m_undoStack;
    std::deque<std::pair<wxString, std::unique_ptr<CHEM_ITEM>>> m_redoStack;
    static constexpr size_t MAX_UNDO_ITEMS = 50;
};

#endif // CHEM_UNDO_REDO_H 