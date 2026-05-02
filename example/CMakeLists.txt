cmake_minimum_required(VERSION 3.26)

option(HIDTOOL_BUILD_ECHO_EXAMPLE "Build echo example" ON)
option(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE "Build mouse circle example" ${HIDTOOL_HAS_MOUSE})

if(HIDTOOL_BUILD_ECHO_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/echo)
endif()

if(HIDTOOL_BUILD_MOUSE_CIRCLE_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mouse_circle)
endif()
