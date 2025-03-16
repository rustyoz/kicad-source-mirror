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

#ifndef CHEM_PLANT_H
#define CHEM_PLANT_H

#include <vector>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include "chem_layout_item.h"

/**
 * Class CHEM_PLANT
 * Represents a complete chemical plant in the reactor layout.
 * A plant can contain multiple items like reactors, pipes, etc.
 */
class CHEM_PLANT : public CHEM_LAYOUT_ITEM
{
public:
    CHEM_PLANT();
    virtual ~CHEM_PLANT();

    /**
     * Set the name of the plant
     * @param aName The name to set
     */
    void SetName( const wxString& aName );

    /**
     * Get the name of the plant
     * @return The plant name
     */
    wxString GetName() const;

    /**
     * Set the description of the plant
     * @param aDescription The description to set
     */
    void SetDescription( const wxString& aDescription );

    /**
     * Get the description of the plant
     * @return The plant description
     */
    wxString GetDescription() const;

    /**
     * Set the position of the plant
     * @param aPosition The position coordinates
     */
    void SetPosition( const wxPoint& aPosition );

    /**
     * Get the position of the plant
     * @return The position coordinates
     */
    wxPoint GetPosition() const;

    /**
     * Add an item to the plant
     * @param aItem The item to add
     */
    void AddItem( CHEM_LAYOUT_ITEM* aItem );

    /**
     * Remove an item from the plant
     * @param aItem The item to remove
     */
    void RemoveItem( CHEM_LAYOUT_ITEM* aItem );

    /**
     * Clear all items from the plant
     */
    void ClearPlantItems();

    /**
     * Get the number of items in the plant
     * @return The item count
     */
    size_t GetItemCount() const;

    /**
     * Get an item by index
     * @param aIndex The index of the item
     * @return Pointer to the item or nullptr if index is out of range
     */
    CHEM_LAYOUT_ITEM* GetItem( size_t aIndex );

    /**
     * Save the plant to a file
     * @param aFileName The file to save to
     * @return True if successful, false otherwise
     */
    bool Save( const wxString& aFileName );

    /**
     * Load a plant from a file
     * @param aFileName The file to load from
     * @return True if successful, false otherwise
     */
    bool Load( const wxString& aFileName );

private:
    wxString m_name;        ///< Name of the plant
    wxString m_description; ///< Description of the plant
    wxPoint m_position;     ///< Position of the plant
    std::vector<CHEM_LAYOUT_ITEM*> m_items; ///< Items in the plant
};

#endif // CHEM_PLANT_H 