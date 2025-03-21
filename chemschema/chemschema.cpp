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
#include <wx/app.h>
#include <wx/event.h>
#include <wx/log.h>
#include <wx/config.h>
#include <wx/window.h>
#include <wx/cmdline.h>
#include <wx/filename.h>
#include <frame_type.h>

#include "chem_frame.h"
#include "chemschema_settings.h"

// Define the chemical schema frame type ID
#define FRAME_CHEM_SCHEMA FRAME_SCH_SYMBOL_EDITOR

// Global singleton pointer count - for standalone mode
static int kiChemschemaSingletonCount = 0;

/**
 * Main application class for the Chemical Process Flow Diagram editor
 * This is used for standalone mode
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
        // In standalone mode, we need to create the frame differently
        // Use nullptr for KIWAY since we're not integrated into a project
        CHEM_FRAME* frame = new CHEM_FRAME( nullptr );
        
        if( frame )
        {
            SetTopWindow( frame );
            frame->Show( true );
            return true;
        }
        
        return false;
    }
};

wxIMPLEMENT_APP( CHEM_SCHEMA_APP );

/**
 * KIFACE implementation for the Chemical Process Flow Diagram editor
 */
namespace CHEMSCHEMA {

static struct IFACE : public KIFACE_BASE
{
    // Of course all are overloads, implementations of virtual functions in KIFACE_I

    IFACE(const char* aName, KIWAY::FACE_T aType) : KIFACE_BASE(aName, aType)
    {
        m_settings_manager = nullptr;
    }

    bool OnKifaceStart( PGM_BASE* aProgram, int aCtlBits, KIWAY* aKiway ) override
    {
        m_settings_manager = new SETTINGS_MANAGER;
        
        // Load chemschema settings - store result to avoid unused variable warning
        m_settings_manager->GetAppSettings<CHEMSCHEMA_SETTINGS>( "chemschema" );

        return start_common( aCtlBits );
    }
    
    void OnKifaceEnd() override
    {
        if( m_settings_manager )
        {
            delete m_settings_manager;
            m_settings_manager = nullptr;
        }
        
        end_common();
    }
    
    wxWindow* CreateKiWindow( wxWindow* aParent, int aClassId, KIWAY* aKiway,
                             int aCtlBits = 0 ) override
    {
        wxASSERT( m_settings_manager );

        switch( aClassId )
        {
        case FRAME_CHEM_SCHEMA:
            {
                CHEM_FRAME* frame = new CHEM_FRAME( aKiway, aParent );

                if( frame )
                {
                    // Set up any frame-specific initialization here
                    if( IsSingle() )
                    {
                        kiChemschemaSingletonCount++;
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
        return nullptr;
    }
    
    SETTINGS_MANAGER& GetSettingsManager() const
    {
        wxASSERT( m_settings_manager );
        return *m_settings_manager;
    }
    
private:
    SETTINGS_MANAGER* m_settings_manager;

} kiface( "chemschema", KIWAY::FACE_CHEM_SCHEMA );

}

using namespace CHEMSCHEMA;

KIFACE_BASE& Kiface() { return CHEMSCHEMA::kiface; }

// KIFACE_GETTER's actual spelling is a substitution macro found in kiway.h.
// KIFACE_GETTER will not have name mangling due to declaration in kiway.h.
KIFACE_API KIFACE* KIFACE_GETTER(  int* aKIFACEversion, int aKiwayVersion, PGM_BASE* aProgram )
{
    return &kiface;
} 