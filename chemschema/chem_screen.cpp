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

#include "chem_screen.h"
#include "chem_includes.h"
// Comment out the problematic include
// #include <plotter.h>
#include <plotters/plotter.h>
#include <trigo.h>
#include <eda_search.h>
#include "chem_schematic.h"

CHEM_SCREEN::CHEM_SCREEN( EDA_ITEM* aParent ) :
    BASE_SCREEN( aParent, CHEM_SCREEN_T ),
    m_fileFormatVersionAtLoad( 0 ),
    m_isReadOnly( false ),
    m_fileExists( false )
{
    m_modification_sync = 0;
    m_refCount = 0;
    m_zoomInitialized = false;
    m_LastZoomLevel = 1.0;
    m_Center = false;
}

CHEM_SCREEN::CHEM_SCREEN( const CHEM_SCREEN& aScreen ) :
    BASE_SCREEN( aScreen )
{
    m_fileFormatVersionAtLoad = aScreen.m_fileFormatVersionAtLoad;
    m_isReadOnly = aScreen.m_isReadOnly;
    m_fileExists = aScreen.m_fileExists;
    m_modification_sync = aScreen.m_modification_sync;
    m_refCount = aScreen.m_refCount;
    m_zoomInitialized = aScreen.m_zoomInitialized;
    m_LastZoomLevel = aScreen.m_LastZoomLevel;
    m_Center = aScreen.m_Center;
}

CHEM_SCREEN::~CHEM_SCREEN()
{
    FreeDrawList();
}

void CHEM_SCREEN::Append( CHEM_ITEM* aItem )
{
    if( aItem->Type() != CHEM_PIPE_T && aItem->Type() != CHEM_FIELD_T )
    {
        aItem->SetParent( this );
        m_rtree.insert( aItem );
        --m_modification_sync;
    }
}

void CHEM_SCREEN::Remove( CHEM_ITEM* aItem )
{
    bool retv = m_rtree.remove( aItem );
    return retv;
}

void CHEM_SCREEN::Update( CHEM_ITEM* aItem )
{
    if( Remove( aItem ) )
        Append( aItem );
}

void CHEM_SCREEN::Clear( bool aFree )
{
    if( aFree )
    {
        FreeDrawList();
    }
    else
    {
        m_rtree.clear();
    }
}

void CHEM_SCREEN::FreeDrawList()
{
    std::vector<CHEM_ITEM*> delete_list;
    std::copy_if( m_rtree.begin(), m_rtree.end(), std::back_inserter( delete_list ),
            []( CHEM_ITEM* aItem )
            {
                return ( aItem->Type() != CHEM_PIPE_T && aItem->Type() != CHEM_FIELD_T );
            } );

    m_rtree.clear();

    for( CHEM_ITEM* item : delete_list )
        delete item;
}

bool CHEM_SCREEN::IsProcessConnected( const VECTOR2I& aPosition ) const
{
    for( const CHEM_ITEM* item : Items().Overlapping( aPosition ) )
    {
        if( item->IsConnectable() && item->IsConnected( aPosition ) )
            return true;
    }
    return false;
}

std::vector<CHEM_ITEM*> CHEM_SCREEN::GetConnectedItems( const VECTOR2I& aPosition ) const
{
    std::vector<CHEM_ITEM*> connected;
    
    for( CHEM_ITEM* item : Items().Overlapping( aPosition ) )
    {
        if( item->IsConnectable() && item->IsConnected( aPosition ) )
            connected.push_back( item );
    }
    
    return connected;
}

void CHEM_SCREEN::UpdateEquipmentLinks( REPORTER* aReporter )
{
    // Update equipment symbol links and validate connections
    for( CHEM_ITEM* item : Items().OfType( CHEM_EQUIPMENT_T ) )
    {
        CHEM_EQUIPMENT* equipment = static_cast<CHEM_EQUIPMENT*>( item );
        equipment->UpdateLinks( aReporter );
    }
}

void CHEM_SCREEN::ValidateProcessFlow()
{
    // Validate process flow connections and constraints
    for( CHEM_ITEM* item : Items() )
    {
        if( item->IsConnectable() )
        {
            std::vector<VECTOR2I> connections = item->GetConnectionPoints();
            for( const VECTOR2I& point : connections )
            {
                if( !IsProcessConnected( point ) )
                {
                    // Handle unconnected process points
                }
            }
        }
    }
}

void CHEM_SCREEN::Plot( PLOTTER* aPlotter, const CHEM_PLOT_OPTS& aPlotOpts ) const
{
    // Plot background items first
    for( CHEM_ITEM* item : Items() )
    {
        if( !item->IsMoving() )
        {
            aPlotter->SetCurrentLineWidth( item->GetEffectivePenWidth() );
            item->Plot( aPlotter, true, aPlotOpts );
        }
    }

    // Plot foreground items
    for( CHEM_ITEM* item : Items() )
    {
        if( !item->IsMoving() )
        {
            aPlotter->SetCurrentLineWidth( item->GetEffectivePenWidth() );
            item->Plot( aPlotter, false, aPlotOpts );
        }
    }
}

