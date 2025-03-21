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

#include "chem_item.h"
#include "chem_screen.h"
#include "chem_schematic.h"

CHEM_ITEM::CHEM_ITEM( EDA_ITEM* aParent, KICAD_T aItemType ) :
        EDA_ITEM( aParent, aItemType )
{
    // Initialize any chemical item specific properties here
}


CHEM_ITEM::~CHEM_ITEM()
{
    // Clean up any resources specific to chemical items here
}

wxString CHEM_ITEM::GetClass() const
{
    return wxT( "CHEM_ITEM" );
}

std::vector<int> CHEM_ITEM::ViewGetLayers() const
{
    // Default implementation - to be overridden by derived classes
    return std::vector<int>();
}

wxString CHEM_ITEM::GetSelectMenuText( EDA_UNITS aUnits ) const
{
    // Default implementation - to be overridden by derived classes
    return GetClass();
}

bool CHEM_ITEM::IsMovableFromAnchorPoint() const
{
    return true;
}

CHEM_SCHEMATIC* CHEM_ITEM::Schematic() const
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

CHEM_SCREEN* CHEM_ITEM::Screen() const
{
    EDA_ITEM* parent = GetParent();

    while( parent )
    {
        if( parent->Type() == CHEM_SCREEN_T )
            return static_cast<CHEM_SCREEN*>( parent );
        else
            parent = parent->GetParent();
    }

    return nullptr;
}

bool CHEM_ITEM::IsLocked() const
{
    return false;
}

void CHEM_ITEM::SetLocked( bool aLocked )
{
    // Default implementation - to be overridden by derived classes
}

bool CHEM_ITEM::IsHypertext() const
{
    return false;
}

void CHEM_ITEM::DoHypertextAction( EDA_DRAW_FRAME* aFrame ) const
{
    // Default implementation - to be overridden by derived classes
}

int CHEM_ITEM::GetLayer() const
{
    return 0;
}

void CHEM_ITEM::SetLayer( int aLayer )
{
    // Default implementation - to be overridden by derived classes
}


void CHEM_ITEM::SetPosition( const VECTOR2I& aPos )
{
    m_pos = aPos;
}

VECTOR2I CHEM_ITEM::GetPosition() const
{
    return m_pos;
}

const BOX2I CHEM_ITEM::GetBoundingBox() const
{
    // Default implementation - to be overridden by derived classes
    return BOX2I( m_pos, VECTOR2I( 0, 0 ) );
}

bool CHEM_ITEM::HitTest( const VECTOR2I& aPosition, int aAccuracy ) const
{
    // Default implementation - to be overridden by derived classes
    return false;
}

bool CHEM_ITEM::HitTest( const BOX2I& aRect, bool aContained, int aAccuracy ) const
{
    // Default implementation - to be overridden by derived classes
    return false;
}

wxString CHEM_ITEM::GetItemDescription( UNITS_PROVIDER* aUnitsProvider, bool aFull ) const
{
    // Default implementation - to be overridden by derived classes
    return GetClass();
} 