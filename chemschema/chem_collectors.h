/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2024 KiCad Developers.
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

#ifndef CHEM_COLLECTORS_H
#define CHEM_COLLECTORS_H

#include <collector.h>
#include "chem_item.h"

/**
 * Class CHEM_COLLECTOR
 *
 * Handles collection of chemical schematic items based on various criteria.
 * This class is used to gather items for selection, connectivity, and other operations.
 */
class CHEM_COLLECTOR : public COLLECTOR
{
public:
    CHEM_COLLECTOR() {};
    ~CHEM_COLLECTOR() {};

    /**
     * A scan list for all chemical items at the given point.
     * This filter is used to determine which items can be collected.
     */
    static const COLLECTOR_FILTER ChemicalItemsFilter;

    /**
     * Operator overload to return a CHEM_ITEM* instead of an EDA_ITEM*
     * @param ndx The index into the list.
     * @return CHEM_ITEM* - The chemical item at the given index.
     */
    CHEM_ITEM* operator[]( int ndx ) const
    {
        return static_cast<CHEM_ITEM*>( m_List[ ndx ] );
    }

    /**
     * Function Inspect
     * is the examining function within the INSPECTOR which is passed to the Iterate function.
     *
     * @param aItem An EDA_ITEM to examine.
     * @param aTestData is not used in this class.
     * @return SEARCH_RESULT - SEARCH_QUIT if the iterator is to stop.
     */
    SEARCH_RESULT Inspect( EDA_ITEM* aItem, void* aTestData ) override;

    /**
     * Collect items at a given point that match our filter criteria.
     *
     * @param aSheet The chemical sheet to search in.
     * @param aPosition The position to search around.
     * @param aFilter The filter to use for item collection (defaults to ChemicalItemsFilter).
     */
    void Collect( CHEM_SHEET* aSheet, const VECTOR2I& aPosition,
                 const COLLECTOR_FILTER& aFilter = ChemicalItemsFilter );
};

#endif // CHEM_COLLECTORS_H 