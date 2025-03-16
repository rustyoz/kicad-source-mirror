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

#include "dialog_pipe_properties.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statbox.h>

DIALOG_PIPE_PROPERTIES::DIALOG_PIPE_PROPERTIES( wxWindow* aParent ) :
        DIALOG_SHIM( aParent, wxID_ANY, wxT( "Pipe Properties" ), wxDefaultPosition, wxDefaultSize )
{
    // Create a simple dialog with OK and Cancel buttons
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );
    
    // Add a pipe name field
    wxBoxSizer* nameSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* nameLabel = new wxStaticText( this, wxID_ANY, wxT( "Pipe Name:" ) );
    m_nameCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    nameSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    nameSizer->Add( m_nameCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( nameSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a material selection
    wxBoxSizer* materialSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* materialLabel = new wxStaticText( this, wxID_ANY, wxT( "Material:" ) );
    
    wxString materials[] = { wxT( "Carbon Steel" ), wxT( "Stainless Steel" ), wxT( "PVC" ), wxT( "HDPE" ), wxT( "Glass" ) };
    m_materialCtrl = new wxComboBox( this, wxID_ANY, materials[0], wxDefaultPosition, wxDefaultSize, 
                                    5, materials, wxCB_READONLY );
    
    materialSizer->Add( materialLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    materialSizer->Add( m_materialCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( materialSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add properties
    wxStaticBoxSizer* propSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Properties" ) );
    
    // Diameter
    wxBoxSizer* diameterSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* diameterLabel = new wxStaticText( this, wxID_ANY, wxT( "Diameter (mm):" ) );
    m_diameterCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "50.0" ) );
    
    diameterSizer->Add( diameterLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    diameterSizer->Add( m_diameterCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( diameterSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Flow Rate
    wxBoxSizer* flowSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* flowLabel = new wxStaticText( this, wxID_ANY, wxT( "Flow Rate (m³/h):" ) );
    m_flowRateCtrl = new wxTextCtrl( this, wxID_ANY, wxT( "10.0" ) );
    
    flowSizer->Add( flowLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    flowSizer->Add( m_flowRateCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( flowSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Insulated checkbox
    m_insulatedCtrl = new wxCheckBox( this, wxID_ANY, wxT( "Insulated Pipe" ) );
    propSizer->Add( m_insulatedCtrl, 0, wxEXPAND | wxALL, 5 );
    
    mainSizer->Add( propSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Line Style
    wxString lineStyles[] = { wxT( "Solid" ), wxT( "Dashed" ), wxT( "Dotted" ) };
    m_lineStyleCtrl = new wxRadioBox( this, wxID_ANY, wxT( "Line Style" ), wxDefaultPosition, wxDefaultSize,
                                     3, lineStyles, 1, wxRA_SPECIFY_COLS );
    
    mainSizer->Add( m_lineStyleCtrl, 0, wxEXPAND | wxALL, 5 );
    
    // Add buttons
    wxStdDialogButtonSizer* buttonSizer = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Set minimum size
    SetMinSize( wxSize( 350, 350 ) );
    
    // Fit to content
    Fit();
    
    // Center on parent
    CenterOnParent();
}


DIALOG_PIPE_PROPERTIES::~DIALOG_PIPE_PROPERTIES()
{
}


void DIALOG_PIPE_PROPERTIES::SetPipeName( const wxString& aName )
{
    m_nameCtrl->SetValue( aName );
}


wxString DIALOG_PIPE_PROPERTIES::GetPipeName() const
{
    return m_nameCtrl->GetValue();
}


void DIALOG_PIPE_PROPERTIES::SetMaterial( const wxString& aMaterial )
{
    m_materialCtrl->SetValue( aMaterial );
}


wxString DIALOG_PIPE_PROPERTIES::GetMaterial() const
{
    return m_materialCtrl->GetValue();
}


void DIALOG_PIPE_PROPERTIES::SetDiameter( const wxString& aDiameter )
{
    m_diameterCtrl->SetValue( aDiameter );
}


wxString DIALOG_PIPE_PROPERTIES::GetDiameter() const
{
    return m_diameterCtrl->GetValue();
}


void DIALOG_PIPE_PROPERTIES::SetFlowRate( const wxString& aFlowRate )
{
    m_flowRateCtrl->SetValue( aFlowRate );
}


wxString DIALOG_PIPE_PROPERTIES::GetFlowRate() const
{
    return m_flowRateCtrl->GetValue();
}


void DIALOG_PIPE_PROPERTIES::SetInsulated( bool aInsulated )
{
    m_insulatedCtrl->SetValue( aInsulated );
}


bool DIALOG_PIPE_PROPERTIES::IsInsulated() const
{
    return m_insulatedCtrl->GetValue();
}


void DIALOG_PIPE_PROPERTIES::SetLineStyle( int aStyle )
{
    m_lineStyleCtrl->SetSelection( aStyle );
}


int DIALOG_PIPE_PROPERTIES::GetLineStyle() const
{
    return m_lineStyleCtrl->GetSelection();
} 