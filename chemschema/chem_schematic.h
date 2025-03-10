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

#ifndef CHEM_SCHEMATIC_H
#define CHEM_SCHEMATIC_H

#include <base_struct.h>
#include <eda_item.h>
#include <tool/tool_event.h>
#include <tool/tool_interactive.h>

class CHEM_EDIT_FRAME;

/**
 * Class representing a chemical process flow diagram
 */
class CHEM_SCHEMATIC : public EDA_ITEM
{
public:
    CHEM_SCHEMATIC( CHEM_EDIT_FRAME* aFrame );
    virtual ~CHEM_SCHEMATIC();

    virtual EDA_ITEM* Clone() const override { return new CHEM_SCHEMATIC( *this ); }

    CHEM_EDIT_FRAME* GetFrame() const { return m_frame; }

    /**
     * Save the chemical process flow diagram to a file
     */
    bool SaveFile( const wxString& aFileName );

    /**
     * Load a chemical process flow diagram from a file
     */
    bool LoadFile( const wxString& aFileName );
    
    /**
     * Clear the current chemical process flow diagram
     */
    void Clear();

    /**
     * Add a component to the chemical process flow diagram
     */
    void AddItem( EDA_ITEM* aItem );

    /**
     * Remove a component from the chemical process flow diagram
     */
    void RemoveItem( EDA_ITEM* aItem );
    
    /**
     * Notify the view system of any changes
     */
    void UpdateView();

private:
    CHEM_SCHEMATIC( const CHEM_SCHEMATIC& other );

    CHEM_EDIT_FRAME* m_frame;
    std::vector<EDA_ITEM*> m_items;
};

#endif // CHEM_SCHEMATIC_H 