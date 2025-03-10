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

#include <kiface_base.h>
#include <kiway.h>
#include <pgm_base.h>
#include <settings/settings_manager.h>
#include <wildcards_and_files_ext.h>

#include "chem_edit_frame.h"
#include "chemlayout_settings.h"

namespace CHEMLAYOUT {

static struct IFACE : public KIFACE_BASE
{
    // Of course all are virtual override.
    bool OnKifaceStart( PGM_BASE* aProgram, int aCtlBits ) override;

    void OnKifaceEnd() override;

    wxWindow* CreateWindow( wxWindow* aParent, int aClassId, KIWAY* aKiway,
                            int aCtlBits = 0 ) override;

    /**
     * Return a pointer to the requested object.
     *
     * The safest way to use this is to retrieve a pointer to a static instance of an
     * interface, similar to how the KIFACE interface is exported.  But if you know what
     * you are doing use it to retrieve anything you want.
     */
    void* IfaceOrAddress( int aDataId ) override;

    /**
     * Return the initialized or created settings manager reference.
     */
    SETTINGS_MANAGER& GetSettingsManager() override;

private:
    SETTINGS_MANAGER* m_settings_manager;

} kiface( "chemlayout", KIWAY::FACE_CHEM_LAYOUT );

} // namespace CHEMLAYOUT

using namespace CHEMLAYOUT;

static int kiChemlayoutSingletonCount = 0;

// KIFACE_GETTER's job is to create the KIFACE side of this binary's
// KIFACE, which is typically only a C++ singleton class, along with
// its few global variables like the single KIWAY.
KIFACE_GETTER( &kiface )

// KIFACE_GETTER_EXTRA is macro debris left from a previous API, not used in C++ context.
KIFACE_GETTER_EXTRA( "This is the Chemical Reactor Layout editor." )


bool IFACE::OnKifaceStart( PGM_BASE* aProgram, int aCtlBits )
{
    m_settings_manager = new SETTINGS_MANAGER;
    m_settings_manager->Init();

    // Ensure that default values are loaded into CHEMLAYOUT_SETTINGS
    // before the frame is created
    auto chemlayoutSetting = m_settings_manager->GetAppSettings<CHEMLAYOUT_SETTINGS>();

    return true;
}


void IFACE::OnKifaceEnd()
{
    delete m_settings_manager;
    m_settings_manager = nullptr;
}


SETTINGS_MANAGER& IFACE::GetSettingsManager()
{
    wxASSERT( m_settings_manager );
    return *m_settings_manager;
}


wxWindow* IFACE::CreateWindow( wxWindow* aParent, int aClassId, KIWAY* aKiway, int aCtlBits )
{
    wxASSERT( m_settings_manager );

    switch( aClassId )
    {
    case FRAME_CHEM_LAYOUT:
    {
        CHEM_EDIT_FRAME* frame = new CHEM_EDIT_FRAME( aKiway, aParent );

        if( frame )
        {
            frame->SwitchColorScheme( aKiway->CommonSettings().GetColorTheme() );

            if( Kiface().IsSingle() )
            {
                kiChemlayoutSingletonCount++;
                return frame;
            }
            
            return frame;
        }
    }
    break;

    default:
        return nullptr;
    }

    return nullptr;
}


void* IFACE::IfaceOrAddress( int aDataId )
{
    return nullptr;
} 