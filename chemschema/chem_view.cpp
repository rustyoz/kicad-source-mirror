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

#include "chem_view.h"
#include <gal/graphics_abstraction_layer.h>
#include <view/view_item.h>
#include <view/view_group.h>

CHEM_VIEW::CHEM_VIEW( bool aIsDynamic ) :
    KIGFX::VIEW( aIsDynamic ),
    m_chemSchematic( nullptr )
{
    // Initialize view settings
    m_scale = 1.0;
    m_mirrorX = false;
    m_mirrorY = false;
    
    // Set default layer order
    for( int i = 0; i < CHEM_LAYER_ID_COUNT; i++ )
    {
        SetLayerOrder( i, i );
    }
}


CHEM_VIEW::~CHEM_VIEW()
{
    // Nothing to do here, the schematic is owned elsewhere
}


void CHEM_VIEW::SetChemSchematic( CHEM_SCHEMATIC* aSchematic )
{
    m_chemSchematic = aSchematic;
    
    if( m_chemSchematic )
    {
        // Add all items from the schematic to the view
        // This is a placeholder for actual implementation
        // In a real implementation, we would iterate through all items in the schematic
        // and add them to the view using Add() method
    }
}


CHEM_SCHEMATIC* CHEM_VIEW::GetChemSchematic() const
{
    return m_chemSchematic;
}


void CHEM_VIEW::UpdateAllItems()
{
    // This is a placeholder implementation
    // In the actual implementation, we would update all view items
    
    // We're not using GetItemCount() and GetItem() as they don't exist in the current VIEW API
    // Instead, we'll use a more direct approach to update the view
    
    // Mark the view as dirty to force a redraw
    MarkDirty();
    
    // Request a redraw of all layers
    for( int i = 0; i < CHEM_LAYER_ID_COUNT; i++ )
    {
        MarkTargetDirty( i );
    }
    
    // Call the base class's update method
    UpdateItems();
}


void CHEM_VIEW::UpdateDisplayOptions( const CHEM_DISPLAY_OPTIONS& aOptions )
{
    // Update display options for the view
    // This is a placeholder for actual implementation
    // In a real implementation, we would update the view settings based on the options
    
    // Force redraw with new options
    UpdateAllItems();
} 