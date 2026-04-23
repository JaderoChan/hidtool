project(mouse_circle)

if(NOT HIDTOOL_HAS_MOUSE)
    message("${MSG_PREFIX} The example 'mouse_circle' is depend on mouse submodule.")
endif()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE hidtool::hidtool)

# Windows 10 for API `SetThreadDpiAwarenessContext()`.
if(WIN32 AND HIDTOOL_BUILD_WITH_MOUSE)
    target_compile_definitions(
        ${PROJECT_NAME} PRIVATE
        WINVER=0x0A00
        _WIN32_WINNT=0x0A00
    )
endif()
