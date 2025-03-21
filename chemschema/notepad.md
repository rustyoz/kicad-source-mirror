# Chemschema Class Hierarchy

## Overview
This document outlines the class hierarchy for the Chemschema module, which is designed to handle chemical schematic diagrams in KiCad. The hierarchy is based on the existing Eeschema structure but adapted for chemical-specific needs.

## Base Classes

### EDA_ITEM
- Base class for all KiCad items
- Provides common functionality for all schematic items

### BASE_SCREEN
- Base class for screen management
- Handles basic screen operations and item collections

## Core Classes

### CHEM_ITEM
```cpp
class CHEM_ITEM : public EDA_ITEM
{
    // Base class for all chemical schematic items
    // Common properties and methods for chemical components
};
```

### CHEM_SCREEN
```cpp
class CHEM_SCREEN : public BASE_SCREEN
{
    // Manages a single chemical schematic page
    // Handles collection and management of CHEM_ITEM objects
};
```

### CHEM_SCHEMATIC
```cpp
class CHEM_SCHEMATIC : public SCHEMATIC
{
    // Top-level container for chemical schematic
    // Manages the entire chemical diagram hierarchy
};
```

## Chemical Components

### CHEM_SYMBOL
```cpp
class CHEM_SYMBOL : public CHEM_ITEM
{
    // Represents chemical symbols (molecules, reactions)
    // Handles chemical-specific properties and rendering
};
```

### CHEM_TEXT
```cpp
class CHEM_TEXT : public CHEM_ITEM
{
    // Represents text annotations in chemical schematics
    // Handles chemical formula text and labels
};
```

### CHEM_SHEET
```cpp
class CHEM_SHEET : public CHEM_ITEM
{
    // Represents a chemical reaction sheet
    // Manages hierarchical chemical diagrams
};
```

## Key Features

### CHEM_ITEM
- Common chemical item properties
- Movement and locking functionality
- Chemical-specific type checking
- Base rendering capabilities

### CHEM_SCREEN
- Screen management
- Item collection and organization
- Drawing and layout capabilities
- Reaction and molecule tracking

### CHEM_SYMBOL
- Molecule and reaction representation
- Stoichiometry management
- Chemical symbol rendering
- Property management

### CHEM_SHEET
- Sheet management
- Reaction organization
- Hierarchy management
- Screen association

### CHEM_SCHEMATIC
- Overall schematic management
- Root sheet handling
- Reaction validation
- Chemical conditions management

## Key Differences from Eeschema

1. Chemical-Specific Features
   - Reaction management instead of electrical connectivity
   - Molecule and reagent handling
   - Stoichiometry tracking
   - Chemical conditions (temperature, pressure)

2. Simplified Connectivity
   - No complex electrical connection graphs
   - Focus on reaction pathways
   - Simpler hierarchical structure

3. Specialized Rendering
   - Chemical symbol rendering
   - Reaction arrow drawing
   - Molecular structure visualization
   - Chemical formula text handling

4. Data Management
   - Chemical-specific properties
   - Reaction validation
   - Molecular database integration

## Implementation Notes

1. Inheritance Structure
   - Maintains compatibility with KiCad's existing infrastructure
   - Reuses common functionality from base classes
   - Adds chemical-specific features where needed

2. Key Methods
   - Plot/rendering methods for each class
   - Property management methods
   - Hierarchy management methods
   - Chemical-specific validation methods

3. Data Structures
   - Reaction pathway tracking
   - Molecular property storage
   - Chemical condition management
   - Hierarchical organization

4. Future Considerations
   - Molecular database integration
   - Advanced reaction validation
   - 3D molecular visualization
   - Chemical property calculations

## Usage Guidelines

1. Creating New Items
   - Use appropriate base class
   - Implement required virtual methods
   - Add chemical-specific properties

2. Managing Hierarchy
   - Use CHEM_SCREEN for page management
   - Use CHEM_SHEET for reaction organization
   - Use CHEM_SCHEMATIC for overall structure

3. Rendering
   - Implement Plot methods
   - Handle chemical-specific drawing
   - Manage text and formula rendering

4. Data Management
   - Store chemical properties appropriately
   - Validate reactions and molecules
   - Manage chemical conditions 