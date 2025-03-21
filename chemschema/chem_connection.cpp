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

#include <wx/tokenzr.h>
#include <connection_graph.h>
#include <chem_symbol.h>
#include <chem_pin.h>
#include <chem_screen.h>
#include <project/net_settings.h>
#include <advanced_config.h>
#include <string_utils.h>
#include <chem_connection.h>

CHEM_CONNECTION::CHEM_CONNECTION( CHEM_ITEM* aParent, const CHEM_SHEET_PATH& aPath ) :
        CHEM_ITEM( aParent, CHEM_CONNECTION_T ),
        m_sheet( aPath ),
        m_local_sheet( aPath ),
        m_parent( aParent ),
        m_driver( nullptr ),
        m_graph( nullptr )
{
    Reset();
}


CHEM_CONNECTION::CHEM_CONNECTION( CONNECTION_GRAPH* aGraph ) :
        CHEM_ITEM( nullptr, CHEM_CONNECTION_T ),
        m_sheet( CHEM_SHEET_PATH() ),
        m_local_sheet( CHEM_SHEET_PATH() ),
        m_parent( nullptr ),
        m_driver( nullptr ),
        m_graph( aGraph )
{
    Reset();
}


CHEM_CONNECTION::CHEM_CONNECTION( CHEM_CONNECTION& aOther ) :
        CHEM_ITEM( aOther ),
        m_parent( nullptr ),
        m_driver( nullptr )
{
    Reset();
    Clone( aOther );
}


bool CHEM_CONNECTION::operator==( const CHEM_CONNECTION& aOther ) const
{
    // NOTE: Not comparing m_dirty or net/bus/subgraph codes
    if( ( aOther.m_driver == m_driver ) &&
        ( aOther.m_type == m_type ) &&
        ( aOther.m_name == m_name ) &&
        ( aOther.m_sheet == m_sheet ) )
    {
        return true;
    }

    return false;
}


void CHEM_CONNECTION::SetDriver( CHEM_ITEM* aItem )
{
    m_driver = aItem;
    recacheName();

    for( const std::shared_ptr<CHEM_CONNECTION>& member : m_members )
        member->SetDriver( aItem );
}


void CHEM_CONNECTION::SetSheet( CHEM_SHEET_PATH aSheet )
{
    m_sheet       = aSheet;
    m_local_sheet = aSheet;

    recacheName();

    for( const std::shared_ptr<CHEM_CONNECTION>& member : m_members )
        member->SetSheet( aSheet );
}


bool CHEM_CONNECTION::operator!=( const CHEM_CONNECTION& aOther ) const
{
    return !( aOther == *this );
}


void CHEM_CONNECTION::ConfigureFromLabel( const wxString& aLabel )
{
    m_members.clear();

    m_name         = aLabel;
    m_local_name   = aLabel;
    m_local_prefix = m_prefix;

    wxString prefix;
    std::vector<wxString> members;

    wxString unescaped = UnescapeString( aLabel );

    if( NET_SETTINGS::ParseBusVector( unescaped, &prefix, &members ) )
    {
        m_type = CONNECTION_TYPE::BUS;
        m_vector_prefix = prefix;

        long i = 0;

        for( const wxString& vector_member : members )
        {
            auto member            = std::make_shared<CHEM_CONNECTION>( m_parent, m_sheet );
            member->m_type         = CONNECTION_TYPE::NET;
            member->m_prefix       = m_prefix;
            member->m_local_name   = vector_member;
            member->m_local_prefix = m_prefix;
            member->m_vector_index = i++;
            member->SetName( vector_member );
            member->SetGraph( m_graph );
            m_members.push_back( member );
        }
    }
    else if( NET_SETTINGS::ParseBusGroup( unescaped, &prefix, &members ) )
    {
        m_type       = CONNECTION_TYPE::BUS_GROUP;
        m_bus_prefix = prefix;

        // Named bus groups generate a net prefix, unnamed ones don't
        if( !prefix.IsEmpty() )
            prefix += wxT( "." );

        for( const wxString& group_member : members )
        {
            // Handle alias inside bus group member list
            if( auto alias = m_graph->GetBusAlias( group_member ) )
            {
                for( const wxString& alias_member : alias->Members() )
                {
                    auto member = std::make_shared<CHEM_CONNECTION>( m_parent, m_sheet );
                    member->SetPrefix( prefix );
                    member->SetGraph( m_graph );
                    member->ConfigureFromLabel( EscapeString( alias_member, CTX_NETNAME ) );
                    m_members.push_back( member );
                }
            }
            else
            {
                auto member = std::make_shared<CHEM_CONNECTION>( m_parent, m_sheet );
                member->SetPrefix( prefix );
                member->SetGraph( m_graph );
                member->ConfigureFromLabel( group_member );
                m_members.push_back( member );
            }
        }
    }
    else
    {
        m_type = CONNECTION_TYPE::NET;
    }

    recacheName();
}


