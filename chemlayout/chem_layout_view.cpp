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

#include <view/view.h>
#include "chem_layout_view.h"
#include "chem_layout.h"

CHEM_LAYOUT_VIEW::CHEM_LAYOUT_VIEW( bool aIsDynamic ) :
        KIGFX::VIEW( aIsDynamic )
{
    m_chemLayout = nullptr;
}


CHEM_LAYOUT_VIEW::~CHEM_LAYOUT_VIEW()
{
    // The view doesn't own the layout, so don't delete it
}


void CHEM_LAYOUT_VIEW::SetChemLayout( CHEM_LAYOUT* aChemLayout )
{
    m_chemLayout = aChemLayout;
    
    if( m_chemLayout )
    {
        // Add the layout items to the view
        // This will be implemented as the layout develops
    }
} 