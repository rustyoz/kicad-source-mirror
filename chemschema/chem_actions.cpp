#include "chem_actions.h"

// Define the static members of CHEM_EDITOR_ACTIONS
TOOL_ACTION CHEM_EDITOR_ACTIONS::newLine( "chemschema.InteractiveDrawing.newLine",
    AS_GLOBAL, 0, "", "", "", "", "Add a new line" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::newSymbol( "chemschema.InteractiveDrawing.newSymbol",
    AS_GLOBAL, 0, "", "", "", "", "Add a new symbol" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::newJunction( "chemschema.InteractiveDrawing.newJunction",
    AS_GLOBAL, 0, "", "", "", "", "Add a new junction" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::newLabel( "chemschema.InteractiveDrawing.newLabel",
    AS_GLOBAL, 0, "", "", "", "", "Add a new label" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::newScreen( "chemschema.InteractiveDrawing.newScreen",
    AS_GLOBAL, 0, "", "", "", "", "Add a new screen" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::newSheet( "chemschema.InteractiveDrawing.newSheet",
    AS_GLOBAL, 0, "", "", "", "", "Add a new sheet" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::measureDistance( "chemschema.MeasurementTool.measureDistance",
    AS_GLOBAL, 0, "", "", "", "", "Measure distance" );

TOOL_ACTION CHEM_EDITOR_ACTIONS::measureAngle( "chemschema.MeasurementTool.measureAngle",
    AS_GLOBAL, 0, "", "", "", "", "Measure angle" );

CHEM_EDITOR_ACTIONS::CHEM_EDITOR_ACTIONS()
{
} 