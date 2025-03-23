# Add all the warnings to the files
if( COMPILER_SUPPORTS_WARNINGS )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS_CXX}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARN_FLAGS_C}")
endif()

include_directories( BEFORE ${INC_BEFORE} )
include_directories(
    ${CMAKE_SOURCE_DIR}/kicad
    ${CMAKE_SOURCE_DIR}/eeschema
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/dialogs
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/kimath/include
    ${CMAKE_SOURCE_DIR}/libs/sexpr/include
    ${INC_AFTER}
    ./dialogs
    ./tools
    ./widgets
    )

set( CHEMSCHEMA_SRCS
    chemschema.cpp
    chemschema_settings.cpp
    chem_schematic.cpp
    chem_view.cpp
    chem_edit_frame.cpp
    chem_painter.cpp
    chem_frame.cpp
    chem_draw_panel.cpp
    chem_render_settings.cpp
    chem_base_frame.cpp
    chem_collectors.cpp
    chem_connection.cpp
    chem_undo_redo.cpp
    chem_validators.cpp
    )

set( CHEMSCHEMA_COMMON_SRCS
    chem_item.cpp
    chem_shape.cpp
    chem_text.cpp
    chem_textbox.cpp
    chem_line.cpp
    chem_symbol.cpp
    chem_sheet.cpp
    chem_junction.cpp
    chem_label.cpp
    chem_screen.cpp
    chem_sheet_path.cpp
    )

set( CHEMSCHEMA_DIALOGS
    dialogs/dialog_symbol_properties.cpp
    dialogs/dialog_chem_properties.cpp
    dialogs/dialog_edit_label.cpp
    dialogs/dialog_chem_text_properties.cpp
    dialogs/dialog_chem_line_properties.cpp
    dialogs/dialog_chem_junction_properties.cpp
    )

set( CHEMSCHEMA_TOOLS
    tools/chem_editor_control.cpp
    tools/chem_drawing_tools.cpp
    tools/chem_selection_tool.cpp
    tools/chem_measurement_tool.cpp
    tools/chem_placement_tool.cpp
    tools/chem_connection_tool.cpp
    tools/chem_inspection_tool.cpp
    tools/chem_actions.cpp
    tools/chem_move_tool.cpp
    tools/chem_selection.cpp
    )

set( CHEMSCHEMA_HEADERS
    chem_actions.h
    chem_base_frame.h
    chem_collectors.h
    chem_connection.h
    chem_draw_panel.h
    chem_frame.h
    chem_includes.h
    chem_painter.h
    chem_render_settings.h
    )

# Create a shared library for the actual chemical process flow diagram editor
add_executable( chemschema WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/common/single_top.cpp
    )

set_source_files_properties( ${CMAKE_SOURCE_DIR}/common/single_top.cpp PROPERTIES
    COMPILE_DEFINITIONS "TOP_FRAME=FRAME_CHEM_SCHEMA;PGM_DATA_FILE_EXT=\"kicad_chem\";BUILD_KIWAY_DLL"
    )

target_link_libraries( chemschema
    kicommon
    ${wxWidgets_LIBRARIES}
    )

# The main chemical schematic program components in object form
add_library( chemschema_kiface_objects OBJECT
    ${CHEMSCHEMA_SRCS}
    ${CHEMSCHEMA_COMMON_SRCS}
    ${CHEMSCHEMA_DIALOGS}
    ${CHEMSCHEMA_TOOLS}
    )

target_include_directories( chemschema_kiface_objects
    PUBLIC
        .
    )

target_link_libraries( chemschema_kiface_objects
    PUBLIC
        common
    )

# Since we're not using target_link_libraries, we need to explicitly declare the dependency
add_dependencies( chemschema_kiface_objects common )

# Build the kiface module
add_library( chemschema_kiface MODULE
    chemschema.cpp
    )

target_link_libraries( chemschema_kiface
    PRIVATE
        common
        chemschema_kiface_objects
        sexpr
        core
        gal
        pcbcommon
        Boost::headers
        ${wxWidgets_LIBRARIES}
    )

set_target_properties( chemschema_kiface PROPERTIES
    # Decorate OUTPUT_NAME with PREFIX and SUFFIX, creating something like _chemschema.so, _chemschema.dll, etc.
    OUTPUT_NAME chemschema
    PREFIX "${KIFACE_PREFIX}"
    SUFFIX "${KIFACE_SUFFIX}"
    )

# The KIFACE is in chemschema.cpp, export it:
set_source_files_properties( chemschema.cpp PROPERTIES
    COMPILE_DEFINITIONS "BUILD_KIWAY_DLL;COMPILING_DLL"
    )

# If building chemschema, then also build chemschema_kiface if out of date
add_dependencies( chemschema chemschema_kiface )

# Generate link map with cross reference if enabled
target_link_options( chemschema_kiface PRIVATE
    $<$<BOOL:${KICAD_MAKE_LINK_MAPS}>:-Wl,--cref,-Map=${KIFACE_PREFIX}chemschema${KIFACE_SUFFIX}.map>
    )
target_link_options( chemschema PRIVATE
    $<$<BOOL:${KICAD_MAKE_LINK_MAPS}>:-Wl,--cref,-Map=chemschema.map>
    )

# Handle platform specific installation
if( APPLE )
    # Set up for macOS bundle
    set_target_properties( chemschema_kiface PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${OSX_BUNDLE_BUILD_KIFACE_DIR}
        )

    set_target_properties( chemschema PROPERTIES INSTALL_RPATH
        "@executable_path/../Frameworks"
        )
    set_target_properties( chemschema_kiface PROPERTIES INSTALL_RPATH
        "@executable_path/../Frameworks"
        )
    set_target_properties( chemschema_kiface PROPERTIES BUILD_WITH_INSTALL_RPATH 1 )

    # Install the application
    install( TARGETS chemschema
        DESTINATION ${KICAD_BIN}
        COMPONENT binary
        )
else()
    if( MSVC )
        # Add Windows resource for MSVC builds
        target_sources( chemschema_kiface PRIVATE ${CMAKE_SOURCE_DIR}/resources/msw/chemschema-dll.rc )
    endif()

    # Install targets
    install( TARGETS chemschema
        DESTINATION ${KICAD_BIN}
        COMPONENT binary
        )
    install( TARGETS chemschema_kiface
        DESTINATION ${KICAD_KIFACE}
        COMPONENT binary
        )
endif()

# Install debug symbols if enabled
if( KICAD_WIN32_INSTALL_PDBS )
    install(FILES $<TARGET_PDB_FILE:chemschema> DESTINATION ${KICAD_BIN})
    install(FILES $<TARGET_PDB_FILE:chemschema_kiface> DESTINATION ${KICAD_KIFACE})
endif() 