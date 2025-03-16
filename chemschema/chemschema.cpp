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
#include <wx/app.h>
#include <wx/event.h>
#include <wx/log.h>
#include <wx/config.h>
#include <wx/window.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

#include "chem_frame.h"

/**
 * Main application class for the Chemical Process Flow Diagram editor
 */
class CHEM_SCHEMA_APP : public wxApp
{
public:
    CHEM_SCHEMA_APP() {}

    /**
     * Called when the application starts
     */
    bool OnInit() override
    {
        CHEM_FRAME* frame = new CHEM_FRAME( nullptr, nullptr );
        SetTopWindow( frame );
        frame->Show( true );
        
        return true;
    }
};

wxIMPLEMENT_APP( CHEM_SCHEMA_APP );

/**
 * KIFACE_I implementation for the Chemical Process Flow Diagram editor
 */
namespace CHEMSCHEMA {

static struct IFACE : public KIFACE_I
{
    // Of course all are overloads, implementations of virtual functions in KIFACE_I

    bool OnKifaceStart( PGM_BASE* aProgram, int aCtlBits ) override
    {
        return start_common( aProgram );
    }
    
    void OnKifaceEnd() override
    {
        end_common();
    }
    
    wxWindow* CreateWindow( wxWindow* aParent, int aClassId, KIWAY* aKiway,
                            int aCtlBits = 0 ) override
    {
        switch( aClassId )
        {
        case FRAME_CHEM_SCHEMA:
            {
                CHEM_FRAME* frame = new CHEM_FRAME( aKiway, aParent );
                return frame;
            }
            break;
            
        default:
            return nullptr;
        }
    }
    
    /**
     * Function IfaceOrAddress
     * returns a pointer to the requested object.
     *
     * @param aDataId identifies which object you want the address of.
     *
     * @return void* - and must be cast into the know type.
     */
    void* IfaceOrAddress( int aDataId ) override
    {
        return NULL;
    }
    
    /**
     * Function ReturnSingle
     * is a simplified version of CreateWindow() for use when there can be only one
     * main window of aClassId open at a time.
     */
    wxWindow* ReturnSingle( wxWindow* aParent, int aClassId, KIWAY* aKiway,
                            int aCtlBits, wxString* aWindowTitle = nullptr ) override
    {
        return CreateWindow( aParent, aClassId, aKiway, aCtlBits );
    }

} kiface;

} // namespace

static KIFACE_I& Kiface() { return CHEMSCHEMA::kiface; }

// KIFACE_GETTER's job is to create KIFACE on demand and return it.
KIFACE_GETTER( &Kiface )

// KIFACE_GETTER_EXTRA is macro debris left from a previous API, not used in C++ context.
KIFACE_GETTER_EXTRA( "This is the Chemical Process Flow Diagram editor." )


bool IFACE::OnKifaceStart( PGM_BASE* aProgram, int aCtlBits )
{
    m_settings_manager = new SETTINGS_MANAGER;
    m_settings_manager->Init();

    // Ensure that default values are loaded into CHEMSCHEMA_SETTINGS
    // before the frame is created
    auto chemschemaSetting = m_settings_manager->GetAppSettings<CHEMSCHEMA_SETTINGS>();

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
    case FRAME_CHEM_SCHEMA:
    {
        CHEM_FRAME* frame = new CHEM_FRAME( aKiway, aParent );

        if( frame )
        {
            frame->SwitchColorScheme( aKiway->CommonSettings().GetColorTheme() );

            if( Kiface().IsSingle() )
            {
                kiChemschemaSingletonCount++;
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