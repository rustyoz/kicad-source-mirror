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

#ifndef CHEM_MEASUREMENT_TOOL_H
#define CHEM_MEASUREMENT_TOOL_H

#include <tool/tool_interactive.h>
#include <tool/tool_event.h>
#include <tool/actions.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <gal/color4d.h>
#include <gal/definitions.h>
#include <math/vector2d.h>
#include "../chem_actions.h"

class CHEM_EDIT_FRAME;

/**
 * Tool for measuring distances and angles in the chemical schematic editor.
 */
class CHEM_MEASUREMENT_TOOL : public TOOL_INTERACTIVE
{
public:
    /**
     * Constructor
     */
    CHEM_MEASUREMENT_TOOL();

    /**
     * Destructor
     */
    ~CHEM_MEASUREMENT_TOOL();

    /**
     * Reset the tool state
     * @param aReason - Reason for the reset
     */
    void Reset( RESET_REASON aReason ) override;

    /**
     * Initialize this tool
     */
    void setTransitions() override;

    /**
     * Main loop for this tool
     * @param aEvent - Tool activation event
     */
    int Main( const TOOL_EVENT& aEvent );

    /**
     * Handler for the measure distance command
     * @param aEvent - Tool event
     */
    int MeasureDistance( const TOOL_EVENT& aEvent );

    /**
     * Handler for the measure angle command
     * @param aEvent - Tool event
     */
    int MeasureAngle( const TOOL_EVENT& aEvent );

private:
    /**
     * Draw the current measurement
     */
    void DrawMeasurement();

    /**
     * Clear the current measurement
     */
    void ClearMeasurement();

    /**
     * Get the current cursor position
     * @return Current cursor position
     */
    VECTOR2I GetCursorPosition();

    /**
     * Add a point to the measurement
     * @param aPoint - Point to add
     */
    void AddPoint( const VECTOR2I& aPoint );

    /**
     * Calculate the distance between two points
     * @param aStart - Start point
     * @param aEnd - End point
     * @return Distance in mm
     */
    double CalculateDistance( const VECTOR2I& aStart, const VECTOR2I& aEnd );

    /**
     * Calculate the angle between three points
     * @param aPoint1 - First point
     * @param aPoint2 - Vertex point
     * @param aPoint3 - Third point
     * @return Angle in degrees
     */
    double CalculateAngle( const VECTOR2I& aPoint1, const VECTOR2I& aPoint2, const VECTOR2I& aPoint3 );

    /**
     * Display a measurement message
     * @param aMessage - Message to display
     */
    void DisplayMeasurement( const wxString& aMessage );

    // Measurement mode
    enum MEASUREMENT_MODE {
        MEASUREMENT_DISTANCE,
        MEASUREMENT_ANGLE
    };

    // Private members
    CHEM_EDIT_FRAME* m_frame;
    MEASUREMENT_MODE m_measureMode;
    bool m_measureInProgress;
    std::vector<VECTOR2I> m_points;
};

#endif // CHEM_MEASUREMENT_TOOL_H 