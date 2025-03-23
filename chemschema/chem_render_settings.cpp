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

#include <chem_render_settings.h>
#include <chem_symbol.h>
#include <chem_sheet.h>
#include <chem_screen.h>
#include <lib_symbol.h>
#include <lib_shape.h>
#include <lib_text.h>
#include <lib_textbox.h>
#include <lib_pin.h>
#include <lib_field.h>
#include <settings/color_settings.h>

using namespace KIGFX;

CHEM_RENDER_SETTINGS::CHEM_RENDER_SETTINGS()
{
    m_ShowUnit = true;
    m_ShowBodyStyle = true;
    m_ShowPinNumbers = true;
    m_ShowPinNames = true;
    m_ShowPinElectricalType = true;
    m_ShowDisabled = false;
    m_ShowGraphicsDisabled = false;
    m_ShowHiddenText = false;
    m_ShowHiddenPins = false;
    m_ShowPinFunction = false;
    m_ShowPinNetname = false;
    m_ShowPinType = false;
    m_ShowPinShape = false;
    m_ShowPinAttributes = false;
    m_ShowPinDescription = false;
    m_ShowPinLength = false;
    m_ShowPinPadName = false;
    m_ShowPinPadNumber = false;
    m_ShowPinPadShape = false;
    m_ShowPinPadSize = false;
    m_ShowPinPadLayers = false;
    m_ShowPinPadType = false;
    m_ShowPinPadOrientation = false;
    m_ShowPinPadDrill = false;
    m_ShowPinPadOffset = false;
    m_ShowPinPadClearance = false;
    m_ShowPinPadMask = false;
    m_ShowPinPadPaste = false;
    m_ShowPinPadThermal = false;
    m_ShowPinPadZone = false;
    m_ShowPinPadNet = false;
    m_ShowPinPadNetClass = false;
    m_ShowPinPadNetName = false;
    m_ShowPinPadNetNumber = false;
    m_ShowPinPadNetType = false;
    m_ShowPinPadNetAttributes = false;
    m_ShowPinPadNetDescription = false;
    m_ShowPinPadNetClassName = false;
    m_ShowPinPadNetClassDescription = false;
    m_ShowPinPadNetClassAttributes = false;
    m_ShowPinPadNetClassClearance = false;
    m_ShowPinPadNetClassTrackWidth = false;
    m_ShowPinPadNetClassViaDiameter = false;
    m_ShowPinPadNetClassViaDrill = false;
    m_ShowPinPadNetClassDiffPairWidth = false;
    m_ShowPinPadNetClassDiffPairGap = false;
    m_ShowPinPadNetClassDiffPairViaGap = false;
    m_ShowPinPadNetClassUViaSize = false;
    m_ShowPinPadNetClassUViaDrill = false;
    m_ShowPinPadNetClassUViaPadSize = false;
    m_ShowPinPadNetClassUViaMask = false;
    m_ShowPinPadNetClassUViaPaste = false;
    m_ShowPinPadNetClassUViaThermal = false;
    m_ShowPinPadNetClassUViaZone = false;
    m_ShowPinPadNetClassUViaNet = false;
    m_ShowPinPadNetClassUViaNetClass = false;
    m_ShowPinPadNetClassUViaNetName = false;
    m_ShowPinPadNetClassUViaNetNumber = false;
    m_ShowPinPadNetClassUViaNetType = false;
    m_ShowPinPadNetClassUViaNetAttributes = false;
    m_ShowPinPadNetClassUViaNetDescription = false;

    m_DefaultLineWidth = 6.0;
    m_TextOffsetRatio = 0.08;
    m_PinSymbolSize = 6.0;
    m_JunctionSize = 2.0;
    m_DashedLineDashRatio = 0.5;
    m_DashedLineGapRatio = 0.5;
    m_LabelSizeRatio = 0.6;

    update();
}


