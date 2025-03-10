# Add all the warnings to the files
if( COMPILER_SUPPORTS_WARNINGS )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS_CXX}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARN_FLAGS_C}")
endif()

include_directories( BEFORE ${INC_BEFORE} )
include_directories(
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/dialogs
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
    )

set( CHEMSCHEMA_DIALOGS
    dialogs/dialog_symbol_properties.cpp
    dialogs/dialog_chem_properties.cpp
    dialogs/dialog_edit_label.cpp
    )

set( CHEMSCHEMA_TOOLS
    tools/chem_editor_control.cpp
    tools/chem_drawing_tools.cpp
    tools/chem_selection_tool.cpp
    )

add_library( chemschema_kiface STATIC
    ${CHEMSCHEMA_SRCS}
    ${CHEMSCHEMA_COMMON_SRCS}
    ${CHEMSCHEMA_DIALOGS}
    ${CHEMSCHEMA_TOOLS}
    )

# Create a shared library for the actual chemical process flow diagram editor
add_library( chemschema SHARED
    chemschema.cpp
    )

target_link_libraries( chemschema
    chemschema_kiface
    common
    pcbcommon
    gal
    ${wxWidgets_LIBRARIES}
    )

set_target_properties( chemschema PROPERTIES
    OUTPUT_NAME chemschema
    SUFFIX "${KIFACE_SUFFIX}"
    )

install( TARGETS chemschema
    DESTINATION ${KICAD_PLUGIN}
    COMPONENT binary
    ) 