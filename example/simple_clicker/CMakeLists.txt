project(simple_clicker)

if(NOT HIDTOOL_HAS_KEYBOARD OR NOT HIDTOOL_HAS_MOUSE)
    message("${MSG_PREFIX} The example 'simple_clicker' is depend on mouse submodule.")
endif()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE hidtool::hidtool)
