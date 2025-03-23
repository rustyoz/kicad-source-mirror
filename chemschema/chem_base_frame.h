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

#ifndef CHEM_BASE_FRAME_H
#define CHEM_BASE_FRAME_H

#include <eda_draw_frame.h>
#include <core/typeinfo.h>
#include <settings/app_settings.h>
#include <chemschema_settings.h>

class CHEM_DRAW_PANEL;
namespace KIGFX { class CHEM_VIEW; }
class CHEM_SCREEN;


/**
 * A shim class between EDA_DRAW_FRAME and several derived classes:
 * CHEM_EDIT_FRAME, CHEM_VIEWER_FRAME, etc. It provides a common way of handling
 * the provided virtual functions for the derived classes.
 */
class CHEM_BASE_FRAME : public EDA_DRAW_FRAME
{
public:
    CHEM_BASE_FRAME( KIWAY* aKiway, wxWindow* aParent, FRAME_T aWindowType,
                     const wxString& aTitle, const wxPoint& aPosition = wxDefaultPosition,
                     const wxSize& aSize = wxDefaultSize,
                     long aStyle = KICAD_DEFAULT_DRAWFRAME_STYLE,
                     const wxString& aFrameName = wxEmptyString );

    ~CHEM_BASE_FRAME() override;

    void createCanvas();

    CHEM_SCREEN* GetScreen() const ;
    virtual KIGFX::CHEM_VIEW* GetChemView() const = 0;

    CHEMSCHEMA_SETTINGS* chemconfig() const;

    void LoadSettings( APP_SETTINGS_BASE* aCfg ) override;
    void SaveSettings( APP_SETTINGS_BASE* aCfg ) override;

    COLOR4D GetDrawBgColor() const override;

    void SetPageSettings( const PAGE_INFO& aPageSettings ) override;
    const PAGE_INFO& GetPageSettings() const override;
    const VECTOR2I GetPageSizeIU() const override;

    const VECTOR2I& GetGridOrigin() const override
    {
        static VECTOR2I zero;
        return zero;
    }
    void SetGridOrigin( const VECTOR2I& aPoint ) override {}

protected:
    DECLARE_EVENT_TABLE()
};

#endif // CHEM_BASE_FRAME_H 