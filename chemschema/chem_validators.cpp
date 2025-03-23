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

#include "chem_validators.h"
#include <wx/msgdlg.h>
#include <regex>

CHEM_FORMULA_VALIDATOR::CHEM_FORMULA_VALIDATOR( wxString* aValue ) :
    wxTextValidator( wxFILTER_NONE, aValue )
{
}

wxObject* CHEM_FORMULA_VALIDATOR::Clone() const
{
    return new CHEM_FORMULA_VALIDATOR( *this );
}

bool CHEM_FORMULA_VALIDATOR::Validate( wxWindow* aParent )
{
    wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( GetWindow() );
    wxString value = ctrl->GetValue();

    if( !ValidateFormula( value ) )
    {
        wxMessageBox( _( "Invalid chemical formula" ), _( "Validation Error" ),
                     wxOK | wxICON_ERROR, aParent );
        return false;
    }

    return true;
}

bool CHEM_FORMULA_VALIDATOR::ValidateFormula( const wxString& aFormula ) const
{
    // Basic chemical formula validation using regex
    // Matches patterns like: H2O, NaCl, Fe2(SO4)3, etc.
    std::regex formulaPattern( "^[A-Z][a-z]?\\d*(?:[A-Z][a-z]?\\d*)*(?:\\([A-Za-z0-9]+\\)\\d*)*$" );
    return std::regex_match( aFormula.ToStdString(), formulaPattern );
}

CHEM_NAME_VALIDATOR::CHEM_NAME_VALIDATOR( wxString* aValue ) :
    wxTextValidator( wxFILTER_NONE, aValue )
{
}

wxObject* CHEM_NAME_VALIDATOR::Clone() const
{
    return new CHEM_NAME_VALIDATOR( *this );
}

bool CHEM_NAME_VALIDATOR::Validate( wxWindow* aParent )
{
    wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( GetWindow() );
    wxString value = ctrl->GetValue();

    if( !ValidateName( value ) )
    {
        wxMessageBox( _( "Invalid chemical name" ), _( "Validation Error" ),
                     wxOK | wxICON_ERROR, aParent );
        return false;
    }

    return true;
}

bool CHEM_NAME_VALIDATOR::ValidateName( const wxString& aName ) const
{
    // Basic chemical name validation
    // Must be at least 2 characters long and contain only letters, numbers, spaces, and hyphens
    std::regex namePattern( "^[A-Za-z0-9\\- ]{2,}$" );
    return std::regex_match( aName.ToStdString(), namePattern );
}

CHEM_UNIT_VALIDATOR::CHEM_UNIT_VALIDATOR( wxString* aValue ) :
    wxTextValidator( wxFILTER_NONE, aValue )
{
}

wxObject* CHEM_UNIT_VALIDATOR::Clone() const
{
    return new CHEM_UNIT_VALIDATOR( *this );
}

bool CHEM_UNIT_VALIDATOR::Validate( wxWindow* aParent )
{
    wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( GetWindow() );
    wxString value = ctrl->GetValue();

    if( !ValidateValue( value ) )
    {
        wxMessageBox( _( "Invalid value with units" ), _( "Validation Error" ),
                     wxOK | wxICON_ERROR, aParent );
        return false;
    }

    return true;
}

bool CHEM_UNIT_VALIDATOR::ValidateValue( const wxString& aValue ) const
{
    // Matches patterns like: 123.45 g/mol, 1.23e-4 mol/L, etc.
    std::regex valuePattern( "^\\d*\\.?\\d+(?:[eE][-+]?\\d+)?\\s*[a-zA-Z°/]+$" );
    return std::regex_match( aValue.ToStdString(), valuePattern );
} 