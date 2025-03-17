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

#include <settings/parameters.h>
#include "chemschema_settings.h"

// Schema version for the chemschema settings
const int chemschemaSchemaVersion = 0;

CHEMSCHEMA_SETTINGS::CHEMSCHEMA_SETTINGS() :
        APP_SETTINGS_BASE( "chemschema", chemschemaSchemaVersion )
{
    m_params.emplace_back( new PARAM<wxString>( "project.last_symbol_lib_dir",
            &m_lastLibDir, "" ) );

    m_params.emplace_back( new PARAM<int>( "window.grid.last_size_idx",
            &m_Window.grid.last_size_idx, 0 ) );

    m_params.emplace_back( new PARAM<bool>( "window.grid.show",
            &m_Window.grid.show, true ) );

    m_params.emplace_back( new PARAM<bool>( "window.grid.snap",
            &m_Window.grid.snap, true ) );

    m_params.emplace_back( new PARAM<bool>( "graphics.use_line_thickness",
            &m_General.use_line_thickness, true ) );

    // Add more settings as needed
}


bool CHEMSCHEMA_SETTINGS::MigrateFromLegacy( wxConfigBase* aLegacyConfig )
{
    // No migration needed for a new application
    return false;
} 