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

#include "dialog_chem_text_properties.h"
#include "chem_edit_frame.h"
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

DIALOG_CHEM_TEXT_PROPERTIES::DIALOG_CHEM_TEXT_PROPERTIES( CHEM_EDIT_FRAME* aParent, CHEM_TEXT* aText ) :
    DIALOG_SHIM( aParent, wxID_ANY, _( "Text Properties" ) ),
    m_frame( aParent ),
    m_text( aText )
{
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );

    // Text content
    wxBoxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( textSizer, 0, wxEXPAND | wxALL, 5 );

    wxStaticText* textLabel = new wxStaticText( this, wxID_ANY, _( "Text:" ) );
    textSizer->Add( textLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );

    m_textCtrl = new wxTextCtrl( this, wxID_ANY );
    textSizer->Add( m_textCtrl, 1, wxEXPAND );

    // Text alignment
    wxBoxSizer* alignSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( alignSizer, 0, wxEXPAND | wxALL, 5 );

    wxStaticText* alignLabel = new wxStaticText( this, wxID_ANY, _( "Alignment:" ) );
    alignSizer->Add( alignLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );

    wxString alignChoices[] = { _( "Left" ), _( "Center" ), _( "Right" ) };
    m_alignmentChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                    3, alignChoices );
    alignSizer->Add( m_alignmentChoice, 1, wxEXPAND );

    // Text size
    wxBoxSizer* sizeSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( sizeSizer, 0, wxEXPAND | wxALL, 5 );

    wxStaticText* sizeLabel = new wxStaticText( this, wxID_ANY, _( "Size:" ) );
    sizeSizer->Add( sizeLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );

    m_sizeCtrl = new wxSpinCtrl( this, wxID_ANY );
    m_sizeCtrl->SetRange( 1, 100 );
    sizeSizer->Add( m_sizeCtrl, 1, wxEXPAND );

    // Standard dialog buttons
    wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
    buttonSizer->AddButton( new wxButton( this, wxID_OK ) );
    buttonSizer->AddButton( new wxButton( this, wxID_CANCEL ) );
    buttonSizer->Realize();
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );

    Fit();
    Center();
}

bool DIALOG_CHEM_TEXT_PROPERTIES::TransferDataToWindow()
{
    if( !m_text )
        return false;

    m_textCtrl->SetValue( m_text->GetText() );
    m_alignmentChoice->SetSelection( static_cast<int>( m_text->GetAlignment() ) );
    m_sizeCtrl->SetValue( m_text->GetTextSize() );

    return true;
}

bool DIALOG_CHEM_TEXT_PROPERTIES::TransferDataFromWindow()
{
    if( !m_text )
        return false;

    m_text->SetText( m_textCtrl->GetValue() );
    m_text->SetAlignment( static_cast<CHEM_TEXT::ALIGNMENT>( m_alignmentChoice->GetSelection() ) );
    m_text->SetTextSize( m_sizeCtrl->GetValue() );

    return true;
} 