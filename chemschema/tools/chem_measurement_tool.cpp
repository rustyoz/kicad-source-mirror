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

#include <wx/statusbr.h>
#include <tool/tool_manager.h>
#include <tool/tool_event.h>
#include <view/view.h>
#include <view/view_controls.h>
#include <gal/graphics_abstraction_layer.h>
#include <confirm.h>
#include <math/util.h>
#include <geometry/shape_line_chain.h>
#include <geometry/geometry_utils.h>

#include "chem_measurement_tool.h"
#include "../chem_edit_frame.h"
#include "../chem_schematic.h"
#include "../chem_actions.h"

using namespace std;

CHEM_MEASUREMENT_TOOL::CHEM_MEASUREMENT_TOOL() :
    TOOL_INTERACTIVE( "chemschema.MeasurementTool" ),
    m_frame( nullptr ),
    m_measureMode( MEASUREMENT_DISTANCE ),
    m_measureInProgress( false )
{
}


CHEM_MEASUREMENT_TOOL::~CHEM_MEASUREMENT_TOOL()
{
    ClearMeasurement();
}


void CHEM_MEASUREMENT_TOOL::Reset( RESET_REASON aReason )
{
    ClearMeasurement();
}


int CHEM_MEASUREMENT_TOOL::Main( const TOOL_EVENT& aEvent )
{
    // Initialize the tool
    m_frame = getEditFrame<CHEM_EDIT_FRAME>();
    CHEM_SCHEMATIC* schematic = getModel<CHEM_SCHEMATIC>();
    
    // Set up the drawing environment
    getViewControls()->ShowCursor( true );
    getViewControls()->SetSnapping( true );
    getViewControls()->SetAutoPan( true );
    
    // Clear any existing measurement
    ClearMeasurement();
    m_measureInProgress = true;
    
    // Determine the measurement mode
    if( aEvent.IsAction( &CHEMSCHEMA_ACTIONS::measureDistance ) )
        m_measureMode = MEASUREMENT_DISTANCE;
    else if( aEvent.IsAction( &CHEMSCHEMA_ACTIONS::measureAngle ) )
        m_measureMode = MEASUREMENT_ANGLE;
    else
        m_measureMode = MEASUREMENT_DISTANCE;  // Default
    
    // Inform the user what to do
    if( m_measureMode == MEASUREMENT_DISTANCE )
        m_frame->SetStatusText( _( "Click on first point for distance measurement." ) );
    else
        m_frame->SetStatusText( _( "Click on first point for angle measurement." ) );
    
    // Main event loop
    while( TOOL_EVENT* evt = Wait() )
    {
        if( evt->IsCancel() || evt->IsActivate() )
        {
            // Cancel or deactivate - clean up
            ClearMeasurement();
            m_measureInProgress = false;
            break;
        }
        
        else if( evt->IsMotion() )
        {
            // If we have at least one point, show measurement preview
            if( !m_points.empty() )
            {
                // Add the current cursor position as a temporary point for preview
                VECTOR2I cursorPos = GetCursorPosition();
                
                if( m_measureMode == MEASUREMENT_DISTANCE && m_points.size() == 1 )
                {
                    // Calculate and display the distance
                    double distance = CalculateDistance( m_points[0], cursorPos );
                    wxString msg = wxString::Format( _( "Distance: %.3f mm" ), distance );
                    DisplayMeasurement( msg );
                }
                else if( m_measureMode == MEASUREMENT_ANGLE && m_points.size() == 2 )
                {
                    // Calculate and display the angle
                    double angle = CalculateAngle( m_points[0], m_points[1], cursorPos );
                    wxString msg = wxString::Format( _( "Angle: %.1f degrees" ), angle );
                    DisplayMeasurement( msg );
                }
                
                // Draw the preview
                DrawMeasurement();
            }
        }
        
        else if( evt->IsClick( BUT_LEFT ) )
        {
            // Add the clicked point
            VECTOR2I pos = GetCursorPosition();
            AddPoint( pos );
            
            // Update the user instructions
            if( m_measureMode == MEASUREMENT_DISTANCE )
            {
                if( m_points.size() == 1 )
                {
                    m_frame->SetStatusText( _( "Click on second point for distance measurement." ) );
                }
                else if( m_points.size() == 2 )
                {
                    // Calculate and display the final distance
                    double distance = CalculateDistance( m_points[0], m_points[1] );
                    wxString msg = wxString::Format( _( "Distance: %.3f mm" ), distance );
                    DisplayMeasurement( msg );
                    
                    // Right click or ESC to exit, click to start a new measurement
                    m_frame->SetStatusText( _( "Click to start a new measurement, right-click to exit." ) );
                }
            }
            else if( m_measureMode == MEASUREMENT_ANGLE )
            {
                if( m_points.size() == 1 )
                {
                    m_frame->SetStatusText( _( "Click on second point (vertex) for angle measurement." ) );
                }
                else if( m_points.size() == 2 )
                {
                    m_frame->SetStatusText( _( "Click on third point for angle measurement." ) );
                }
                else if( m_points.size() == 3 )
                {
                    // Calculate and display the final angle
                    double angle = CalculateAngle( m_points[0], m_points[1], m_points[2] );
                    wxString msg = wxString::Format( _( "Angle: %.1f degrees" ), angle );
                    DisplayMeasurement( msg );
                    
                    // Right click or ESC to exit, click to start a new measurement
                    m_frame->SetStatusText( _( "Click to start a new measurement, right-click to exit." ) );
                }
            }
            
            // Draw the measurement
            DrawMeasurement();
            
            // Reset points if we've completed a measurement
            if( (m_measureMode == MEASUREMENT_DISTANCE && m_points.size() >= 2) ||
                (m_measureMode == MEASUREMENT_ANGLE && m_points.size() >= 3) )
            {
                // Keep the measurement displayed but prepare for a new one
                ClearMeasurement();
            }
        }
        
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            // Exit the tool
            ClearMeasurement();
            m_measureInProgress = false;
            break;
        }
    }
    
    // Clean up
    getViewControls()->SetAutoPan( false );
    m_frame->SetStatusText( wxEmptyString );
    
    return 0;
}


