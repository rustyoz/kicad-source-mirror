/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2024 KiCad Developers.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <chem_painter.h>
#include <chem_symbol.h>
#include <chem_sheet.h>
#include <chem_screen.h>
#include <lib_symbol.h>
#include <lib_shape.h>
#include <lib_text.h>
#include <lib_textbox.h>
#include <lib_pin.h>
#include <lib_field.h>
#include <gal/graphics_abstraction_layer.h>
#include <view/view.h>
#include <math/util.h>
#include <trigo.h>
#include <settings/color_settings.h>

using namespace KIGFX;

const std::vector<KICAD_T> CHEM_PAINTER::g_ScaledSelectionTypes = {
    CHEM_SYMBOL_T,
    CHEM_SHEET_T,
    CHEM_JUNCTION_T,
    CHEM_LINE_T,
    CHEM_BUS_WIRE_ENTRY_T,
    CHEM_BUS_BUS_ENTRY_T,
    CHEM_NO_CONNECT_T,
    CHEM_PIN_T,
    CHEM_FIELD_T,
    CHEM_TEXT_T,
    CHEM_TEXTBOX_T,
    CHEM_HIERLABEL_T,
    CHEM_GLOBALLABEL_T
};


CHEM_PAINTER::CHEM_PAINTER( GAL* aGal ) :
    PAINTER( aGal )
{
}


bool CHEM_PAINTER::Draw( const VIEW_ITEM* aItem, int aLayer )
{
    if( !aItem )
        return false;

    auto item = static_cast<const EDA_ITEM*>( aItem );

    if( item->Type() == LIB_SYMBOL_T )
        draw( static_cast<const LIB_SYMBOL*>( item ), aLayer );
    else if( item->Type() == LIB_SHAPE_T )
        draw( static_cast<const LIB_SHAPE*>( item ), aLayer );
    else if( item->Type() == LIB_TEXT_T )
        draw( static_cast<const LIB_TEXT*>( item ), aLayer );
    else if( item->Type() == LIB_TEXTBOX_T )
        draw( static_cast<const LIB_TEXTBOX*>( item ), aLayer );
    else if( item->Type() == LIB_PIN_T )
        draw( static_cast<const LIB_PIN*>( item ), aLayer );
    else if( item->Type() == LIB_FIELD_T )
        draw( static_cast<const LIB_FIELD*>( item ), aLayer );
    else if( item->Type() == CHEM_SYMBOL_T )
        draw( static_cast<const CHEM_SYMBOL*>( item ), aLayer );
    else if( item->Type() == CHEM_SHEET_T )
        draw( static_cast<const CHEM_SHEET*>( item ), aLayer );
    else if( item->Type() == CHEM_FIELD_T )
        draw( static_cast<const CHEM_FIELD*>( item ), aLayer, false );
    else if( item->Type() == CHEM_HIERLABEL_T )
        draw( static_cast<const CHEM_HIERLABEL*>( item ), aLayer, false );
    else if( item->Type() == CHEM_GLOBALLABEL_T )
        draw( static_cast<const CHEM_GLOBALLABEL*>( item ), aLayer, false );
    else if( item->Type() == CHEM_TEXT_T )
        draw( static_cast<const CHEM_TEXT*>( item ), aLayer, false );
    else if( item->Type() == CHEM_TEXTBOX_T )
        draw( static_cast<const CHEM_TEXTBOX*>( item ), aLayer, false );
    else if( item->Type() == CHEM_JUNCTION_T )
        draw( static_cast<const CHEM_JUNCTION*>( item ), aLayer );
    else if( item->Type() == CHEM_LINE_T )
        draw( static_cast<const CHEM_LINE*>( item ), aLayer );
    else if( item->Type() == CHEM_BUS_WIRE_ENTRY_T )
        draw( static_cast<const CHEM_BUS_WIRE_ENTRY*>( item ), aLayer );
    else if( item->Type() == CHEM_BUS_BUS_ENTRY_T )
        draw( static_cast<const CHEM_BUS_BUS_ENTRY*>( item ), aLayer );
    else if( item->Type() == CHEM_NO_CONNECT_T )
        draw( static_cast<const CHEM_NO_CONNECT*>( item ), aLayer );
    else if( item->Type() == CHEM_PIN_T )
        draw( static_cast<const CHEM_PIN*>( item ), aLayer );
    else
        return false;

    return true;
}


void CHEM_PAINTER::draw( const SCH_ITEM* aItem, int aLayer ) const
{
    // Base class for all chemical schematic items
    // This is a placeholder - implement specific drawing logic for chemical items
}


void CHEM_PAINTER::draw( const LIB_ITEM* aItem, int aLayer ) const
{
    // Base class for all library items
    // This is a placeholder - implement specific drawing logic for library items
}


void CHEM_PAINTER::draw( const SCH_SYMBOL* aSymbol, int aLayer ) const
{
    // Draw a chemical symbol
    // This is a placeholder - implement specific drawing logic for chemical symbols
}


void CHEM_PAINTER::draw( const SCH_SHEET* aSheet, int aLayer ) const
{
    // Draw a chemical sheet
    // This is a placeholder - implement specific drawing logic for chemical sheets
}


void CHEM_PAINTER::draw( const SCH_FIELD* aField, int aLayer, bool aDnp ) const
{
    // Draw a field
    // This is a placeholder - implement specific drawing logic for fields
}


void CHEM_PAINTER::draw( const SCH_HIERLABEL* aLabel, int aLayer, bool aDnp ) const
{
    // Draw a hierarchical label
    // This is a placeholder - implement specific drawing logic for hierarchical labels
}


