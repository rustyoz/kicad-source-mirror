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
#include <wx/gdicmn.h>
#include <vector>
#include <view/view.h>
#include <math/vector2d.h>

class CHEM_EDIT_FRAME;
class CHEM_SCHEMATIC;
class KIGFX::GAL;
class KIID;

/**
 * @brief Tool for measuring distances and angles in chemical schematics.
 *
 * This tool allows users to measure distances between points, measure angles
 * between lines, and display the coordinates of selected points.
 */
class CHEM_MEASUREMENT_TOOL : public TOOL_INTERACTIVE
{
public:
    CHEM_MEASUREMENT_TOOL();
    ~CHEM_MEASUREMENT_TOOL();

    /// Tool name identifier
    static const char* GetToolName() { return "chemschema.MeasurementTool"; }
    
    /// Main run function that handles measurements
    int Main( const TOOL_EVENT& aEvent );
    
    /// Reset tool state when activated
    void Reset( RESET_REASON aReason ) override;
    
    /**
     * @brief Measure distance between two points.
     * @param aEvent The tool event
     * @return 0 on success
     */
    int MeasureDistance( const TOOL_EVENT& aEvent );
    
    /**
     * @brief Measure angle between three points or two lines.
     * @param aEvent The tool event
     * @return 0 on success
     */
    int MeasureAngle( const TOOL_EVENT& aEvent );

    /**
     * @brief Setup transitions for this tool
     */
    void setTransitions() override;

private:
    enum MEASUREMENT_MODE
    {
        MEASUREMENT_DISTANCE,
        MEASUREMENT_ANGLE
    };

    /**
     * @brief Draw preview of measurement
     */
    void DrawMeasurement();
    
    /**
     * @brief Clear the current measurement and preview
     */
    void ClearMeasurement();
    
    /**
     * @brief Get the current cursor position
     * @return The cursor position in screen coordinates
     */
    VECTOR2I GetCursorPosition();
    
    /**
     * @brief Add a point to the current measurement
     * @param aPoint The point to add
     */
    void AddPoint( const VECTOR2I& aPoint );
    
    /**
     * @brief Calculate distance between two points
     * @param aStart The start point
     * @param aEnd The end point
     * @return The distance in current units
     */
    double CalculateDistance( const VECTOR2I& aStart, const VECTOR2I& aEnd );
    
    /**
     * @brief Calculate angle between three points
     * @param aPoint1 First point
     * @param aPoint2 Second point (vertex)
     * @param aPoint3 Third point
     * @return The angle in degrees
     */
    double CalculateAngle( const VECTOR2I& aPoint1, const VECTOR2I& aPoint2, const VECTOR2I& aPoint3 );
    
    /**
     * @brief Display measurement on status bar
     * @param aMessage The message to display
     */
    void DisplayMeasurement( const wxString& aMessage );

private:
    CHEM_EDIT_FRAME*        m_frame;          ///< The current edit frame
    MEASUREMENT_MODE        m_measureMode;    ///< Current measurement mode
    std::vector<VECTOR2I>   m_points;         ///< Points gathered for measurement
    bool                    m_measureInProgress; ///< Indicates if measurement is in progress
};

#endif // CHEM_MEASUREMENT_TOOL_H 