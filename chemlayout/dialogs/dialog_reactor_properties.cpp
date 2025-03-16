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

#include "dialog_reactor_properties.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>

DIALOG_REACTOR_PROPERTIES::DIALOG_REACTOR_PROPERTIES( wxWindow* aParent ) :
        DIALOG_SHIM( aParent, wxID_ANY, wxT( "Reactor Properties" ), wxDefaultPosition, wxDefaultSize )
{
    // Create a simple dialog with OK and Cancel buttons
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );
    
    // Add a reactor name field
    wxBoxSizer* nameSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* nameLabel = new wxStaticText( this, wxID_ANY, wxT( "Reactor Name:" ) );
    m_nameCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    nameSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    nameSizer->Add( m_nameCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( nameSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a reactor type selection
    wxBoxSizer* typeSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* typeLabel = new wxStaticText( this, wxID_ANY, wxT( "Reactor Type:" ) );
    
    wxString types[] = { wxT( "Batch" ), wxT( "CSTR" ), wxT( "PFR" ), wxT( "Fluidized Bed" ), wxT( "Fixed Bed" ) };
    m_typeCtrl = new wxComboBox( this, wxID_ANY, types[0], wxDefaultPosition, wxDefaultSize, 
                                5, types, wxCB_READONLY );
    
    typeSizer->Add( typeLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    typeSizer->Add( m_typeCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( typeSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add properties
    wxStaticBoxSizer* propSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Properties" ) );
    
    // Volume
    wxBoxSizer* volumeSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* volumeLabel = new wxStaticText( this, wxID_ANY, wxT( "Volume (m³):" ) );
    m_volumeCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "1.0" ) );
    
    volumeSizer->Add( volumeLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    volumeSizer->Add( m_volumeCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( volumeSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Temperature
    wxBoxSizer* tempSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* tempLabel = new wxStaticText( this, wxID_ANY, wxT( "Temperature (°C):" ) );
    m_temperatureCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "25.0" ) );
    
    tempSizer->Add( tempLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    tempSizer->Add( m_temperatureCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( tempSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Pressure
    wxBoxSizer* pressureSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* pressureLabel = new wxStaticText( this, wxID_ANY, wxT( "Pressure (bar):" ) );
    m_pressureCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "1.0" ) );
    
    pressureSizer->Add( pressureLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    pressureSizer->Add( m_pressureCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( pressureSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Heated checkbox
    m_heatedCtrl = new wxCheckBox( this, wxID_ANY, wxT( "Heated Reactor" ) );
    propSizer->Add( m_heatedCtrl, 0, wxEXPAND | wxALL, 5 );
    
    mainSizer->Add( propSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Connections
    wxStaticBoxSizer* connSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Connections" ) );
    
    // Inlets
    wxBoxSizer* inletSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* inletLabel = new wxStaticText( this, wxID_ANY, wxT( "Number of Inlets:" ) );
    m_inletCountCtrl = new wxSpinCtrl( this, wxID_ANY, wxT( "1" ), wxDefaultPosition, wxDefaultSize,
                                      wxSP_ARROW_KEYS, 1, 10, 1 );
    
    inletSizer->Add( inletLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    inletSizer->Add( m_inletCountCtrl, 1, wxEXPAND, 5 );
    
    connSizer->Add( inletSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Outlets
    wxBoxSizer* outletSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* outletLabel = new wxStaticText( this, wxID_ANY, wxT( "Number of Outlets:" ) );
    m_outletCountCtrl = new wxSpinCtrl( this, wxID_ANY, wxT( "1" ), wxDefaultPosition, wxDefaultSize,
                                       wxSP_ARROW_KEYS, 1, 10, 1 );
    
    outletSizer->Add( outletLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    outletSizer->Add( m_outletCountCtrl, 1, wxEXPAND, 5 );
    
    connSizer->Add( outletSizer, 0, wxEXPAND | wxALL, 5 );
    
    mainSizer->Add( connSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add buttons
    wxStdDialogButtonSizer* buttonSizer = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Set minimum size
    SetMinSize( wxSize( 400, 450 ) );
    
    // Fit to content
    Fit();
    
    // Center on parent
    CenterOnParent();
}


DIALOG_REACTOR_PROPERTIES::~DIALOG_REACTOR_PROPERTIES()
{
}


void DIALOG_REACTOR_PROPERTIES::SetReactorName( const wxString& aName )
{
    m_nameCtrl->SetValue( aName );
}


wxString DIALOG_REACTOR_PROPERTIES::GetReactorName() const
{
    return m_nameCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetReactorType( const wxString& aType )
{
    m_typeCtrl->SetValue( aType );
}


wxString DIALOG_REACTOR_PROPERTIES::GetReactorType() const
{
    return m_typeCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetVolume( const wxString& aVolume )
{
    m_volumeCtrl->SetValue( aVolume );
}


wxString DIALOG_REACTOR_PROPERTIES::GetVolume() const
{
    return m_volumeCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetTemperature( const wxString& aTemperature )
{
    m_temperatureCtrl->SetValue( aTemperature );
}


wxString DIALOG_REACTOR_PROPERTIES::GetTemperature() const
{
    return m_temperatureCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetPressure( const wxString& aPressure )
{
    m_pressureCtrl->SetValue( aPressure );
}


wxString DIALOG_REACTOR_PROPERTIES::GetPressure() const
{
    return m_pressureCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetHeated( bool aHeated )
{
    m_heatedCtrl->SetValue( aHeated );
}


bool DIALOG_REACTOR_PROPERTIES::IsHeated() const
{
    return m_heatedCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetInletCount( int aCount )
{
    m_inletCountCtrl->SetValue( aCount );
}


int DIALOG_REACTOR_PROPERTIES::GetInletCount() const
{
    return m_inletCountCtrl->GetValue();
}


void DIALOG_REACTOR_PROPERTIES::SetOutletCount( int aCount )
{
    m_outletCountCtrl->SetValue( aCount );
}


int DIALOG_REACTOR_PROPERTIES::GetOutletCount() const
{
    return m_outletCountCtrl->GetValue();
} 