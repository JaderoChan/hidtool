project(mouse_circle)

if(NOT HIDTOOL_HAS_MOUSE)
    message("${MSG_PREFIX} The example 'mouse_circle' is depend on mouse submodule.")
endif()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE hidtool::hidtool)
