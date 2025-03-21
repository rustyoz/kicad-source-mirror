#ifndef CHEM_ACTIONS_H
#define CHEM_ACTIONS_H

#include <tool/tool_action.h>

class CHEM_EDITOR_ACTIONS
{
public:
    // Constructor
    CHEM_EDITOR_ACTIONS();

    // Drawing tool actions
    static TOOL_ACTION newSymbol;
    static TOOL_ACTION newLine;
    static TOOL_ACTION newJunction;
    static TOOL_ACTION newLabel;
    static TOOL_ACTION newScreen;
    static TOOL_ACTION newSheet;

    // Measurement tool actions
    static TOOL_ACTION measureDistance;
    static TOOL_ACTION measureAngle;
};

#endif // CHEM_ACTIONS_H 