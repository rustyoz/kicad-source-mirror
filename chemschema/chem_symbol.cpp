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

#include "chem_symbol.h"
#include "chem_shape.h"
#include "chem_text.h"
#include "chem_line.h"
#include <chem_schematic.h>
#include <kiway.h>
#include <eda_units.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>

CHEM_SYMBOL::CHEM_SYMBOL() :
    CHEM_ITEM( nullptr, CHEM_ITEM::CHEM_SYMBOL_T )
{
    m_position = wxPoint( 0, 0 );
    m_symbolType = CUSTOM;
    m_name = wxT( "New Symbol" );
    m_description = wxEmptyString;
    m_rotationAngle = 0.0;
}


CHEM_SYMBOL::CHEM_SYMBOL( const CHEM_SYMBOL& aSymbol ) :
    CHEM_ITEM( aSymbol )
{
    m_position = aSymbol.m_position;
    m_symbolType = aSymbol.m_symbolType;
    m_name = aSymbol.m_name;
    m_description = aSymbol.m_description;
    m_rotationAngle = aSymbol.m_rotationAngle;
    
    // Deep copy of shapes, texts and lines
    for( CHEM_SHAPE* shape : aSymbol.m_shapes )
    {
        CHEM_SHAPE* newShape = static_cast<CHEM_SHAPE*>( shape->Clone() );
        m_shapes.push_back( newShape );
    }
    
    for( CHEM_TEXT* text : aSymbol.m_texts )
    {
        CHEM_TEXT* newText = static_cast<CHEM_TEXT*>( text->Clone() );
        m_texts.push_back( newText );
    }
    
    for( CHEM_LINE* line : aSymbol.m_lines )
    {
        CHEM_LINE* newLine = static_cast<CHEM_LINE*>( line->Clone() );
        m_lines.push_back( newLine );
    }
}


CHEM_SYMBOL::~CHEM_SYMBOL()
{
    // Delete all owned objects
    for( CHEM_SHAPE* shape : m_shapes )
        delete shape;
        
    for( CHEM_TEXT* text : m_texts )
        delete text;
        
    for( CHEM_LINE* line : m_lines )
        delete line;
}


EDA_ITEM* CHEM_SYMBOL::Clone() const
{
    return new CHEM_SYMBOL( *this );
}


void CHEM_SYMBOL::ViewGetLayers( int aLayers[], int& aCount ) const
{
    aLayers[0] = CHEM_LAYER_SYMBOLS;
    aCount = 1;
}


void CHEM_SYMBOL::SwapData( CHEM_ITEM* aItem )
{
    CHEM_SYMBOL* item = static_cast<CHEM_SYMBOL*>( aItem );

    std::swap( m_position, item->m_position );
    std::swap( m_symbolType, item->m_symbolType );
    std::swap( m_name, item->m_name );
    std::swap( m_description, item->m_description );
    std::swap( m_rotationAngle, item->m_rotationAngle );
    std::swap( m_shapes, item->m_shapes );
    std::swap( m_texts, item->m_texts );
    std::swap( m_lines, item->m_lines );
}


void CHEM_SYMBOL::SetPosition( const wxPoint& aPosition )
{
    m_position = aPosition;
}


wxPoint CHEM_SYMBOL::GetPosition() const
{
    return m_position;
}


void CHEM_SYMBOL::SetSymbolType( SYMBOL_TYPE aType )
{
    m_symbolType = aType;
}


CHEM_SYMBOL::SYMBOL_TYPE CHEM_SYMBOL::GetSymbolType() const
{
    return m_symbolType;
}


void CHEM_SYMBOL::SetName( const wxString& aName )
{
    m_name = aName;
}


wxString CHEM_SYMBOL::GetName() const
{
    return m_name;
}


void CHEM_SYMBOL::SetDescription( const wxString& aDescription )
{
    m_description = aDescription;
}


wxString CHEM_SYMBOL::GetDescription() const
{
    return m_description;
}


void CHEM_SYMBOL::SetRotationAngle( double aAngle )
{
    m_rotationAngle = aAngle;
}


double CHEM_SYMBOL::GetRotationAngle() const
{
    return m_rotationAngle;
}


void CHEM_SYMBOL::AddShape( CHEM_SHAPE* aShape )
{
    if( aShape )
        m_shapes.push_back( aShape );
}


const std::vector<CHEM_SHAPE*>& CHEM_SYMBOL::GetShapes() const
{
    return m_shapes;
}


void CHEM_SYMBOL::AddText( CHEM_TEXT* aText )
{
    if( aText )
        m_texts.push_back( aText );
}


const std::vector<CHEM_TEXT*>& CHEM_SYMBOL::GetTexts() const
{
    return m_texts;
}


void CHEM_SYMBOL::AddLine( CHEM_LINE* aLine )
{
    if( aLine )
        m_lines.push_back( aLine );
}


const std::vector<CHEM_LINE*>& CHEM_SYMBOL::GetLines() const
{
    return m_lines;
}


