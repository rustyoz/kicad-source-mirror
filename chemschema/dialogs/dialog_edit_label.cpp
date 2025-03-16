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

#include "dialog_edit_label.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/statline.h>

DIALOG_EDIT_LABEL::DIALOG_EDIT_LABEL( wxWindow* aParent ) :
        DIALOG_SHIM( aParent, wxID_ANY, wxT( "Edit Label" ), wxDefaultPosition, wxDefaultSize )
{
    // Create a simple dialog with OK and Cancel buttons
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );
    
    // Add a label text field
    wxBoxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* textLabel = new wxStaticText( this, wxID_ANY, wxT( "Label Text:" ) );
    m_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString );
    
    textSizer->Add( textLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    textSizer->Add( m_textCtrl, 1, wxEXPAND, 5 );
    
    mainSizer->Add( textSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Horizontal line
    mainSizer->Add( new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ),
                   0, wxEXPAND | wxALL, 5 );
    
    // Text properties
    wxStaticBoxSizer* propSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Text Properties" ) );
    
    // Font size
    wxBoxSizer* sizeSizer = new wxBoxSizer( wxHORIZONTAL );
    wxStaticText* sizeLabel = new wxStaticText( this, wxID_ANY, wxT( "Size:" ) );
    
    wxString sizes[] = { wxT( "Small" ), wxT( "Medium" ), wxT( "Large" ) };
    m_sizeCtrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, sizes );
    m_sizeCtrl->SetSelection( 1 ); // Default to Medium
    
    sizeSizer->Add( sizeLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );
    sizeSizer->Add( m_sizeCtrl, 1, wxEXPAND, 5 );
    
    propSizer->Add( sizeSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Bold checkbox
    m_boldCtrl = new wxCheckBox( this, wxID_ANY, wxT( "Bold" ) );
    propSizer->Add( m_boldCtrl, 0, wxEXPAND | wxALL, 5 );
    
    // Italic checkbox
    m_italicCtrl = new wxCheckBox( this, wxID_ANY, wxT( "Italic" ) );
    propSizer->Add( m_italicCtrl, 0, wxEXPAND | wxALL, 5 );
    
    mainSizer->Add( propSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Add buttons
    wxStdDialogButtonSizer* buttonSizer = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );
    
    // Set minimum size
    SetMinSize( wxSize( 300, 250 ) );
    
    // Fit to content
    Fit();
    
    // Center on parent
    CenterOnParent();
}


DIALOG_EDIT_LABEL::~DIALOG_EDIT_LABEL()
{
}


void DIALOG_EDIT_LABEL::SetLabelText( const wxString& aText )
{
    m_textCtrl->SetValue( aText );
}


wxString DIALOG_EDIT_LABEL::GetLabelText() const
{
    return m_textCtrl->GetValue();
}


void DIALOG_EDIT_LABEL::SetSize( int aSize )
{
    m_sizeCtrl->SetSelection( aSize );
}


int DIALOG_EDIT_LABEL::GetSize() const
{
    return m_sizeCtrl->GetSelection();
}


void DIALOG_EDIT_LABEL::SetBold( bool aBold )
{
    m_boldCtrl->SetValue( aBold );
}


bool DIALOG_EDIT_LABEL::IsBold() const
{
    return m_boldCtrl->GetValue();
}


void DIALOG_EDIT_LABEL::SetItalic( bool aItalic )
{
    m_italicCtrl->SetValue( aItalic );
}


bool DIALOG_EDIT_LABEL::IsItalic() const
{
    return m_italicCtrl->GetValue();
} 