void CHEM_PAINTER::draw( const SCH_GLOBALLABEL* aLabel, int aLayer, bool aDnp ) const
{
    // Draw a global label
    // This is a placeholder - implement specific drawing logic for global labels
}


void CHEM_PAINTER::draw( const SCH_TEXT* aText, int aLayer, bool aDnp ) const
{
    // Draw text
    // This is a placeholder - implement specific drawing logic for text
}


void CHEM_PAINTER::draw( const SCH_TEXTBOX* aTextBox, int aLayer, bool aDnp ) const
{
    // Draw a text box
    // This is a placeholder - implement specific drawing logic for text boxes
}


void CHEM_PAINTER::draw( const SCH_JUNCTION* aJunction, int aLayer ) const
{
    // Draw a junction
    // This is a placeholder - implement specific drawing logic for junctions
}


void CHEM_PAINTER::draw( const SCH_LINE* aLine, int aLayer ) const
{
    // Draw a line
    // This is a placeholder - implement specific drawing logic for lines
}


void CHEM_PAINTER::draw( const SCH_BUS_WIRE_ENTRY* aEntry, int aLayer ) const
{
    // Draw a bus wire entry
    // This is a placeholder - implement specific drawing logic for bus wire entries
}


void CHEM_PAINTER::draw( const SCH_BUS_BUS_ENTRY* aEntry, int aLayer ) const
{
    // Draw a bus bus entry
    // This is a placeholder - implement specific drawing logic for bus bus entries
}


void CHEM_PAINTER::draw( const SCH_NO_CONNECT* aNC, int aLayer ) const
{
    // Draw a no connect symbol
    // This is a placeholder - implement specific drawing logic for no connect symbols
}


void CHEM_PAINTER::draw( const SCH_PIN* aPin, int aLayer ) const
{
    // Draw a pin
    // This is a placeholder - implement specific drawing logic for pins
}


void CHEM_PAINTER::draw( const LIB_SYMBOL* aSymbol, int aLayer ) const
{
    // Draw a library symbol
    // This is a placeholder - implement specific drawing logic for library symbols
}


void CHEM_PAINTER::draw( const LIB_SHAPE* aShape, int aLayer ) const
{
    // Draw a library shape
    // This is a placeholder - implement specific drawing logic for library shapes
}


void CHEM_PAINTER::draw( const LIB_TEXT* aText, int aLayer ) const
{
    // Draw library text
    // This is a placeholder - implement specific drawing logic for library text
}


void CHEM_PAINTER::draw( const LIB_TEXTBOX* aTextBox, int aLayer ) const
{
    // Draw a library text box
    // This is a placeholder - implement specific drawing logic for library text boxes
}


void CHEM_PAINTER::draw( const LIB_PIN* aPin, int aLayer ) const
{
    // Draw a library pin
    // This is a placeholder - implement specific drawing logic for library pins
}


void CHEM_PAINTER::draw( const LIB_FIELD* aField, int aLayer ) const
{
    // Draw a library field
    // This is a placeholder - implement specific drawing logic for library fields
}


void CHEM_PAINTER::drawSymbolBorder( const SCH_SYMBOL* aSymbol, int aLayer ) const
{
    // Draw a symbol border
    // This is a placeholder - implement specific drawing logic for symbol borders
}


void CHEM_PAINTER::drawDanglingSymbolPin( const SCH_SYMBOL* aSymbol, const SCH_PIN* aPin ) const
{
    // Draw a dangling symbol pin
    // This is a placeholder - implement specific drawing logic for dangling symbol pins
}


void CHEM_PAINTER::drawNoConnectMarker( const VECTOR2I& aPos ) const
{
    // Draw a no connect marker
    // This is a placeholder - implement specific drawing logic for no connect markers
}


void CHEM_PAINTER::drawBusEntryShape( const VECTOR2I& aStart, const VECTOR2I& aEnd, bool aEntry ) const
{
    // Draw a bus entry shape
    // This is a placeholder - implement specific drawing logic for bus entry shapes
}


void CHEM_PAINTER::drawPinDot( const VECTOR2I& aPos, const COLOR4D& aColor ) const
{
    // Draw a pin dot
    // This is a placeholder - implement specific drawing logic for pin dots
}


void CHEM_PAINTER::drawPinSymbol( const VECTOR2I& aPos, const VECTOR2I& aDir, bool aInverted,
                                 ELECTRICAL_PINTYPE aType, const COLOR4D& aColor ) const
{
    // Draw a pin symbol
    // This is a placeholder - implement specific drawing logic for pin symbols
}


void CHEM_PAINTER::drawPinText( const SCH_PIN* aPin, const wxString& aText, const VECTOR2I& aPos,
                               const COLOR4D& aColor, bool aHideIfSmall ) const
{
    // Draw pin text
    // This is a placeholder - implement specific drawing logic for pin text
}


void CHEM_PAINTER::drawFieldText( const SCH_FIELD* aField, const wxString& aText, const VECTOR2I& aPos,
                                 const COLOR4D& aColor, bool aHideIfSmall ) const
{
    // Draw field text
    // This is a placeholder - implement specific drawing logic for field text
}


void CHEM_PAINTER::drawTextBox( const SCH_TEXTBOX* aTextBox, const wxString& aText,
                               const VECTOR2I& aPos, const COLOR4D& aColor,
                               bool aHideIfSmall ) const
{
    // Draw a text box
    // This is a placeholder - implement specific drawing logic for text boxes
}


void CHEM_PAINTER::drawLabel( const SCH_TEXT* aLabel, const wxString& aText, const VECTOR2I& aPos,
                             const COLOR4D& aColor, bool aHideIfSmall ) const
{
    // Draw a label
    // This is a placeholder - implement specific drawing logic for labels
} 