int CHEM_MEASUREMENT_TOOL::MeasureDistance( const TOOL_EVENT& aEvent )
{
    m_measureMode = MEASUREMENT_DISTANCE;
    return Main( aEvent );
}


int CHEM_MEASUREMENT_TOOL::MeasureAngle( const TOOL_EVENT& aEvent )
{
    m_measureMode = MEASUREMENT_ANGLE;
    return Main( aEvent );
}


void CHEM_MEASUREMENT_TOOL::DrawMeasurement()
{
    KIGFX::VIEW* view = getView();
    KIGFX::GAL* gal = view->GetGAL();
    
    // Set up drawing parameters
    KIGFX::COLOR4D measurementColor( 0.0, 1.0, 0.0, 0.8 );  // Green, semi-transparent
    double lineWidth = 2.0;
    
    // Store the current drawing mode
    KIGFX::GAL::STROKE_STYLE savedLineStyle = gal->GetStrokeStyle();
    KIGFX::COLOR4D savedColor = gal->GetStrokeColor();
    double savedLineWidth = gal->GetLineWidth();
    
    // Set new drawing style
    gal->SetStrokeColor( measurementColor );
    gal->SetLineWidth( lineWidth );
    gal->SetStrokeStyle( KIGFX::GAL::STROKE_STYLE::SOLID );
    
    // Draw based on the measurement mode
    if( m_measureMode == MEASUREMENT_DISTANCE )
    {
        if( m_points.size() >= 1 )
        {
            // For distance measurement, draw a line between the two points
            VECTOR2I p1 = m_points[0];
            VECTOR2I p2 = m_points.size() >= 2 ? m_points[1] : GetCursorPosition();
            
            gal->DrawLine( p1, p2 );
            
            // Draw small crosses at the points
            double crossSize = 10.0;
            gal->DrawLine( VECTOR2D( p1.x - crossSize, p1.y ), VECTOR2D( p1.x + crossSize, p1.y ) );
            gal->DrawLine( VECTOR2D( p1.x, p1.y - crossSize ), VECTOR2D( p1.x, p1.y + crossSize ) );
            
            gal->DrawLine( VECTOR2D( p2.x - crossSize, p2.y ), VECTOR2D( p2.x + crossSize, p2.y ) );
            gal->DrawLine( VECTOR2D( p2.x, p2.y - crossSize ), VECTOR2D( p2.x, p2.y + crossSize ) );
        }
    }
    else if( m_measureMode == MEASUREMENT_ANGLE )
    {
        if( m_points.size() >= 2 )
        {
            // For angle measurement, draw lines from the vertex to the two points
            VECTOR2I p1 = m_points[0];
            VECTOR2I p2 = m_points[1];  // Vertex
            VECTOR2I p3 = m_points.size() >= 3 ? m_points[2] : GetCursorPosition();
            
            // Draw the two lines
            gal->DrawLine( p2, p1 );
            gal->DrawLine( p2, p3 );
            
            // Draw small crosses at the points
            double crossSize = 10.0;
            gal->DrawLine( VECTOR2D( p1.x - crossSize, p1.y ), VECTOR2D( p1.x + crossSize, p1.y ) );
            gal->DrawLine( VECTOR2D( p1.x, p1.y - crossSize ), VECTOR2D( p1.x, p1.y + crossSize ) );
            
            gal->DrawLine( VECTOR2D( p2.x - crossSize, p2.y ), VECTOR2D( p2.x + crossSize, p2.y ) );
            gal->DrawLine( VECTOR2D( p2.x, p2.y - crossSize ), VECTOR2D( p2.x, p2.y + crossSize ) );
            
            gal->DrawLine( VECTOR2D( p3.x - crossSize, p3.y ), VECTOR2D( p3.x + crossSize, p3.y ) );
            gal->DrawLine( VECTOR2D( p3.x, p3.y - crossSize ), VECTOR2D( p3.x, p3.y + crossSize ) );
            
            // Draw the arc for the angle if all three points are defined
            if( m_points.size() >= 3 || m_measureInProgress )
            {
                // Calculate vectors for the two lines
                VECTOR2D v1( p1.x - p2.x, p1.y - p2.y );
                VECTOR2D v2( p3.x - p2.x, p3.y - p2.y );
                
                // Get the angle between them
                double angle1 = atan2( v1.y, v1.x );
                double angle2 = atan2( v2.y, v2.x );
                
                // Determine which direction to draw the arc
                double startAngle = angle1;
                double endAngle = angle2;
                
                // Draw an arc to indicate the angle
                double radius = 50.0;  // Radius of the arc
                gal->DrawArc( VECTOR2D( p2.x, p2.y ), radius, startAngle, endAngle );
            }
        }
    }
    
    // Restore previous style
    gal->SetStrokeStyle( savedLineStyle );
    gal->SetStrokeColor( savedColor );
    gal->SetLineWidth( savedLineWidth );
    
    view->RedrawRect( BOX2I( VECTOR2I( 0, 0 ), VECTOR2I( INT_MAX, INT_MAX ) ) );
}


