cmake_minimum_required(VERSION 3.26)

if(${HIDTOOL_HAS_KEYBOARD} AND ${HIDTOOL_HAS_MOUSE})
    set(HIDTOOL_HAS_KEYBOARD_MOUSE ON)
else()
    set(HIDTOOL_HAS_KEYBOARD_MOUSE OFF)
endif()

option(HIDTOOL_BUILD_ECHO_EXAMPLE "Build echo example" ${HIDTOOL_HAS_KEYBOARD_MOUSE})
option(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE "Build mouse circle example" ${HIDTOOL_HAS_MOUSE})
option(HIDTOOL_BUILD_SIMPLE_CLICKER "Build simple clicker example" ${HIDTOOL_HAS_KEYBOARD_MOUSE})

# If it is an MSVC compiler, explicitly specify the encoding to prevent garbled characters.
if(MSVC)
    add_compile_options(/utf-8)
endif()

if(HIDTOOL_BUILD_ECHO_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/echo)
endif()

if(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mouse_circle)
endif()

if(HIDTOOL_BUILD_SIMPLE_CLICKER)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simple_clicker)
endif()
