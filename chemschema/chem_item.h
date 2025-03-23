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

#ifndef CHEM_ITEM_H
#define CHEM_ITEM_H

#include <eda_item.h>
#include <chem_schematic.h>
// Forward declare CHEM_SCREEN instead of including
class CHEM_SCREEN;

/**
 * Base class for all chemical diagram items.
 */
class CHEM_ITEM : public EDA_ITEM
{
public:
    // Item types for chemical schematic elements
    enum CHEM_ITEM_TYPE
    {
        CHEM_LINE_T,
        CHEM_SYMBOL_T,
        CHEM_JUNCTION_T,
        CHEM_LABEL_T,
        CHEM_SCREEN_T,
        CHEM_SHEET_T,
        CHEM_TEXT_T,
        CHEM_SHAPE_T
    };

    CHEM_ITEM( EDA_ITEM* aParent = nullptr, KICAD_T aItemType = CHEM_ITEM_T );
    virtual ~CHEM_ITEM();

    virtual wxString GetClass() const override;

    /**
     * Get the list of layers this item is drawn on
     * @return Vector of layer indices
     */
    virtual std::vector<int> ViewGetLayers() const override;

    /**
     * Get the text to display in a selection menu
     * @param aUnits - Units for display
     * @return String for menu
     */
    virtual wxString GetSelectMenuText( EDA_UNITS aUnits ) const;

    /**
     * Check if object is movable from the anchor point.
     * @return true for items which are moved with the anchor point at mouse cursor.
     * @return false for items moved with no reference to anchor.
     */
    virtual bool IsMovableFromAnchorPoint() const;

    /**
     * Get the parent schematic this item lives on.
     * @return the parent schematic this item lives on, or nullptr.
     */
    CHEM_SCHEMATIC* Schematic() const;

    /**
     * Get the parent screen this item lives on.
     * @return the parent screen this item lives on, or nullptr.
     */
    CHEM_SCREEN* Screen() const;

    /**
     * Check if the item is locked.
     * @return true if the item is locked, false otherwise.
     */
    virtual bool IsLocked() const;

    /**
     * Set the locked state of the item.
     * @param aLocked - true to lock the item, false to unlock.
     */
    virtual void SetLocked( bool aLocked );

    /**
     * Allow items to support hypertext actions when hovered/clicked.
     * @return true if the item supports hypertext actions.
     */
    virtual bool IsHypertext() const;

    /**
     * Execute hypertext action when clicked.
     * @param aFrame - The frame containing the item.
     */
    virtual void DoHypertextAction( EDA_DRAW_FRAME* aFrame ) const;

    /**
     * Return the layer this item is on.
     * @return The layer index.
     */
    virtual int GetLayer() const;

    /**
     * Set the layer this item is on.
     * @param aLayer - The layer index to set.
     */
    virtual void SetLayer( int aLayer );

    /**
     * Set the position of the item.
     * @param aPos - The new position vector.
     */
    virtual void SetPosition( const VECTOR2I& aPos ) override;

    /**
     * Get the position of the item.
     * @return The position vector.
     */
    virtual VECTOR2I GetPosition() const override;

    /**
     * Get the bounding box of this item.
     * @return The bounding box.
     */
    virtual const BOX2I GetBoundingBox() const override;

    /**
     * Test if a position is inside or on the boundary of this item.
     * @param aPosition - The position to test.
     * @param aAccuracy - Hit test accuracy.
     * @return True if the position is within the item.
     */
    virtual bool HitTest( const VECTOR2I& aPosition, int aAccuracy = 0 ) const override;

    /**
     * Test if a rectangle intersects this item.
     * @param aRect - The rectangle to test.
     * @param aContained - True to test for containment instead of intersection.
     * @param aAccuracy - Hit test accuracy.
     * @return True if the rectangle intersects or contains the item.
     */
    virtual bool HitTest( const BOX2I& aRect, bool aContained, int aAccuracy = 0 ) const override;

    /**
     * Get a description of this item for display purposes.
     * @param aUnitsProvider - Units provider for conversion.
     * @param aFull - Whether to show full description.
     * @return The description.
     */
    virtual wxString GetItemDescription( UNITS_PROVIDER* aUnitsProvider, bool aFull ) const override;

protected:
    VECTOR2I m_pos;  // Position of the item
};

#endif // CHEM_ITEM_H 