void CHEM_CONNECTION::Reset()
{
    m_type = CONNECTION_TYPE::NONE;
    m_name.Empty();
    m_local_name.Empty();
    m_local_prefix.Empty();
    m_cached_name.Empty();
    m_cached_name_with_path.Empty();
    m_prefix.Empty();
    m_bus_prefix.Empty();
    m_suffix.Empty();
    m_lastDriver = m_driver;
    m_driver = nullptr;
    m_members.clear();
    m_dirty = true;
    m_net_code = 0;
    m_bus_code = 0;
    m_subgraph_code = 0;
    m_vector_start = 0;
    m_vector_end = 0;
    m_vector_index = 0;
    m_vector_prefix.Empty();
}


void CHEM_CONNECTION::Clone( const CHEM_CONNECTION& aOther )
{
    m_graph = aOther.m_graph;

    // Note: m_lastDriver is not cloned as it needs to be the last driver of *this* connection
    m_driver = aOther.Driver();
    m_sheet  = aOther.Sheet();

    // Note: m_local_sheet is not cloned
    m_name   = aOther.m_name;
    m_type   = aOther.m_type;

    // Note: m_local_name is not cloned if not set yet
    if( m_local_name.IsEmpty() )
    {
        m_local_name   = aOther.LocalName();
        m_local_prefix = aOther.Prefix();
    }

    m_prefix = aOther.Prefix();

    // m_bus_prefix is not cloned; only used for local names
    m_suffix       = aOther.Suffix();
    m_net_code     = aOther.NetCode();
    m_bus_code     = aOther.BusCode();
    m_vector_start = aOther.VectorStart();
    m_vector_end   = aOther.VectorEnd();

    // Note: m_vector_index is not cloned
    m_vector_prefix = aOther.VectorPrefix();

    // Note: subgraph code isn't cloned, it should remain with the original object

    // Handle vector bus members: make sure local names are preserved where possible
    const std::vector<std::shared_ptr<CHEM_CONNECTION>>& otherMembers = aOther.Members();

    if( m_type == CONNECTION_TYPE::BUS && aOther.Type() == CONNECTION_TYPE::BUS )
    {
        if( m_members.empty() )
        {
            m_members = otherMembers;
        }
        else
        {
            size_t cloneLimit = std::min( m_members.size(), otherMembers.size() );

            for( size_t i = 0; i < cloneLimit; ++i )
                m_members[i]->Clone( *otherMembers[i] );
        }
    }
    else if( m_type == CONNECTION_TYPE::BUS_GROUP && aOther.Type() == CONNECTION_TYPE::BUS_GROUP )
    {
        if( m_members.empty() )
        {
            m_members = otherMembers;
        }
        else
        {
            // TODO: refactor this once we support deep nesting
            for( std::shared_ptr<CHEM_CONNECTION>& member : m_members )
            {
                auto it = std::find_if( otherMembers.begin(), otherMembers.end(),
                                        [&]( const std::shared_ptr<CHEM_CONNECTION>& aTest )
                                        {
                                            return aTest->LocalName() == member->LocalName();
                                        } );

                if( it != otherMembers.end() )
                    member->Clone( **it );
            }
        }
    }

    m_type = aOther.Type();

    recacheName();
}


bool CHEM_CONNECTION::IsDriver() const
{
    wxASSERT( Parent() );

    switch( Parent()->Type() )
    {
    case CHEM_LABEL_T:
    case CHEM_GLOBAL_LABEL_T:
    case CHEM_HIER_LABEL_T:
    case CHEM_SHEET_PIN_T:
    case CHEM_SHEET_T:
        return true;

    case CHEM_PIN_T:
    {
        const CHEM_PIN* pin = static_cast<const CHEM_PIN*>( Parent() );

        if( const CHEM_SYMBOL* symbol = dynamic_cast<const CHEM_SYMBOL*>( pin->GetParentSymbol() ) )
        {
            // Only annotated symbols should drive nets.
            return pin->IsPower() || symbol->IsAnnotated( &m_sheet );
        }

        return true;
    }

    default:
        return false;
    }
}


bool CHEM_CONNECTION::HasDriverChanged() const
{
    return m_driver != m_lastDriver;
}


void CHEM_CONNECTION::ClearDriverChanged()
{
    m_lastDriver = m_driver;
}


wxString CHEM_CONNECTION::Name( bool aIgnoreSheet ) const
{
    wxASSERT( !m_cached_name.IsEmpty() );
    return aIgnoreSheet ? m_cached_name : m_cached_name_with_path;
}


