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

#ifndef LAYOUT_VIEW_H
#define LAYOUT_VIEW_H

#include <view/view.h>

class CHEM_LAYOUT;

/**
 * Enum for chemical layout layers
 */
enum LAYOUT_LAYER_ID
{
    LAYOUT_LAYER_BACKGROUND,
    LAYOUT_LAYER_GRID,
    LAYOUT_LAYER_PIPES,
    LAYOUT_LAYER_REACTORS,
    LAYOUT_LAYER_LABELS,
    LAYOUT_LAYER_SELECTION,
    LAYOUT_LAYER_ID_COUNT
};

/**
 * Class to store display options for the chemical layout
 */
class LAYOUT_DISPLAY_OPTIONS
{
public:
    bool m_showGrid;
    bool m_showLabels;
    bool m_showPipes;
    bool m_showReactors;
    
    LAYOUT_DISPLAY_OPTIONS() :
        m_showGrid( true ),
        m_showLabels( true ),
        m_showPipes( true ),
        m_showReactors( true )
    {
    }
};

/**
 * View class for the chemical reactor layout
 */
class LAYOUT_VIEW : public KIGFX::VIEW
{
public:
    /**
     * Constructor
     * @param aIsPrinting - True if the view is for printing, false otherwise
     */
    LAYOUT_VIEW( bool aIsPrinting = false );
    
    /**
     * Destructor
     */
    ~LAYOUT_VIEW();
    
    /**
     * Set the chemical layout
     * @param aLayout - The chemical layout
     */
    void SetChemLayout( CHEM_LAYOUT* aLayout );
    
    /**
     * Get the chemical layout
     * @return The chemical layout
     */
    CHEM_LAYOUT* GetChemLayout() const;
    
    /**
     * Update all items in the view
     */
    void UpdateAllItems();
    
    /**
     * Update the display options
     * @param aOptions - The display options
     */
    void UpdateDisplayOptions( const LAYOUT_DISPLAY_OPTIONS& aOptions );
    
private:
    CHEM_LAYOUT* m_chemLayout;
};

#endif // LAYOUT_VIEW_H 