void CHEM_RENDER_SETTINGS::LoadColors( const COLOR_SETTINGS* aSettings )
{
    // Load colors from the color settings
    for( int i = 0; i < LAYER_ID_COUNT; i++ )
    {
        m_LayerColors[i] = aSettings->GetColor( i );
    }

    // Set specific colors for chemical schematic items
    m_LayerColors[LAYER_CHEM_SYMBOL] = aSettings->GetColor( LAYER_CHEM_SYMBOL );
    m_LayerColors[LAYER_CHEM_SHEET] = aSettings->GetColor( LAYER_CHEM_SHEET );
    m_LayerColors[LAYER_CHEM_JUNCTION] = aSettings->GetColor( LAYER_CHEM_JUNCTION );
    m_LayerColors[LAYER_CHEM_LINE] = aSettings->GetColor( LAYER_CHEM_LINE );
    m_LayerColors[LAYER_CHEM_BUS_WIRE_ENTRY] = aSettings->GetColor( LAYER_CHEM_BUS_WIRE_ENTRY );
    m_LayerColors[LAYER_CHEM_BUS_BUS_ENTRY] = aSettings->GetColor( LAYER_CHEM_BUS_BUS_ENTRY );
    m_LayerColors[LAYER_CHEM_NO_CONNECT] = aSettings->GetColor( LAYER_CHEM_NO_CONNECT );
    m_LayerColors[LAYER_CHEM_PIN] = aSettings->GetColor( LAYER_CHEM_PIN );
    m_LayerColors[LAYER_CHEM_FIELD] = aSettings->GetColor( LAYER_CHEM_FIELD );
    m_LayerColors[LAYER_CHEM_TEXT] = aSettings->GetColor( LAYER_CHEM_TEXT );
    m_LayerColors[LAYER_CHEM_TEXTBOX] = aSettings->GetColor( LAYER_CHEM_TEXTBOX );
    m_LayerColors[LAYER_CHEM_HIERLABEL] = aSettings->GetColor( LAYER_CHEM_HIERLABEL );
    m_LayerColors[LAYER_CHEM_GLOBALLABEL] = aSettings->GetColor( LAYER_CHEM_GLOBALLABEL );
}


const COLOR4D& CHEM_RENDER_SETTINGS::GetColor( const VIEW_ITEM* aItem, int aLayer ) const
{
    const EDA_ITEM* item = static_cast<const EDA_ITEM*>( aItem );

    if( !item )
        return m_LayerColors[aLayer];

    // Get the appropriate color based on the item type and layer
    switch( item->Type() )
    {
    case CHEM_SYMBOL_T:
        return m_LayerColors[LAYER_CHEM_SYMBOL];

    case CHEM_SHEET_T:
        return m_LayerColors[LAYER_CHEM_SHEET];

    case CHEM_JUNCTION_T:
        return m_LayerColors[LAYER_CHEM_JUNCTION];

    case CHEM_LINE_T:
        return m_LayerColors[LAYER_CHEM_LINE];

    case CHEM_BUS_WIRE_ENTRY_T:
        return m_LayerColors[LAYER_CHEM_BUS_WIRE_ENTRY];

    case CHEM_BUS_BUS_ENTRY_T:
        return m_LayerColors[LAYER_CHEM_BUS_BUS_ENTRY];

    case CHEM_NO_CONNECT_T:
        return m_LayerColors[LAYER_CHEM_NO_CONNECT];

    case CHEM_PIN_T:
        return m_LayerColors[LAYER_CHEM_PIN];

    case CHEM_FIELD_T:
        return m_LayerColors[LAYER_CHEM_FIELD];

    case CHEM_TEXT_T:
        return m_LayerColors[LAYER_CHEM_TEXT];

    case CHEM_TEXTBOX_T:
        return m_LayerColors[LAYER_CHEM_TEXTBOX];

    case CHEM_HIERLABEL_T:
        return m_LayerColors[LAYER_CHEM_HIERLABEL];

    case CHEM_GLOBALLABEL_T:
        return m_LayerColors[LAYER_CHEM_GLOBALLABEL];

    default:
        return m_LayerColors[aLayer];
    }
} 