bool CHEM_SCREEN::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    for( CHEM_ITEM* item : Items().Overlapping( aPosition, aAccuracy ) )
    {
        if( item->HitTest( aPosition, aAccuracy ) )
            return true;
    }
    return false;
}

CHEM_ITEM* CHEM_SCREEN::GetItem( const VECTOR2I& aPosition, int aAccuracy, KICAD_T aType ) const
{
    BOX2I bbox;
    bbox.SetOrigin( aPosition );
    bbox.Inflate( aAccuracy );

    for( CHEM_ITEM* item : Items().Overlapping( aType, bbox ) )
    {
        if( item->HitTest( aPosition, aAccuracy ) )
            return item;
    }

    return nullptr;
}

void CHEM_SCREEN::SetFileName( const wxString& aFileName )
{
    wxASSERT( aFileName.IsEmpty() || wxIsAbsolutePath( aFileName ) );
    m_fileName = aFileName;
}

void CHEM_SCREEN::IncRefCount()
{
    m_refCount++;
}

void CHEM_SCREEN::DecRefCount()
{
    wxCHECK_RET( m_refCount != 0, wxT( "Screen reference count already zero. Bad programmer!" ) );
    m_refCount--;
}

bool CHEM_SCREEN::HasItems( KICAD_T aItemType ) const
{
    for( const CHEM_ITEM* item : m_rtree )
    {
        if( item->Type() == aItemType )
            return true;
    }
    return false;
}

bool CHEM_SCREEN::ClassOf( const EDA_ITEM* aItem )
{
    return aItem && CHEM_SCREEN_T == aItem->Type();
}

EDA_ITEM* CHEM_SCREEN::Clone() const
{
    return new CHEM_SCREEN( *this );
}

std::vector<int> CHEM_SCREEN::ViewGetLayers() const
{
    std::vector<int> layers;
    // Add default layers for chemical schematics
    layers.push_back( LAYER_CHEM_BACKGROUND );
    layers.push_back( LAYER_CHEM_FOREGROUND );
    return layers;
}

void CHEM_SCREEN::SwapData( EDA_ITEM* aItem )
{
    CHEM_SCREEN* other = dynamic_cast<CHEM_SCREEN*>( aItem );
    if( !other )
        return;
    
    std::swap( m_position, other->m_position );
    std::swap( m_size, other->m_size );
    std::swap( m_description, other->m_description );
    std::swap( m_name, other->m_name );
    std::swap( m_rotationAngle, other->m_rotationAngle );
    std::swap( m_outlineColor, other->m_outlineColor );
    std::swap( m_fillColor, other->m_fillColor );
    std::swap( m_lineWidth, other->m_lineWidth );
    
    BASE_SCREEN::SwapData( aItem );
}

void CHEM_SCREEN::SetPosition( const VECTOR2I& aPosition )
{
    m_position = aPosition;
}

VECTOR2I CHEM_SCREEN::GetPosition() const
{
    return m_position;
}

void CHEM_SCREEN::SetSize( const VECTOR2I& aSize )
{
    m_size = aSize;
}

const VECTOR2I& CHEM_SCREEN::GetSize() const
{
    return m_size;
}

void CHEM_SCREEN::SetDescription( const wxString& aDescription )
{
    m_description = aDescription;
}

const wxString& CHEM_SCREEN::GetDescription() const
{
    return m_description;
}

void CHEM_SCREEN::SetName( const wxString& aName )
{
    m_name = aName;
}

const wxString& CHEM_SCREEN::GetName() const
{
    return m_name;
}

void CHEM_SCREEN::SetRotationAngle( int aAngle )
{
    // Normalize angle to 0-3600 range (0-360 degrees)
    m_rotationAngle = NormalizeAngle360( aAngle );
}

int CHEM_SCREEN::GetRotationAngle() const
{
    return m_rotationAngle;
}

void CHEM_SCREEN::SetOutlineColor( const COLOR4D& aColor )
{
    m_outlineColor = aColor;
}

COLOR4D CHEM_SCREEN::GetOutlineColor() const
{
    return m_outlineColor;
}

void CHEM_SCREEN::SetFillColor( const COLOR4D& aColor )
{
    m_fillColor = aColor;
}

COLOR4D CHEM_SCREEN::GetFillColor() const
{
    return m_fillColor;
}

void CHEM_SCREEN::SetLineWidth( int aWidth )
{
    m_lineWidth = aWidth;
}

int CHEM_SCREEN::GetLineWidth() const
{
    return m_lineWidth;
}

