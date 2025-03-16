add_compile_definitions( CHEMLAYOUT )

include_directories( BEFORE ${INC_BEFORE} )
include_directories(
    ./dialogs
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/common/dialogs
    ${INC_AFTER}
    )

set( CHEMLAYOUT_SRCS
    chemlayout.cpp
    chemlayout_settings.cpp
    chem_plant.cpp
    chem_view.cpp
    chem_edit_frame.cpp
    )

set( CHEMLAYOUT_COMMON_SRCS
    chem_item.cpp
    chem_shape.cpp
    chem_text.cpp
    chem_textbox.cpp
 #   chem_reactor.cpp
#   chem_pipe.cpp
#   chem_valve.cpp
#   chem_pump.cpp
#   chem_vessel.cpp
#   chem_sensor.cpp
#   chem_group.cpp
#   chem_dimension.cpp
    )

set( CHEMLAYOUT_DIALOGS
    dialogs/dialog_reactor_properties.cpp
    dialogs/dialog_pipe_properties.cpp
    dialogs/dialog_chem_properties.cpp
    )

set( CHEMLAYOUT_TOOLS
    tools/chem_editor_control.cpp
    tools/chem_drawing_tools.cpp
    tools/chem_selection_tool.cpp
    tools/chem_measurement_tool.cpp
    )

add_library( chemlayout_kiface STATIC
    ${CHEMLAYOUT_SRCS}
    ${CHEMLAYOUT_COMMON_SRCS}
    ${CHEMLAYOUT_DIALOGS}
    ${CHEMLAYOUT_TOOLS}
    )

# Create a shared library for the actual chemical reactor layout editor
add_library( chemlayout SHARED
    chemlayout.cpp
    )

target_link_libraries( chemlayout
    chemlayout_kiface
    common
    pcbcommon
    gal
    ${wxWidgets_LIBRARIES}
    )

set_target_properties( chemlayout PROPERTIES
    OUTPUT_NAME chemlayout
    SUFFIX "${KIFACE_SUFFIX}"
    )

install( TARGETS chemlayout
    DESTINATION ${KICAD_PLUGIN}
    COMPONENT binary
    ) 