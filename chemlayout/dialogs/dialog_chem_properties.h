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

#ifndef DIALOG_CHEM_PROPERTIES_H
#define DIALOG_CHEM_PROPERTIES_H

#include <dialog_shim.h>

class wxTextCtrl;
class wxComboBox;
class wxCheckBox;

/**
 * Dialog for editing chemical properties in the chemical reactor layout.
 */
class DIALOG_CHEM_PROPERTIES : public DIALOG_SHIM
{
public:
    /**
     * Constructor
     * @param aParent - Parent window
     */
    DIALOG_CHEM_PROPERTIES( wxWindow* aParent );
    
    /**
     * Destructor
     */
    ~DIALOG_CHEM_PROPERTIES();
    
    /**
     * Set the chemical name
     * @param aName - The chemical name
     */
    void SetChemicalName( const wxString& aName );
    
    /**
     * Get the chemical name
     * @return The chemical name
     */
    wxString GetChemicalName() const;
    
    /**
     * Set the chemical formula
     * @param aFormula - The chemical formula
     */
    void SetFormula( const wxString& aFormula );
    
    /**
     * Get the chemical formula
     * @return The chemical formula
     */
    wxString GetFormula() const;
    
    /**
     * Set the physical state
     * @param aState - The physical state (Solid, Liquid, Gas, etc.)
     */
    void SetState( const wxString& aState );
    
    /**
     * Get the physical state
     * @return The physical state
     */
    wxString GetState() const;
    
    /**
     * Set whether the chemical is hazardous
     * @param aHazardous - True if hazardous, false otherwise
     */
    void SetHazardous( bool aHazardous );
    
    /**
     * Check if the chemical is hazardous
     * @return True if hazardous, false otherwise
     */
    bool IsHazardous() const;
    
    /**
     * Set the density value
     * @param aDensity - The density value as a string
     */
    void SetDensity( const wxString& aDensity );
    
    /**
     * Get the density value
     * @return The density value as a string
     */
    wxString GetDensity() const;
    
    /**
     * Set the viscosity value
     * @param aViscosity - The viscosity value as a string
     */
    void SetViscosity( const wxString& aViscosity );
    
    /**
     * Get the viscosity value
     * @return The viscosity value as a string
     */
    wxString GetViscosity() const;
    
private:
    wxTextCtrl* m_nameCtrl;
    wxTextCtrl* m_formulaCtrl;
    wxComboBox* m_stateCtrl;
    wxCheckBox* m_hazardousCtrl;
    wxTextCtrl* m_densityCtrl;
    wxTextCtrl* m_viscosityCtrl;
};

#endif // DIALOG_CHEM_PROPERTIES_H 