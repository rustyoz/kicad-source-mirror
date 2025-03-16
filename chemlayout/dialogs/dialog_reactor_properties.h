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

#ifndef DIALOG_REACTOR_PROPERTIES_H
#define DIALOG_REACTOR_PROPERTIES_H

#include <wx/string.h>
#include <dialog_shim.h>

class wxTextCtrl;
class wxComboBox;
class wxCheckBox;
class wxSpinCtrl;

/**
 * Dialog for editing chemical reactor properties in the layout.
 */
class DIALOG_REACTOR_PROPERTIES : public DIALOG_SHIM
{
public:
    /**
     * Constructor
     *
     * @param aParent The parent window
     */
    DIALOG_REACTOR_PROPERTIES( wxWindow* aParent );

    /**
     * Destructor
     */
    ~DIALOG_REACTOR_PROPERTIES();

    /**
     * Set the reactor name
     *
     * @param aName The name to set
     */
    void SetReactorName( const wxString& aName );

    /**
     * Get the reactor name
     *
     * @return The reactor name
     */
    wxString GetReactorName() const;

    /**
     * Set the reactor type
     *
     * @param aType The type to set
     */
    void SetReactorType( const wxString& aType );

    /**
     * Get the reactor type
     *
     * @return The reactor type
     */
    wxString GetReactorType() const;

    /**
     * Set the reactor volume
     *
     * @param aVolume The volume value as a string
     */
    void SetVolume( const wxString& aVolume );

    /**
     * Get the reactor volume
     *
     * @return The volume value as a string
     */
    wxString GetVolume() const;

    /**
     * Set the reactor temperature
     *
     * @param aTemperature The temperature value as a string
     */
    void SetTemperature( const wxString& aTemperature );

    /**
     * Get the reactor temperature
     *
     * @return The temperature value as a string
     */
    wxString GetTemperature() const;

    /**
     * Set the reactor pressure
     *
     * @param aPressure The pressure value as a string
     */
    void SetPressure( const wxString& aPressure );

    /**
     * Get the reactor pressure
     *
     * @return The pressure value as a string
     */
    wxString GetPressure() const;

    /**
     * Set whether the reactor is heated
     *
     * @param aHeated True if heated
     */
    void SetHeated( bool aHeated );

    /**
     * Check if the reactor is heated
     *
     * @return True if heated
     */
    bool IsHeated() const;

    /**
     * Set the number of inlets
     *
     * @param aCount The number of inlets
     */
    void SetInletCount( int aCount );

    /**
     * Get the number of inlets
     *
     * @return The number of inlets
     */
    int GetInletCount() const;

    /**
     * Set the number of outlets
     *
     * @param aCount The number of outlets
     */
    void SetOutletCount( int aCount );

    /**
     * Get the number of outlets
     *
     * @return The number of outlets
     */
    int GetOutletCount() const;

private:
    wxTextCtrl* m_nameCtrl;
    wxComboBox* m_typeCtrl;
    wxTextCtrl* m_volumeCtrl;
    wxTextCtrl* m_temperatureCtrl;
    wxTextCtrl* m_pressureCtrl;
    wxCheckBox* m_heatedCtrl;
    wxSpinCtrl* m_inletCountCtrl;
    wxSpinCtrl* m_outletCountCtrl;
};

#endif // DIALOG_REACTOR_PROPERTIES_H 