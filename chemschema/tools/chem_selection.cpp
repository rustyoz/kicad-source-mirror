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

#include <tools/chem_selection.h>
#include <chem_item.h>
#include <chem_symbol.h>
#include <chem_schematic.h>

CHEM_SELECTION::CHEM_SELECTION( CHEM_SCHEMATIC* aSchematic ) :
        SELECTION()
{
    m_schematic = aSchematic;
}

EDA_ITEM* CHEM_SELECTION::GetTopLeftItem( bool onlyModules ) const
{
    if( m_items.empty() )
        return nullptr;

    EDA_ITEM* topLeftItem = m_items[0];
    VECTOR2I  topLeftPos = topLeftItem->GetPosition();

    for( EDA_ITEM* item : m_items )
    {
        VECTOR2I pos = item->GetPosition();

        if( pos.y < topLeftPos.y || ( pos.y == topLeftPos.y && pos.x < topLeftPos.x ) )
        {
            topLeftItem = item;
            topLeftPos = pos;
        }
    }

    return topLeftItem;
}

BOX2I CHEM_SELECTION::GetBoundingBox() const
{
    BOX2I bbox;

    for( EDA_ITEM* item : m_items )
    {
        if( item->Type() == CHEM_SYMBOL_T )
        {
            bbox.Merge( static_cast<CHEM_SYMBOL*>( item )->GetBoundingBox() );
        }
        else
        {
            bbox.Merge( item->GetBoundingBox() );
        }
    }

    return bbox;
}

const std::vector<KIGFX::VIEW_ITEM*> CHEM_SELECTION::updateDrawList() const
{
    std::vector<VIEW_ITEM*> items;

    for( EDA_ITEM* item : m_items )
        items.push_back( item );

    return items;
} 