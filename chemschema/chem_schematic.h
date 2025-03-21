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

#ifndef CHEM_SCHEMATIC_H
#define CHEM_SCHEMATIC_H

#include <vector>
#include <wx/string.h>
#include <wx/filename.h>
#include <schematic.h>
#include "chem_item.h"
#include "chem_screen.h"
#include "chem_sheet_path.h"
#include "chem_symbol.h"
#include "chem_connection.h"
#include "chem_label.h"

class CHEM_SHEET;

/**
 * Class CHEM_SCHEMATIC
 * Represents a chemical process flow diagram.
 */
class CHEM_SCHEMATIC : public SCHEMATIC
{
public:
    CHEM_SCHEMATIC( PROJECT* aPrj );
    CHEM_SCHEMATIC( const CHEM_SCHEMATIC& other );
    virtual ~CHEM_SCHEMATIC();

    virtual wxString GetClass() const override
    {
        return wxT( "CHEM_SCHEMATIC" );
    }

    /// Initialize this schematic to a blank one, unloading anything existing.
    void Reset();

    /// A simple test if the schematic is loaded, not a complete one
    bool IsValid() const
    {
        return m_rootSheet != nullptr;
    }

    /**
     * Initialize the schematic with a new root sheet.
     *
     * This is typically done by calling a file loader that returns the new root sheet
     * As a side-effect, takes care of some post-load initialization.
     *
     * @param aRootSheet is the new root sheet for this schematic.
     */
    void SetRoot( CHEM_SHEET* aRootSheet );

    /// Helper to retrieve the screen of the root sheet
    CHEM_SCREEN* RootScreen() const;

    /// Helper to retrieve the filename from the root sheet screen
    wxString GetFileName() const;

    CHEM_SHEET_PATH& CurrentSheet() const
    {
        return *m_currentSheet;
    }

    void SetCurrentSheet( const CHEM_SHEET_PATH& aPath )
    {
        *m_currentSheet = aPath;
    }

    CHEM_SHEET& Root() const
    {
        return *m_rootSheet;
    }

    /**
     * Return the full schematic flattened hierarchical sheet list.
     */
    CHEM_SHEET_LIST Hierarchy() const;

    void RefreshHierarchy();

    /**
     * Test if the schematic is a complex hierarchy.
     *
     * @return true if the schematic is a complex hierarchy or false if it's a simple hierarchy.
     */
    bool IsComplexHierarchy() const;

    // File operations
    bool SaveFile( const wxString& aFileName );
    bool LoadFile( const wxString& aFileName );

    // Item management
    void Add( CHEM_ITEM* aItem );
    void Remove( CHEM_ITEM* aItem );
    void UpdateView();
    void Clear();

    // Symbol management
    void AddSymbol( CHEM_SYMBOL* aSymbol );
    void RemoveSymbol( CHEM_SYMBOL* aSymbol );
    const std::vector<CHEM_SYMBOL*>& GetSymbols() const;

    // Connection management
    void AddConnection( CHEM_CONNECTION* aConnection );
    void RemoveConnection( CHEM_CONNECTION* aConnection );
    const std::vector<CHEM_CONNECTION*>& GetConnections() const;

    // Label management
    void AddLabel( CHEM_LABEL* aLabel );
    void RemoveLabel( CHEM_LABEL* aLabel );
    const std::vector<CHEM_LABEL*>& GetLabels() const;

    // File and title management
    void SetFilename( const wxFileName& aFilename );
    const wxFileName& GetFilename() const;
    void SetTitle( const wxString& aTitle );
    const wxString& GetTitle() const;
    void SetModified( bool aModified );
    bool IsModified() const;

protected:
    CHEM_SHEET* m_rootSheet;               ///< Top-level sheet in hierarchy
    CHEM_SHEET_PATH* m_currentSheet;       ///< Currently active sheet
    std::vector<CHEM_SYMBOL*> m_symbols;   ///< List of symbols in the schematic
    std::vector<CHEM_CONNECTION*> m_connections; ///< List of connections in the schematic
    std::vector<CHEM_LABEL*> m_labels;     ///< List of labels in the schematic
    wxFileName m_filename;                 ///< The filename of the schematic
    wxString m_title;                      ///< The title of the schematic
    bool m_modified;                       ///< Whether the schematic has been modified
};

#endif // CHEM_SCHEMATIC_H 