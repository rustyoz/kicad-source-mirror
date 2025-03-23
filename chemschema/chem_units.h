/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2024 KiCad Developers.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHEM_UNITS_H
#define CHEM_UNITS_H

#include <base_units.h>

// Chemical schematic internal units are 100nm (same as schematic)
constexpr double CHEM_IU_PER_MM = 1e4;

// Define the chemical schematic scale using the same pattern as other modules
constexpr EDA_IU_SCALE chemIUScale = EDA_IU_SCALE( CHEM_IU_PER_MM );

#endif // CHEM_UNITS_H 