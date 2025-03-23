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
class CHEM_SCHEMATIC;

/**
 * Class CHEM_SCHEMATIC_LISTENER
 * Interface for objects that want to receive notifications about changes to chemical schematics.
 */
class CHEM_SCHEMATIC_LISTENER
{
public:
    virtual ~CHEM_SCHEMATIC_LISTENER() {}

    /**
     * Called when chemical items are added to the schematic
     * @param aSchematic The schematic that was modified
     * @param aItems The items that were added
     */
    virtual void OnChemItemsAdded( CHEM_SCHEMATIC& aSchematic, std::vector<CHEM_ITEM*>& aItems ) {}

    /**
     * Called when chemical items are removed from the schematic
     * @param aSchematic The schematic that was modified
     * @param aItems The items that were removed
     */
    virtual void OnChemItemsRemoved( CHEM_SCHEMATIC& aSchematic, std::vector<CHEM_ITEM*>& aItems ) {}

    /**
     * Called when chemical items are modified in the schematic
     * @param aSchematic The schematic that was modified
     * @param aItems The items that were modified
     */
    virtual void OnChemItemsChanged( CHEM_SCHEMATIC& aSchematic, std::vector<CHEM_ITEM*>& aItems ) {}

    /**
     * Called when the current chemical sheet changes
     * This is called when the user changes to a new sheet, not when a sheet is altered.
     * Sheet alteration events will call OnChemItems*
     * @param aSchematic The schematic whose sheet changed
     */
    virtual void OnChemSheetChanged( CHEM_SCHEMATIC& aSchematic ) {}
};

/**
 * Class CHEM_SCHEMATIC
 * Represents a chemical process flow diagram. Acts as a specialized interface to SCHEMATIC
 * that handles chemical-specific items while maintaining compatibility with the base schematic system.
 */
class CHEM_SCHEMATIC : public SCHEMATIC
{
public:
    CHEM_SCHEMATIC( PROJECT* aPrj );
    virtual ~CHEM_SCHEMATIC();

    virtual wxString GetClass() const override
    {
        return wxT( "CHEM_SCHEMATIC" );
    }

    /**
     * Initialize this schematic to a blank one, unloading anything existing.
     */
    void Reset();

    /**
     * Test if the schematic is valid and loaded
     * @return true if the schematic has a root sheet
     */
    bool IsValid() const
    {
        return m_rootSheet != nullptr;
    }

    /**
     * Initialize the schematic with a new root sheet.
     * @param aRootSheet is the new root sheet for this schematic.
     */
    void SetRoot( CHEM_SHEET* aRootSheet );

    /**
     * Get the root screen of the schematic
     * @return Pointer to the root CHEM_SCREEN
     */
    CHEM_SCREEN* RootScreen() const;

    /**
     * Get the filename from the root sheet screen
     * @return The filename as a string
     */
    wxString GetFileName() const;

    /**
     * Get the current sheet path
     * @return Reference to the current CHEM_SHEET_PATH
     */
    CHEM_SHEET_PATH& CurrentSheet() const
    {
        return *m_currentSheet;
    }

    /**
     * Set the current sheet path
     * @param aPath The new sheet path
     */
    void SetCurrentSheet( const CHEM_SHEET_PATH& aPath )
    {
        *m_currentSheet = aPath;
    }

    /**
     * Get the root sheet
     * @return Reference to the root CHEM_SHEET
     */
    CHEM_SHEET& Root() const
    {
        return *m_rootSheet;
    }

    /**
     * Get the full schematic flattened hierarchical sheet list
     * @return The CHEM_SHEET_LIST containing all sheets
     */
    CHEM_SHEET_LIST Hierarchy() const;

    /**
     * Refresh the hierarchy after changes
     */
    void RefreshHierarchy();

    /**
     * Test if the schematic is a complex hierarchy
     * @return true if any screen is referenced by multiple sheets
     */
    bool IsComplexHierarchy() const;

    // File operations
    bool SaveFile( const wxString& aFileName );
    bool LoadFile( const wxString& aFileName );


    const std::vector<CHEM_LABEL*>& GetLabels() const;

    // File and title management
    void SetFilename( const wxFileName& aFilename );
    const wxFileName& GetFilename() const;
    void SetTitle( const wxString& aTitle );
    const wxString& GetTitle() const;
    void SetModified( bool aModified );
    bool IsModified() const;

    /**
     * Get an item by its type and ID
     * @tparam T The type of item to retrieve
     * @param aId The ID of the item
     * @return Pointer to the item if found and of correct type, nullptr otherwise
     */
    template<typename T>
    T* GetItem( const KIID& aId ) const
    {
        EDA_ITEM* item = SCHEMATIC::GetItem( aId );
        return dynamic_cast<T*>( item );
    }

    /**
     * Add a listener to receive notifications about chemical schematic changes.
     * The schematic does not take ownership of the listener object.
     * @param aListener The listener to add
     */
    void AddChemListener( CHEM_SCHEMATIC_LISTENER* aListener );

    /**
     * Remove a listener from receiving notifications.
     * @param aListener The listener to remove
     */
    void RemoveChemListener( CHEM_SCHEMATIC_LISTENER* aListener );

    /**
     * Remove all chemical schematic listeners.
     */
    void RemoveAllChemListeners();

    /**
     * Notify listeners that chemical items were added.
     * @param aItems The items that were added
     */
    void OnChemItemsAdded( std::vector<CHEM_ITEM*>& aItems );

    /**
     * Notify listeners that chemical items were removed.
     * @param aItems The items that were removed
     */
    void OnChemItemsRemoved( std::vector<CHEM_ITEM*>& aItems );

    /**
     * Notify listeners that chemical items were changed.
     * @param aItems The items that were changed
     */
    void OnChemItemsChanged( std::vector<CHEM_ITEM*>& aItems );

    /**
     * Notify listeners that the current chemical sheet changed.
     */
    void OnChemSheetChanged();

protected:
    template <typename Func, typename... Args>
    void InvokeChemListeners( Func&& aFunc, Args&&... args )
    {
        for( auto&& l : m_chemListeners )
            ( l->*aFunc )( std::forward<Args>( args )... );
    }

protected:
    CHEM_SHEET* m_rootSheet;               ///< Top-level sheet in hierarchy
    CHEM_SHEET_PATH* m_currentSheet;       ///< Currently active sheet
    wxFileName m_filename;                 ///< The filename of the schematic
    wxString m_title;                      ///< The title of the schematic
    bool m_modified;                       ///< Modification flag
    std::vector<CHEM_SCHEMATIC_LISTENER*> m_chemListeners; ///< Chemical schematic change listeners


};

#endif // CHEM_SCHEMATIC_H 