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

// Forward declarations
class CHEM_SYMBOL;
class CHEM_CONNECTION;
class CHEM_LABEL;

/**
 * Enum for chemical schematic layers
 */
enum CHEM_LAYER_ID
{
    CHEM_LAYER_BACKGROUND,
    CHEM_LAYER_GRID,
    CHEM_LAYER_CONNECTIONS,
    CHEM_LAYER_SYMBOLS,
    CHEM_LAYER_LABELS,
    CHEM_LAYER_SELECTION,
    CHEM_LAYER_ID_COUNT
};

/**
 * Class to store display options for the chemical schematic
 */
class CHEM_DISPLAY_OPTIONS
{
public:
    bool m_showGrid;
    bool m_showLabels;
    bool m_showConnections;
    bool m_showSymbols;
    
    CHEM_DISPLAY_OPTIONS() :
        m_showGrid( true ),
        m_showLabels( true ),
        m_showConnections( true ),
        m_showSymbols( true )
    {
    }
};

/**
 * Class to represent a chemical process flow diagram
 */
class CHEM_SCHEMATIC
{
public:
    /**
     * Constructor
     */
    CHEM_SCHEMATIC();
    
    /**
     * Destructor
     */
    ~CHEM_SCHEMATIC();
    
    /**
     * Clear the schematic
     */
    void Clear();
    
    /**
     * Add a symbol to the schematic
     * @param aSymbol - The symbol to add
     */
    void AddSymbol( CHEM_SYMBOL* aSymbol );
    
    /**
     * Remove a symbol from the schematic
     * @param aSymbol - The symbol to remove
     */
    void RemoveSymbol( CHEM_SYMBOL* aSymbol );
    
    /**
     * Get all symbols in the schematic
     * @return Vector of symbols
     */
    const std::vector<CHEM_SYMBOL*>& GetSymbols() const;
    
    /**
     * Add a connection to the schematic
     * @param aConnection - The connection to add
     */
    void AddConnection( CHEM_CONNECTION* aConnection );
    
    /**
     * Remove a connection from the schematic
     * @param aConnection - The connection to remove
     */
    void RemoveConnection( CHEM_CONNECTION* aConnection );
    
    /**
     * Get all connections in the schematic
     * @return Vector of connections
     */
    const std::vector<CHEM_CONNECTION*>& GetConnections() const;
    
    /**
     * Add a label to the schematic
     * @param aLabel - The label to add
     */
    void AddLabel( CHEM_LABEL* aLabel );
    
    /**
     * Remove a label from the schematic
     * @param aLabel - The label to remove
     */
    void RemoveLabel( CHEM_LABEL* aLabel );
    
    /**
     * Get all labels in the schematic
     * @return Vector of labels
     */
    const std::vector<CHEM_LABEL*>& GetLabels() const;
    
    /**
     * Set the schematic filename
     * @param aFilename - The filename
     */
    void SetFilename( const wxFileName& aFilename );
    
    /**
     * Get the schematic filename
     * @return The filename
     */
    const wxFileName& GetFilename() const;
    
    /**
     * Set the schematic title
     * @param aTitle - The title
     */
    void SetTitle( const wxString& aTitle );
    
    /**
     * Get the schematic title
     * @return The title
     */
    const wxString& GetTitle() const;
    
    /**
     * Set whether the schematic has been modified
     * @param aModified - True if modified, false otherwise
     */
    void SetModified( bool aModified );
    
    /**
     * Check if the schematic has been modified
     * @return True if modified, false otherwise
     */
    bool IsModified() const;
    
private:
    std::vector<CHEM_SYMBOL*> m_symbols;
    std::vector<CHEM_CONNECTION*> m_connections;
    std::vector<CHEM_LABEL*> m_labels;
    wxFileName m_filename;
    wxString m_title;
    bool m_modified;
};

#endif // CHEM_SCHEMATIC_H 