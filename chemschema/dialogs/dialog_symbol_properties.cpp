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

#include "dialog_symbol_properties.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>

DIALOG_SYMBOL_PROPERTIES::DIALOG_SYMBOL_PROPERTIES( wxWindow* aParent, const wxString& aTitle ) :
        DIALOG_SHIM( aParent, wxID_ANY, aTitle, wxDefaultPosition, wxDefaultSize )
{
    // Create a simple dialog with OK and Cancel buttons
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );
    
    // Add a name field
    wxBoxSizer* nameSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* nameLabel = new wxStaticText( this, wxID_ANY, wxT( "Symbol Name:" ) );
    m_nameCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    nameSizer->Add( nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    nameSizer->Add( m_nameCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( nameSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add a description field
    wxBoxSizer* descSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* descLabel = new wxStaticText( this, wxID_ANY, wxT( "Description:" ) );
    m_descCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    
    descSizer->Add( descLabel, 0, wxALIGN_TOP | wxRIGHT, 5 );
    descSizer->Add( m_descCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( descSizer, 1, wxEXPAND | wxALL, 5 );
    
    // Add buttons
    wxStdDialogButtonSizer* buttonSizer = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Set minimum size
    SetMinSize( wxSize( 300, 200 ) );
    
    // Fit to content
    Fit();
    
    // Center on parent
    CenterOnParent();
}


DIALOG_SYMBOL_PROPERTIES::~DIALOG_SYMBOL_PROPERTIES()
{
}


void DIALOG_SYMBOL_PROPERTIES::SetSymbolName( const wxString& aName )
{
    m_nameCtrl->SetValue( aName );
}


wxString DIALOG_SYMBOL_PROPERTIES::GetSymbolName() const
{
    return m_nameCtrl->GetValue();
}


void DIALOG_SYMBOL_PROPERTIES::SetDescription( const wxString& aDescription )
{
    m_descCtrl->SetValue( aDescription );
}


wxString DIALOG_SYMBOL_PROPERTIES::GetDescription() const
{
    return m_descCtrl->GetValue();
} 