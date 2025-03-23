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

#include "dialog_chem_line_properties.h"
#include "chem_edit_frame.h"
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

DIALOG_CHEM_LINE_PROPERTIES::DIALOG_CHEM_LINE_PROPERTIES( CHEM_EDIT_FRAME* aParent, CHEM_LINE* aLine ) :
    DIALOG_SHIM( aParent, wxID_ANY, _( "Line Properties" ) ),
    m_frame( aParent ),
    m_line( aLine )
{
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( mainSizer );

    // Line width
    wxBoxSizer* widthSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( widthSizer, 0, wxEXPAND | wxALL, 5 );

    wxStaticText* widthLabel = new wxStaticText( this, wxID_ANY, _( "Width:" ) );
    widthSizer->Add( widthLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );

    m_widthCtrl = new wxSpinCtrl( this, wxID_ANY );
    m_widthCtrl->SetRange( 1, 100 );
    widthSizer->Add( m_widthCtrl, 1, wxEXPAND );

    // Line style
    wxBoxSizer* styleSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( styleSizer, 0, wxEXPAND | wxALL, 5 );

    wxStaticText* styleLabel = new wxStaticText( this, wxID_ANY, _( "Style:" ) );
    styleSizer->Add( styleLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5 );

    wxString styleChoices[] = { _( "Solid" ), _( "Dashed" ), _( "Dotted" ) };
    m_styleChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                3, styleChoices );
    styleSizer->Add( m_styleChoice, 1, wxEXPAND );

    // Direction checkbox
    wxBoxSizer* directionSizer = new wxBoxSizer( wxHORIZONTAL );
    mainSizer->Add( directionSizer, 0, wxEXPAND | wxALL, 5 );

    m_directionCheck = new wxCheckBox( this, wxID_ANY, _( "Show direction" ) );
    directionSizer->Add( m_directionCheck, 1, wxEXPAND );

    // Standard dialog buttons
    wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
    buttonSizer->AddButton( new wxButton( this, wxID_OK ) );
    buttonSizer->AddButton( new wxButton( this, wxID_CANCEL ) );
    buttonSizer->Realize();
    mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, 5 );

    Fit();
    Center();
}

bool DIALOG_CHEM_LINE_PROPERTIES::TransferDataToWindow()
{
    if( !m_line )
        return false;

    m_widthCtrl->SetValue( m_line->GetWidth() );
    m_styleChoice->SetSelection( static_cast<int>( m_line->GetStyle() ) );
    m_directionCheck->SetValue( m_line->GetShowDirection() );

    return true;
}

bool DIALOG_CHEM_LINE_PROPERTIES::TransferDataFromWindow()
{
    if( !m_line )
        return false;

    m_line->SetWidth( m_widthCtrl->GetValue() );
    m_line->SetStyle( static_cast<CHEM_LINE::LINE_STYLE>( m_styleChoice->GetSelection() ) );
    m_line->SetShowDirection( m_directionCheck->GetValue() );

    return true;
} 