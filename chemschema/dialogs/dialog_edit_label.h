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

#ifndef DIALOG_EDIT_LABEL_H
#define DIALOG_EDIT_LABEL_H

#include <wx/string.h>
#include <dialog_shim.h>

class wxTextCtrl;
class wxChoice;
class wxCheckBox;

/**
 * Dialog for editing labels in the chemical process flow diagram.
 */
class DIALOG_EDIT_LABEL : public DIALOG_SHIM
{
public:
    /**
     * Constructor
     *
     * @param aParent The parent window
     */
    DIALOG_EDIT_LABEL( wxWindow* aParent );

    /**
     * Destructor
     */
    ~DIALOG_EDIT_LABEL();

    /**
     * Set the label text
     *
     * @param aText The text to set
     */
    void SetLabelText( const wxString& aText );

    /**
     * Get the label text
     *
     * @return The label text
     */
    wxString GetLabelText() const;

    /**
     * Set the text size
     *
     * @param aSize The size index (0=Small, 1=Medium, 2=Large)
     */
    void SetSize( int aSize );

    /**
     * Get the text size
     *
     * @return The size index (0=Small, 1=Medium, 2=Large)
     */
    int GetSize() const;

    /**
     * Set whether the text is bold
     *
     * @param aBold True for bold text
     */
    void SetBold( bool aBold );

    /**
     * Check if the text is bold
     *
     * @return True if bold
     */
    bool IsBold() const;

    /**
     * Set whether the text is italic
     *
     * @param aItalic True for italic text
     */
    void SetItalic( bool aItalic );

    /**
     * Check if the text is italic
     *
     * @return True if italic
     */
    bool IsItalic() const;

private:
    wxTextCtrl* m_textCtrl;
    wxChoice* m_sizeCtrl;
    wxCheckBox* m_boldCtrl;
    wxCheckBox* m_italicCtrl;
};

#endif // DIALOG_EDIT_LABEL_H 