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

#include "layout_view.h"
#include "chem_layout.h"
#include <gal/graphics_abstraction_layer.h>
#include <view/view_item.h>
#include <view/view_group.h>

LAYOUT_VIEW::LAYOUT_VIEW( bool aIsPrinting ) :
    KIGFX::VIEW( aIsPrinting ),
    m_chemLayout( nullptr )
{
    // Initialize view settings
    m_scale = 1.0;
    m_mirrorX = false;
    m_mirrorY = false;
    
    // Set default layer order
    for( int i = 0; i < LAYOUT_LAYER_ID_COUNT; i++ )
    {
        SetLayerOrder( i, i );
    }
}


LAYOUT_VIEW::~LAYOUT_VIEW()
{
    // Nothing to do here, the layout is owned elsewhere
}


void LAYOUT_VIEW::SetChemLayout( CHEM_LAYOUT* aLayout )
{
    m_chemLayout = aLayout;
    
    if( m_chemLayout )
    {
        // Add all items from the layout to the view
        // This is a placeholder for actual implementation
        // In a real implementation, we would iterate through all items in the layout
        // and add them to the view using Add() method
    }
}


CHEM_LAYOUT* LAYOUT_VIEW::GetChemLayout() const
{
    return m_chemLayout;
}


void LAYOUT_VIEW::UpdateAllItems()
{
    // Force redraw of all items
    for( int i = 0; i < GetItemCount(); ++i )
    {
        KIGFX::VIEW_ITEM* item = GetItem( i );
        
        if( item )
        {
            item->ViewUpdate( KIGFX::VIEW_ITEM::GEOMETRY );
            item->ViewUpdate( KIGFX::VIEW_ITEM::APPEARANCE );
        }
    }
    
    UpdateItems();
}


void LAYOUT_VIEW::UpdateDisplayOptions( const LAYOUT_DISPLAY_OPTIONS& aOptions )
{
    // Update display options for the view
    // This is a placeholder for actual implementation
    // In a real implementation, we would update the view settings based on the options
    
    // Force redraw with new options
    UpdateAllItems();
} 