std::vector<BOX2I> CHEM_SYMBOL::GetBoundingBoxes() const
{
    std::vector<BOX2I> boxes;
    
    // Collect all bounding boxes from shapes
    for( CHEM_SHAPE* shape : m_shapes )
    {
        std::vector<BOX2I> shapeBoxes = shape->GetBoundingBoxes();
        boxes.insert( boxes.end(), shapeBoxes.begin(), shapeBoxes.end() );
    }
    
    // Collect all bounding boxes from texts
    for( CHEM_TEXT* text : m_texts )
    {
        std::vector<BOX2I> textBoxes = text->GetBoundingBoxes();
        boxes.insert( boxes.end(), textBoxes.begin(), textBoxes.end() );
    }
    
    // Collect all bounding boxes from lines
    for( CHEM_LINE* line : m_lines )
    {
        std::vector<BOX2I> lineBoxes = line->GetBoundingBoxes();
        boxes.insert( boxes.end(), lineBoxes.begin(), lineBoxes.end() );
    }
    
    return boxes;
}


wxString CHEM_SYMBOL::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    wxString type;
    
    switch( m_symbolType )
    {
    case REACTOR:
        type = _( "Reactor" );
        break;
    case TANK:
        type = _( "Tank" );
        break;
    case PUMP:
        type = _( "Pump" );
        break;
    case VALVE:
        type = _( "Valve" );
        break;
    case HEAT_EXCHANGER:
        type = _( "Heat Exchanger" );
        break;
    case COMPRESSOR:
        type = _( "Compressor" );
        break;
    case DISTILLATION_COLUMN:
        type = _( "Distillation Column" );
        break;
    case FILTER:
        type = _( "Filter" );
        break;
    case MIXER:
        type = _( "Mixer" );
        break;
    case CUSTOM:
    default:
        type = _( "Custom Symbol" );
        break;
    }
    
    return wxString::Format( _( "%s '%s'" ), type, m_name );
}


BITMAPS CHEM_SYMBOL::GetMenuImage() const
{
    // Select appropriate icon based on symbol type
    // For now, return a generic symbol icon
    return BITMAPS::symbol;
}


bool CHEM_SYMBOL::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    // Check if any shape, text or line is hit
    for( CHEM_SHAPE* shape : m_shapes )
    {
        if( shape->HitTest( aPosition, aAccuracy ) )
            return true;
    }
    
    for( CHEM_TEXT* text : m_texts )
    {
        if( text->HitTest( aPosition, aAccuracy ) )
            return true;
    }
    
    for( CHEM_LINE* line : m_lines )
    {
        if( line->HitTest( aPosition, aAccuracy ) )
            return true;
    }
    
    return false;
}


bool CHEM_SYMBOL::HitTest( const BOX2I& aRect, bool aContains, int aAccuracy ) const
{
    if( aContains )
    {
        // All parts must be fully contained
        for( CHEM_SHAPE* shape : m_shapes )
        {
            if( !shape->HitTest( aRect, aContains, aAccuracy ) )
                return false;
        }
        
        for( CHEM_TEXT* text : m_texts )
        {
            if( !text->HitTest( aRect, aContains, aAccuracy ) )
                return false;
        }
        
        for( CHEM_LINE* line : m_lines )
        {
            if( !line->HitTest( aRect, aContains, aAccuracy ) )
                return false;
        }
        
        return !m_shapes.empty() || !m_texts.empty() || !m_lines.empty();
    }
    else
    {
        // At least one part must intersect
        for( CHEM_SHAPE* shape : m_shapes )
        {
            if( shape->HitTest( aRect, aContains, aAccuracy ) )
                return true;
        }
        
        for( CHEM_TEXT* text : m_texts )
        {
            if( text->HitTest( aRect, aContains, aAccuracy ) )
                return true;
        }
        
        for( CHEM_LINE* line : m_lines )
        {
            if( line->HitTest( aRect, aContains, aAccuracy ) )
                return true;
        }
        
        return false;
    }
}


void CHEM_SYMBOL::Plot( PLOTTER* aPlotter ) const
{
    // Plot all shapes, texts and lines
    for( CHEM_SHAPE* shape : m_shapes )
        shape->Plot( aPlotter );
        
    for( CHEM_TEXT* text : m_texts )
        text->Plot( aPlotter );
        
    for( CHEM_LINE* line : m_lines )
        line->Plot( aPlotter );
}


bool CHEM_SYMBOL::Matches( const EDA_SEARCH_DATA& aSearchData, void* aAuxData ) const
{
    // Check name and description
    if( EDA_ITEM::Matches( m_name, aSearchData ) )
        return true;
        
    if( EDA_ITEM::Matches( m_description, aSearchData ) )
        return true;
        
    // Check text in all text items
    for( CHEM_TEXT* text : m_texts )
    {
        if( text->Matches( aSearchData, aAuxData ) )
            return true;
    }
    
    return false;
} 