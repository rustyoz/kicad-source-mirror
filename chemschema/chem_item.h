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

#ifndef CHEM_ITEM_H
#define CHEM_ITEM_H

#include <eda_item.h>

/**
 * Base class for all chemical diagram items.
 */
class CHEM_ITEM : public EDA_ITEM
{
public:
    CHEM_ITEM( EDA_ITEM* aParent = nullptr, KICAD_T aItemType = CHEM_ITEM_T );
    virtual ~CHEM_ITEM();

    /**
     * Enum for chemical item types
     */
    enum CHEM_ITEM_T
    {
        CHEM_ITEM_T = 10000, // Base value to avoid conflicts with other types
        CHEM_SHAPE_T,
        CHEM_TEXT_T,
        CHEM_TEXTBOX_T,
        CHEM_LINE_T,
        CHEM_SYMBOL_T,
        CHEM_SHEET_T,
        CHEM_JUNCTION_T,
        CHEM_LABEL_T,
        CHEM_SCREEN_T
    };

    virtual wxString GetClass() const override
    {
        return wxT( "CHEM_ITEM" );
    }

    virtual void ViewGetLayers( int aLayers[], int& aCount ) const override
    {
        // Default implementation - to be overridden by derived classes
        aCount = 0;
    }
};

#endif // CHEM_ITEM_H 