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

#include "dialog_chem_properties.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>

DIALOG_CHEM_PROPERTIES::DIALOG_CHEM_PROPERTIES( wxWindow* aParent ) :
        DIALOG_SHIM( aParent, wxID_ANY, wxT( "Chemical Properties" ), wxDefaultPosition, wxDefaultSize )
{
    // Create a simple dialog with OK and Cancel buttons
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );
    
    // Add a chemical name field
    wxBoxSizer* nameSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* nameLabel = new wxStaticText( this, wxID_ANY, wxT( "Chemical Name:" ) );
    m_nameCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    nameSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    nameSizer->Add( m_nameCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( nameSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a formula field
    wxBoxSizer* formulaSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* formulaLabel = new wxStaticText( this, wxID_ANY, wxT( "Chemical Formula:" ) );
    m_formulaCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    formulaSizer->Add( formulaLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    formulaSizer->Add( m_formulaCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( formulaSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a state selection
    wxBoxSizer* stateSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* stateLabel = new wxStaticText( this, wxID_ANY, wxT( "Physical State:" ) );
    
    wxString states[] = { wxT( "Solid" ), wxT( "Liquid" ), wxT( "Gas" ), wxT( "Aqueous" ), wxT( "Slurry" ) };
    m_stateCtrl = new wxComboBox( this, wxID_ANY, states[0], wxDefaultPosition, wxDefaultSize, 
                                 5, states, wxCB_READONLY );
    
    stateSizer->Add( stateLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    stateSizer->Add( m_stateCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( stateSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a hazardous checkbox
    wxBoxSizer* hazardSizer = new wxBoxSizer( wxHORIZONTAL );
    m_hazardousCtrl = new wxCheckBox( this, wxID_ANY, wxT( "Hazardous Material" ) );
    
    hazardSizer->Add( m_hazardousCtrl, 0, wxALIGN_CENTER_VERTICAL, 5 );
    
    mainSizer->Add( hazardSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add properties
    wxStaticBoxSizer* propSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Properties" ) );
    
    // Density
    wxBoxSizer* densitySizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* densityLabel = new wxStaticText( this, wxID_ANY, wxT( "Density (kg/m³):" ) );
    m_densityCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "1000" ) );
    
    densitySizer->Add( densityLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    densitySizer->Add( m_densityCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( densitySizer, 0, wxEXPAND | wxALL, 5 );
    
    // Viscosity
    wxBoxSizer* viscositySizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* viscosityLabel = new wxStaticText( this, wxID_ANY, wxT( "Viscosity (cP):" ) );
    m_viscosityCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "1.0" ) );
    
    viscositySizer->Add( viscosityLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    viscositySizer->Add( m_viscosityCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( viscositySizer, 0, wxEXPAND | wxALL, 5 );
    
    mainSizer->Add( propSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add buttons
    wxStdDialogButtonSizer* buttonSizer = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Set minimum size
    SetMinSize( wxSize( 400, 300 ) );
    
    // Fit to content
    Fit();
    
    // Center on parent
    CenterOnParent();
}


DIALOG_CHEM_PROPERTIES::~DIALOG_CHEM_PROPERTIES()
{
}


void DIALOG_CHEM_PROPERTIES::SetChemicalName( const wxString& aName )
{
    m_nameCtrl->SetValue( aName );
}


wxString DIALOG_CHEM_PROPERTIES::GetChemicalName() const
{
    return m_nameCtrl->GetValue();
}


void DIALOG_CHEM_PROPERTIES::SetFormula( const wxString& aFormula )
{
    m_formulaCtrl->SetValue( aFormula );
}


wxString DIALOG_CHEM_PROPERTIES::GetFormula() const
{
    return m_formulaCtrl->GetValue();
}


void DIALOG_CHEM_PROPERTIES::SetState( const wxString& aState )
{
    m_stateCtrl->SetValue( aState );
}


wxString DIALOG_CHEM_PROPERTIES::GetState() const
{
    return m_stateCtrl->GetValue();
}


void DIALOG_CHEM_PROPERTIES::SetHazardous( bool aHazardous )
{
    m_hazardousCtrl->SetValue( aHazardous );
}


bool DIALOG_CHEM_PROPERTIES::IsHazardous() const
{
    return m_hazardousCtrl->GetValue();
}


void DIALOG_CHEM_PROPERTIES::SetDensity( const wxString& aDensity )
{
    m_densityCtrl->SetValue( aDensity );
}


wxString DIALOG_CHEM_PROPERTIES::GetDensity() const
{
    return m_densityCtrl->GetValue();
}


void DIALOG_CHEM_PROPERTIES::SetViscosity( const wxString& aViscosity )
{
    m_viscosityCtrl->SetValue( aViscosity );
}


wxString DIALOG_CHEM_PROPERTIES::GetViscosity() const
{
    return m_viscosityCtrl->GetValue();
} 