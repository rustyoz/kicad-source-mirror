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

#ifndef CHEM_VIEW_H
#define CHEM_VIEW_H

#include <view/view.h>



/**
 * Class to store display options for the chemical schematic
 */
class CHEM_DISPLAY_OPTIONS
{
public:
    bool m_showGrid;
    bool m_showLabels;
    bool m_showConnections;
    bool m_showSymbols;
    
    CHEM_DISPLAY_OPTIONS() :
        m_showGrid( true ),
        m_showLabels( true ),
        m_showConnections( true ),
        m_showSymbols( true )
    {
    }
};

class CHEM_SCHEMATIC;

/**
 * Class CHEM_VIEW
 * is a VIEW implementation for the chemical schematic.
 */
class CHEM_VIEW : public KIGFX::VIEW
{
public:
    /**
     * Constructor
     * @param aIsDynamic True for a dynamic view, false for a static view
     */
    CHEM_VIEW( bool aIsDynamic = true );
    
    ~CHEM_VIEW();

    // Set the chemical schematic model for this view
    void SetChemSchematic( CHEM_SCHEMATIC* aChemSchematic );
    
    // Get the chemical schematic model
    CHEM_SCHEMATIC* GetChemSchematic() const;
    
    // Update all items in the view
    void UpdateAllItems();
    
    // Update display options
    void UpdateDisplayOptions( const CHEM_DISPLAY_OPTIONS& aOptions );

private:
    CHEM_SCHEMATIC* m_chemSchematic;
};

#endif // CHEM_VIEW_H 