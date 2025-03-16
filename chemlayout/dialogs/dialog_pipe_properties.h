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

#ifndef DIALOG_PIPE_PROPERTIES_H
#define DIALOG_PIPE_PROPERTIES_H

#include <wx/string.h>
#include <dialog_shim.h>

class wxTextCtrl;
class wxComboBox;
class wxCheckBox;
class wxRadioBox;

/**
 * Dialog for editing pipe properties in the chemical reactor layout.
 */
class DIALOG_PIPE_PROPERTIES : public DIALOG_SHIM
{
public:
    /**
     * Constructor
     *
     * @param aParent The parent window
     */
    DIALOG_PIPE_PROPERTIES( wxWindow* aParent );

    /**
     * Destructor
     */
    ~DIALOG_PIPE_PROPERTIES();

    /**
     * Set the pipe name
     *
     * @param aName The name to set
     */
    void SetPipeName( const wxString& aName );

    /**
     * Get the pipe name
     *
     * @return The pipe name
     */
    wxString GetPipeName() const;

    /**
     * Set the pipe material
     *
     * @param aMaterial The material to set
     */
    void SetMaterial( const wxString& aMaterial );

    /**
     * Get the pipe material
     *
     * @return The pipe material
     */
    wxString GetMaterial() const;

    /**
     * Set the pipe diameter
     *
     * @param aDiameter The diameter value as a string
     */
    void SetDiameter( const wxString& aDiameter );

    /**
     * Get the pipe diameter
     *
     * @return The diameter value as a string
     */
    wxString GetDiameter() const;

    /**
     * Set the pipe flow rate
     *
     * @param aFlowRate The flow rate value as a string
     */
    void SetFlowRate( const wxString& aFlowRate );

    /**
     * Get the pipe flow rate
     *
     * @return The flow rate value as a string
     */
    wxString GetFlowRate() const;

    /**
     * Set whether the pipe is insulated
     *
     * @param aInsulated True if insulated
     */
    void SetInsulated( bool aInsulated );

    /**
     * Check if the pipe is insulated
     *
     * @return True if insulated
     */
    bool IsInsulated() const;

    /**
     * Set the pipe line style
     *
     * @param aStyle The style index (0=Solid, 1=Dashed, 2=Dotted)
     */
    void SetLineStyle( int aStyle );

    /**
     * Get the pipe line style
     *
     * @return The style index (0=Solid, 1=Dashed, 2=Dotted)
     */
    int GetLineStyle() const;

private:
    wxTextCtrl* m_nameCtrl;
    wxComboBox* m_materialCtrl;
    wxTextCtrl* m_diameterCtrl;
    wxTextCtrl* m_flowRateCtrl;
    wxCheckBox* m_insulatedCtrl;
    wxRadioBox* m_lineStyleCtrl;
};

#endif // DIALOG_PIPE_PROPERTIES_H 