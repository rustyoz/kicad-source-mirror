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

#ifndef CHEM_SELECTION_H
#define CHEM_SELECTION_H

#include <tool/selection.h>

class CHEM_SCHEMATIC;

class CHEM_SELECTION : public SELECTION
{
    /**
     * Schematic of selected objects. Used to fetch chemical symbols for copy.
     */
    CHEM_SCHEMATIC* m_schematic;

public:
    CHEM_SELECTION( CHEM_SCHEMATIC* aSchematic = nullptr );

    EDA_ITEM* GetTopLeftItem( bool onlyModules = false ) const override;

    BOX2I GetBoundingBox() const override;

    void SetSchematic( CHEM_SCHEMATIC* aSchematic ) { m_schematic = aSchematic; }
    CHEM_SCHEMATIC* GetSchematic() { return m_schematic; }

    const std::vector<KIGFX::VIEW_ITEM*> updateDrawList() const override;
};

#endif  //  CHEM_SELECTION_H 