wxString CHEM_CONNECTION::GetNetName() const
{
    wxString retv;

    if( m_graph )
    {
        CONNECTION_SUBGRAPH* subgraph = m_graph->GetSubgraphForItem( m_parent );

        if( subgraph )
            retv = subgraph->GetNetName();
    }

    return retv;
}


void CHEM_CONNECTION::recacheName()
{
    m_cached_name = m_name.IsEmpty() ? wxString( wxT( "<NO NET>" ) )
                                     : wxString( m_prefix ) << m_name << m_suffix;

    bool prepend_path = true;

    if( !Parent() || m_type == CONNECTION_TYPE::NONE )
        prepend_path = false;

    if( m_driver )
    {
        switch( m_driver->Type() )
        {
        case CHEM_GLOBAL_LABEL_T:
            prepend_path = false;
            break;

        case CHEM_PIN_T:
        { // Normal pins and global power pins do not need a path.  But local power pins do
            CHEM_PIN* pin = static_cast<CHEM_PIN*>( m_driver );

            prepend_path = pin->IsLocalPower();
            break;
        }

        default:
            break;
        }
    }

    m_cached_name_with_path = prepend_path ? m_sheet.PathHumanReadable() << m_cached_name
                                           : m_cached_name;
}


void CHEM_CONNECTION::SetPrefix( const wxString& aPrefix )
{
    m_prefix = aPrefix;

    recacheName();

    for( const std::shared_ptr<CHEM_CONNECTION>& m : Members() )
        m->SetPrefix( aPrefix );
}


void CHEM_CONNECTION::SetSuffix( const wxString& aSuffix )
{
    m_suffix = aSuffix;

    recacheName();

    for( const std::shared_ptr<CHEM_CONNECTION>& m : Members() )
        m->SetSuffix( aSuffix );
}


void CHEM_CONNECTION::AppendInfoToMsgPanel( std::vector<MSG_PANEL_ITEM>& aList ) const
{
    wxString msg;

    aList.emplace_back( _( "Connection Name" ), UnescapeString( Name() ) );

#if defined(DEBUG)
    // These messages are not flagged as translatable, because they are only debug messages

    if( IsBus() )
        aList.emplace_back( wxT( "Bus Code" ), wxString::Format( "%d", m_bus_code ) );

    aList.emplace_back( wxT( "Subgraph Code" ), wxString::Format( "%d", m_subgraph_code ) );

    if( CHEM_ITEM* driver = Driver() )
    {
        UNITS_PROVIDER unitsProvider( schIUScale, EDA_UNITS::MM );

        msg.Printf( wxS( "%s at %p" ),
                    driver->GetItemDescription( &unitsProvider, false ),
                    driver );
        aList.emplace_back( wxT( "Connection Source" ), msg );
    }
#endif
}


bool CHEM_CONNECTION::IsBusLabel( const wxString& aLabel )
{
    const wxString& unescaped = UnescapeString( aLabel );

    return NET_SETTINGS::ParseBusVector( unescaped, nullptr, nullptr )
                || NET_SETTINGS::ParseBusGroup( unescaped, nullptr, nullptr );
}


bool CHEM_CONNECTION::MightBeBusLabel( const wxString& aLabel )
{
    // Weak heuristic for performance reasons.  Stronger test will be used for connectivity
    wxString label = UnescapeString( aLabel );

    return label.Contains( wxT( "[" ) ) || label.Contains( wxT( "{" ) );
}


const std::vector< std::shared_ptr< CHEM_CONNECTION > > CHEM_CONNECTION::AllMembers() const
{
    std::vector< std::shared_ptr< CHEM_CONNECTION > > ret( m_members );

    for( const std::shared_ptr<CHEM_CONNECTION>& member : m_members )
    {
        if( member->IsBus() )
            ret.insert( ret.end(), member->Members().begin(), member->Members().end() );
    }

    return ret;
}


bool CHEM_CONNECTION::IsSubsetOf( CHEM_CONNECTION* aOther ) const
{
    if( !aOther->IsBus() )
        return false;

    for( const std::shared_ptr<CHEM_CONNECTION>& member : aOther->Members() )
    {
        if( member->FullLocalName() == FullLocalName() )
            return true;
    }

    return false;
}


bool CHEM_CONNECTION::IsMemberOfBus( CHEM_CONNECTION* aOther ) const
{
    if( !aOther->IsBus() )
        return false;

    wxString me = Name( true );

    for( const std::shared_ptr<CHEM_CONNECTION>& m : aOther->Members() )
    {
        if( m->Name( true ) == me )
            return true;
    }

    return false;
} 