const BOX2I CHEM_SCREEN::GetBoundingBox() override const
{
    BOX2I bbox;
    bool first = true;

    for( const CHEM_ITEM* item : m_rtree )
    {
        if( first )
        {
            bbox = item->GetBoundingBox();
            first = false;
        }
        else
        {
            bbox.Merge( item->GetBoundingBox() );
        }
    }

    return bbox;
}

std::vector<BOX2I> CHEM_SCREEN::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    boxes.push_back( GetBoundingBox() );
    return boxes;
}

wxString CHEM_SCREEN::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    return wxString::Format( _( "Chemical Screen" ) );
}

BITMAPS CHEM_SCREEN::GetMenuImage() const
{
    return BITMAPS::SCHEMATIC;
}

bool CHEM_SCREEN::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    for( const CHEM_ITEM* item : m_rtree )
    {
        if( item->HitTest( aRect, aContains, aAccuracy ) )
            return true;
    }
    return false;
}

void CHEM_SCREEN::Plot( PLOTTER* aPlotter ) const
{
    // Plot the screen outline
    aPlotter->SetCurrentLineWidth( m_lineWidth );
    aPlotter->SetColor( m_outlineColor );
    
    // Plot the rectangle
    VECTOR2I corners[4];
    corners[0] = m_position;
    corners[1] = m_position + VECTOR2I( m_size.x, 0 );
    corners[2] = m_position + m_size;
    corners[3] = m_position + VECTOR2I( 0, m_size.y );
    
    // Rotate corners if needed
    if( m_rotationAngle != 0 )
    {
        VECTOR2I center = m_position + (m_size / 2);
        for( int i = 0; i < 4; i++ )
        {
            RotatePoint( corners[i], center, m_rotationAngle );
        }
    }
    
    // Draw the rectangle
    aPlotter->MoveTo( corners[0] );
    for( int i = 1; i < 4; i++ )
    {
        aPlotter->LineTo( corners[i] );
    }
    aPlotter->LineTo( corners[0] );
    
    // Fill if needed
    if( m_fillColor != COLOR4D::UNSPECIFIED )
    {
        aPlotter->SetColor( m_fillColor );
        aPlotter->FillRect( m_position, m_size, m_rotationAngle );
    }
}

bool CHEM_SCREEN::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // Search in name and description
    if( Matches( m_name, aSearchData ) )
        return true;
        
    if( Matches( m_description, aSearchData ) )
        return true;
        
    return false;
}

const VECTOR2I CHEM_SCREEN::GetFocusPosition() const
{
    // For a screen, the focus position is the center
    return m_position + (m_size / 2);
}

VECTOR2I CHEM_SCREEN::GetSortPosition() const
{
    // Use the top-left corner for sorting
    return m_position;
}

wxString CHEM_SCREEN::GetItemDescription( UNITS_PROVIDER* aUnitsProvider, bool aFull ) const
{
    wxString desc = wxString::Format( _("Chemical Process Screen: %s"), m_name );
    
    if( aFull )
    {
        desc += wxString::Format( wxT("\nPosition: %s"), 
            aUnitsProvider->MessageTextFromValue( m_position.x ) + wxT(", ") +
            aUnitsProvider->MessageTextFromValue( m_position.y ) );
        
        desc += wxString::Format( wxT("\nSize: %s"), 
            aUnitsProvider->MessageTextFromValue( m_size.x ) + wxT(" x ") +
            aUnitsProvider->MessageTextFromValue( m_size.y ) );
        
        if( !m_description.IsEmpty() )
            desc += wxString::Format( wxT("\nDescription: %s"), m_description );
    }
    
    return desc;
}

bool CHEM_SCREEN::IsReplaceable() const
{
    // Screens have replaceable text in name and description
    return true;
}

bool CHEM_SCREEN::Replace( const EDA_SEARCH_DATA& aSearchData, void* aAuxData )
{
    bool modified = false;
    
    // Replace in name
    if( Replace( aSearchData, m_name ) )
        modified = true;
        
    // Replace in description
    if( Replace( aSearchData, m_description ) )
        modified = true;
        
    if( modified )
        SetModified();
        
    return modified;
}

CHEM_SCHEMATIC* CHEM_SCREEN::Schematic() const
{
    EDA_ITEM* parent = GetParent();

    while( parent )
    {
        if( parent->Type() == SCHEMATIC_T )
            return static_cast<CHEM_SCHEMATIC*>( parent );
        else
            parent = parent->GetParent();
    }

    return nullptr;
}

void CHEM_SCREEN::Plot( PLOTTER* aPlotter, bool aBackground,
                       const CHEM_PLOT_OPTS& aPlotOpts,
                       int aUnit, int aBodyStyle,
                       const VECTOR2I& aOffset, bool aDimmed )
{
    for( CHEM_ITEM* item : m_rtree )
    {
        item->Plot( aPlotter, aBackground, aPlotOpts, aUnit, aBodyStyle, aOffset, aDimmed );
    }
} 