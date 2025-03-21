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

#ifndef CHEM_CONNECTION_H
#define CHEM_CONNECTION_H

#include <wx/string.h>
#include <memory>
#include <vector>

#include <connection_graph.h>
#include <chem_sheet_path.h>
#include <chem_item.h>

class CHEM_ITEM;
class CHEM_PIN;
class CHEM_SYMBOL;
class CONNECTION_SUBGRAPH;
class MSG_PANEL_ITEM;

/**
 * Class CHEM_CONNECTION
 * handles the connection information for chemical schematic items.
 * It manages net names, bus connections, and connection types.
 */
class CHEM_CONNECTION : public CHEM_ITEM
{
public:
    CHEM_CONNECTION( CHEM_ITEM* aParent, const CHEM_SHEET_PATH& aPath );
    CHEM_CONNECTION( CONNECTION_GRAPH* aGraph );
    CHEM_CONNECTION( CHEM_CONNECTION& aOther );
    ~CHEM_CONNECTION();

    bool operator==( const CHEM_CONNECTION& aOther ) const;
    bool operator!=( const CHEM_CONNECTION& aOther ) const;

    void Reset();
    void Clone( const CHEM_CONNECTION& aOther );

    /**
     * Configure the connection from a label string.
     * @param aLabel The label text to parse
     */
    void ConfigureFromLabel( const wxString& aLabel );

    /**
     * Set the driver item for this connection.
     * @param aItem The item that drives this connection
     */
    void SetDriver( CHEM_ITEM* aItem );

    /**
     * Set the sheet path for this connection.
     * @param aSheet The sheet path to set
     */
    void SetSheet( CHEM_SHEET_PATH aSheet );

    /**
     * Set the prefix for this connection's name.
     * @param aPrefix The prefix to set
     */
    void SetPrefix( const wxString& aPrefix );

    /**
     * Set the suffix for this connection's name.
     * @param aSuffix The suffix to set
     */
    void SetSuffix( const wxString& aSuffix );

    /**
     * Get the name of this connection.
     * @param aIgnoreSheet Whether to ignore the sheet path in the name
     * @return The connection name
     */
    wxString Name( bool aIgnoreSheet = false ) const;

    /**
     * Get the net name for this connection.
     * @return The net name
     */
    wxString GetNetName() const;

    /**
     * Check if this connection is a driver.
     * @return true if this connection is a driver
     */
    bool IsDriver() const;

    /**
     * Check if the driver has changed.
     * @return true if the driver has changed
     */
    bool HasDriverChanged() const;

    /**
     * Clear the driver changed flag.
     */
    void ClearDriverChanged();

    /**
     * Check if a label is a bus label.
     * @param aLabel The label to check
     * @return true if the label is a bus label
     */
    static bool IsBusLabel( const wxString& aLabel );

    /**
     * Check if a label might be a bus label.
     * @param aLabel The label to check
     * @return true if the label might be a bus label
     */
    static bool MightBeBusLabel( const wxString& aLabel );

    /**
     * Get all members of this connection.
     * @return Vector of all members
     */
    const std::vector<std::shared_ptr<CHEM_CONNECTION>>& Members() const { return m_members; }

    /**
     * Get all members including nested bus members.
     * @return Vector of all members including nested ones
     */
    const std::vector<std::shared_ptr<CHEM_CONNECTION>> AllMembers() const;

    /**
     * Check if this connection is a subset of another connection.
     * @param aOther The other connection to check against
     * @return true if this is a subset of the other connection
     */
    bool IsSubsetOf( CHEM_CONNECTION* aOther ) const;

    /**
     * Check if this connection is a member of a bus.
     * @param aOther The bus connection to check against
     * @return true if this is a member of the bus
     */
    bool IsMemberOfBus( CHEM_CONNECTION* aOther ) const;

    /**
     * Append connection information to a message panel.
     * @param aList The list to append to
     */
    void AppendInfoToMsgPanel( std::vector<MSG_PANEL_ITEM>& aList ) const;

    // Getters
    CHEM_ITEM* Parent() const { return m_parent; }
    CHEM_ITEM* Driver() const { return m_driver; }
    const CHEM_SHEET_PATH& Sheet() const { return m_sheet; }
    wxString LocalName() const { return m_local_name; }
    wxString Prefix() const { return m_prefix; }
    wxString Suffix() const { return m_suffix; }
    wxString FullLocalName() const { return m_local_prefix + m_local_name; }
    CONNECTION_TYPE Type() const { return m_type; }
    bool IsBus() const { return m_type == CONNECTION_TYPE::BUS; }
    bool IsNet() const { return m_type == CONNECTION_TYPE::NET; }
    bool IsNone() const { return m_type == CONNECTION_TYPE::NONE; }
    int NetCode() const { return m_net_code; }
    int BusCode() const { return m_bus_code; }
    int SubgraphCode() const { return m_subgraph_code; }
    int VectorStart() const { return m_vector_start; }
    int VectorEnd() const { return m_vector_end; }
    int VectorIndex() const { return m_vector_index; }
    wxString VectorPrefix() const { return m_vector_prefix; }
    CONNECTION_GRAPH* Graph() const { return m_graph; }

    // Setters
    void SetName( const wxString& aName ) { m_name = aName; recacheName(); }
    void SetGraph( CONNECTION_GRAPH* aGraph ) { m_graph = aGraph; }
    void SetNetCode( int aCode ) { m_net_code = aCode; }
    void SetBusCode( int aCode ) { m_bus_code = aCode; }
    void SetSubgraphCode( int aCode ) { m_subgraph_code = aCode; }
    void SetVectorStart( int aStart ) { m_vector_start = aStart; }
    void SetVectorEnd( int aEnd ) { m_vector_end = aEnd; }
    void SetVectorIndex( int aIndex ) { m_vector_index = aIndex; }
    void SetVectorPrefix( const wxString& aPrefix ) { m_vector_prefix = aPrefix; }

private:
    void recacheName();

    CHEM_SHEET_PATH m_sheet;           ///< The sheet path for this connection
    CHEM_SHEET_PATH m_local_sheet;     ///< The local sheet path
    CHEM_ITEM* m_parent;               ///< The parent item
    CHEM_ITEM* m_driver;               ///< The item that drives this connection
    CHEM_ITEM* m_lastDriver;           ///< The previous driver
    CONNECTION_GRAPH* m_graph;         ///< The connection graph

    CONNECTION_TYPE m_type;            ///< The type of connection (NET, BUS, etc.)
    wxString m_name;                   ///< The connection name
    wxString m_local_name;             ///< The local name
    wxString m_local_prefix;           ///< The local prefix
    wxString m_cached_name;            ///< The cached full name
    wxString m_cached_name_with_path;  ///< The cached name with sheet path
    wxString m_prefix;                 ///< The name prefix
    wxString m_bus_prefix;             ///< The bus prefix
    wxString m_suffix;                 ///< The name suffix
    wxString m_vector_prefix;          ///< The vector prefix for bus vectors

    std::vector<std::shared_ptr<CHEM_CONNECTION>> m_members;  ///< The members of this connection

    bool m_dirty;                      ///< Whether the connection needs updating
    int m_net_code;                    ///< The net code
    int m_bus_code;                    ///< The bus code
    int m_subgraph_code;               ///< The subgraph code
    int m_vector_start;                ///< The start of the vector range
    int m_vector_end;                  ///< The end of the vector range
    int m_vector_index;                ///< The current vector index
};

#endif // CHEM_CONNECTION_H 