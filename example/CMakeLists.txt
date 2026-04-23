cmake_minimum_required(VERSION 3.26)

option(HIDTOOL_BUILD_ECHO_EXAMPLE "Build echo example" ON)
option(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE "Build mouse circle example" ${HIDTOOL_HAS_MOUSE})
option(HIDTOOL_BUILD_SIMPLE_HID_SIM_EXAMPLE "Build simple HID simulator example" OFF)

if(HIDTOOL_BUILD_ECHO_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/echo)
endif()

if(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mouse_circle)
endif()

if(HIDTOOL_BUILD_SIMPLE_HID_SIM_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simple_hid_simulator)
endif()
