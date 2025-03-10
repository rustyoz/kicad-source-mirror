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

#ifndef CHEMLAYOUT_SETTINGS_H
#define CHEMLAYOUT_SETTINGS_H

#include <settings/app_settings.h>
#include <settings/common_settings.h>

namespace CHEMLAYOUT
{
    // Frame identifiers
    enum FRAME_T
    {
        FRAME_CHEM_LAYOUT = 0
    };
}

class CHEMLAYOUT_SETTINGS : public APP_SETTINGS_BASE
{
public:
    CHEMLAYOUT_SETTINGS();

    virtual ~CHEMLAYOUT_SETTINGS() {}

    virtual bool MigrateFromLegacy( wxConfigBase* aLegacyConfig ) override;

protected:
    virtual std::string getLegacyFrameName() const override { return "ChemLayout"; }
};

#endif  // CHEMLAYOUT_SETTINGS_H 