void CHEM_MEASUREMENT_TOOL::ClearMeasurement()
{
    m_points.clear();
    
    // Make the drawing areas redraw without the measurement
    if( getView() )
        getView()->RedrawRect( BOX2I( VECTOR2I( 0, 0 ), VECTOR2I( INT_MAX, INT_MAX ) ) );
}


VECTOR2I CHEM_MEASUREMENT_TOOL::GetCursorPosition()
{
    return getViewControls()->GetCursorPosition();
}


void CHEM_MEASUREMENT_TOOL::AddPoint( const VECTOR2I& aPoint )
{
    m_points.push_back( aPoint );
}


double CHEM_MEASUREMENT_TOOL::CalculateDistance( const VECTOR2I& aStart, const VECTOR2I& aEnd )
{
    // Calculate the Euclidean distance
    double dx = aEnd.x - aStart.x;
    double dy = aEnd.y - aStart.y;
    
    // Convert to mm (assuming IU are in mils)
    return sqrt( dx * dx + dy * dy ) / IU_PER_MM;
}


double CHEM_MEASUREMENT_TOOL::CalculateAngle( const VECTOR2I& aPoint1, const VECTOR2I& aPoint2, const VECTOR2I& aPoint3 )
{
    // Create vectors from the vertex (aPoint2) to the other points
    VECTOR2D v1( aPoint1.x - aPoint2.x, aPoint1.y - aPoint2.y );
    VECTOR2D v2( aPoint3.x - aPoint2.x, aPoint3.y - aPoint2.y );
    
    // Normalize the vectors
    double len1 = sqrt( v1.x * v1.x + v1.y * v1.y );
    double len2 = sqrt( v2.x * v2.x + v2.y * v2.y );
    
    if( len1 == 0.0 || len2 == 0.0 )
        return 0.0;
    
    v1.x /= len1;
    v1.y /= len1;
    v2.x /= len2;
    v2.y /= len2;
    
    // Calculate dot product and determine the angle
    double dotProduct = v1.x * v2.x + v1.y * v2.y;
    
    // Clamp the dot product to avoid numerical errors
    if( dotProduct > 1.0 )
        dotProduct = 1.0;
    else if( dotProduct < -1.0 )
        dotProduct = -1.0;
    
    // Calculate the angle in degrees
    double angleRad = acos( dotProduct );
    double angleDeg = angleRad * 180.0 / M_PI;
    
    // Determine direction (clockwise or counterclockwise)
    double crossProduct = v1.x * v2.y - v1.y * v2.x;
    if( crossProduct < 0 )
        angleDeg = 360.0 - angleDeg;
    
    return angleDeg;
}


void CHEM_MEASUREMENT_TOOL::DisplayMeasurement( const wxString& aMessage )
{
    if( m_frame )
        m_frame->SetStatusText( aMessage );
}


void CHEM_MEASUREMENT_TOOL::setTransitions()
{
    Go( &CHEM_MEASUREMENT_TOOL::Main,             TC_COMMAND, TA_ACTIVATE, "chemschema.MeasurementTool" );
    Go( &CHEM_MEASUREMENT_TOOL::MeasureDistance,  CHEMSCHEMA_ACTIONS::measureDistance.MakeEvent() );
    Go( &CHEM_MEASUREMENT_TOOL::MeasureAngle,     CHEMSCHEMA_ACTIONS::measureAngle.MakeEvent() );
} 