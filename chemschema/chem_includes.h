#ifndef CHEM_INCLUDES_H
#define CHEM_INCLUDES_H

// Include the proper paths to the missing header files

// For color4d.h
#include <gal/color4d.h>

// For plotter.h
#include <plotters/plotter.h>

// For collectors-related functionality
// This path is incorrect and will be corrected later when the proper collectors
// implementation is ready for the chemschema module
// #include <pcbnew/collectors.h>

// For optional.h
#include <boost/optional.hpp>

// Add a stub for chem_actions.h until it's properly implemented
#ifndef CHEM_ACTIONS_H
#define CHEM_ACTIONS_H

// Temporary placeholder for the chem_actions.h file
// Actions are now defined in the CHEM_EDITOR_ACTIONS class

#endif // CHEM_ACTIONS_H

#include <wx/string.h>

// Internal units per mil (1/1000 inch)
typedef int IU_PER_MILS;

// Default scale factor for converting mils to internal units
const IU_PER_MILS Mils2iu = 10000;

// Layer definitions for chemical schematic
enum CHEM_LAYER_ID
{
    CHEM_LAYER_BACKGROUND,
    CHEM_LAYER_GRID,
    CHEM_LAYER_CONNECTIONS,
    CHEM_LAYER_SYMBOLS,
    CHEM_LAYER_LABELS,
    CHEM_LAYER_JUNCTIONS,
    CHEM_LAYER_SCREENS,
    CHEM_LAYER_SHEETS,
    CHEM_LAYER_SELECTION,
    CHEM_LAYER_ID_COUNT
};

#endif // CHEM_INCLUDES_H 