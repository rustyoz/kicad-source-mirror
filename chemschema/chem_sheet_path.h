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

#ifndef CHEM_SHEET_PATH_H
#define CHEM_SHEET_PATH_H

#include <vector>
#include <wx/string.h>
#include <kiid.h>
#include <eda_item.h>
#include <map>
#include <optional>

class CHEM_SHEET;
class CHEM_SCREEN;
class CHEM_ITEM;

/**
 * A class to handle the hierarchical path of sheets in a chemical schematic.
 */
class CHEM_SHEET_PATH
{
public:
    CHEM_SHEET_PATH();
    CHEM_SHEET_PATH( const CHEM_SHEET_PATH& aOther );
    ~CHEM_SHEET_PATH();

    CHEM_SHEET_PATH& operator=( const CHEM_SHEET_PATH& aOther );
    CHEM_SHEET_PATH operator+( const CHEM_SHEET_PATH& aOther ) const;
    bool operator==( const CHEM_SHEET_PATH& aOther ) const;

    /**
     * Get the number of sheets in this path.
     * @return The number of sheets.
     */
    size_t size() const { return m_sheets.size(); }

    /**
     * Check if this path is empty.
     * @return true if empty.
     */
    bool empty() const { return m_sheets.empty(); }

    /**
     * Get a sheet at a specific index.
     * @param aIndex - The index of the sheet to get.
     * @return The sheet at the index.
     */
    CHEM_SHEET* at( size_t aIndex ) const { return m_sheets.at( aIndex ); }

    /**
     * Get the last sheet in this path.
     * @return The last sheet.
     */
    CHEM_SHEET* Last() const;

    /**
     * Get the last screen in this path.
     * @return The last screen.
     */
    CHEM_SCREEN* LastScreen() const;

    /**
     * Get the path as a string.
     * @return The path string.
     */
    wxString PathAsString() const;

    /**
     * Get the path as a KIID_PATH.
     * @return The path.
     */
    KIID_PATH Path() const;

    /**
     * Get the path in human readable format.
     * @param aUseShortRootName - Whether to use short root name.
     * @param aStripTrailingSeparator - Whether to strip trailing separator.
     * @return The human readable path.
     */
    wxString PathHumanReadable( bool aUseShortRootName = false,
                               bool aStripTrailingSeparator = false ) const;

    /**
     * Get an item by its KIID.
     * @param aID - The KIID of the item to get.
     * @return The item if found, nullptr otherwise.
     */
    EDA_ITEM* GetItem( const KIID& aID ) const;

    /**
     * Get the current hash of this path.
     * @return The current hash.
     */
    size_t GetCurrentHash() const { return m_current_hash; }

    /**
     * Set the virtual page number.
     * @param aPageNumber - The virtual page number to set.
     */
    void SetVirtualPageNumber( int aPageNumber ) { m_virtualPageNumber = aPageNumber; }

    /**
     * Get the virtual page number.
     * @return The virtual page number.
     */
    int GetVirtualPageNumber() const { return m_virtualPageNumber; }

    /**
     * Get the page number.
     * @return The page number.
     */
    wxString GetPageNumber() const;

    /**
     * Set the page number.
     * @param aPageNumber - The page number to set.
     */
    void SetPageNumber( const wxString& aPageNumber );

private:
    std::vector<CHEM_SHEET*> m_sheets;
    int m_virtualPageNumber;
    size_t m_current_hash;
    wxString m_cached_page_number;
};

/**
 * A container for handling CHEM_SHEET_PATH objects in a flattened hierarchy.
 *
 * CHEM_SHEET objects are not unique, there can be many sheets with the same filename and
 * that share the same CHEM_SCREEN reference. Each sheet may be shared between these sheets.
 */
class CHEM_SHEET_LIST : public std::vector<CHEM_SHEET_PATH>
{
public:
    /**
     * Construct a flattened list of CHEM_SHEET_PATH objects from \a aSheet.
     *
     * If aSheet == NULL, then this is an empty hierarchy which the user can populate.
     */
    CHEM_SHEET_LIST( CHEM_SHEET* aSheet = nullptr );

    ~CHEM_SHEET_LIST() {}

    /**
     * Check the entire hierarchy for any modifications.
     *
     * @return True if the hierarchy is modified otherwise false.
     */
    bool IsModified() const;

    void ClearModifyStatus();

    /**
     * Fetch a CHEM_ITEM by ID.
     *
     * Also returns the sheet the item was found on in \a aPathOut.
     */
    CHEM_ITEM* GetItem( const KIID& aID, CHEM_SHEET_PATH* aPathOut = nullptr ) const;

    /**
     * Fill an item cache for temporary use when many items need to be fetched.
     */
    void FillItemMap( std::map<KIID, EDA_ITEM*>& aMap );

    /**
     * Build the list of sheets and their sheet path from \a aSheet.
     *
     * If \a aSheet is the root sheet, the full sheet path and sheet list are built.
     *
     * @param aSheet is the starting sheet from which the list is built, or NULL
     *               indicating that g_RootSheet should be used.
     * @throw std::bad_alloc if the memory for the sheet path list could not be allocated.
     */
    void BuildSheetList( CHEM_SHEET* aSheet, bool aCheckIntegrity );

    /**
     * Return a pointer to the first CHEM_SHEET_PATH object (not necessarily the only one)
     * matching the provided path. Returns nullptr if not found.
     */
    CHEM_SHEET_PATH* FindSheetForPath( const CHEM_SHEET_PATH* aPath );

    /**
     * Return the first CHEM_SHEET_PATH object (not necessarily the only one) using a particular
     * screen.
     */
    CHEM_SHEET_PATH FindSheetForScreen( const CHEM_SCREEN* aScreen );

    /**
     * Return a CHEM_SHEET_LIST with a copy of all the CHEM_SHEET_PATH using a particular screen.
     */
    CHEM_SHEET_LIST FindAllSheetsForScreen( const CHEM_SCREEN* aScreen ) const;

    /**
     * Test if a sheet exists in the hierarchy.
     *
     * @param aSheet is the sheet to test for.
     * @return true if the sheet exists in the hierarchy.
     */
    bool ContainsSheet( const CHEM_SHEET* aSheet ) const;

private:
    CHEM_SHEET_PATH m_currentSheetPath;  ///< Current sheet path being built
};

#endif // CHEM_SHEET_PATH_H 