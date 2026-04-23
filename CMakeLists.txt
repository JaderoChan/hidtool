cmake_minimum_required(VERSION 3.26)

project(hidtool VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MSG_PREFIX "[HID Tool]")

include(GNUInstallDirs)

# Set the CMAKE_BUILD_TYPE default value to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Projects cannot be built in the same-level directory of the source code.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "${MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

# Check whether the project is the root project.
if(NOT HIDTOOL_IS_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(HIDTOOL_IS_MASTER_PROJECT ON)
    else()
        set(HIDTOOL_IS_MASTER_PROJECT OFF)
    endif()
endif()

###########
# Options #
###########

option(HIDTOOL_BUILD_WITH_KEYBOARD "Build keyboard module" ON)
set(HIDTOOL_HAS_KEYBOARD ${HIDTOOL_BUILD_WITH_KEYBOARD})

option(HIDTOOL_BUILD_WITH_MOUSE "Build mouse module" ON)
set(HIDTOOL_HAS_MOUSE ${HIDTOOL_BUILD_WITH_MOUSE})

if(NOT HIDTOOL_BUILD_WITH_KEYBOARD AND NOT HIDTOOL_BUILD_WITH_MOUSE)
    message(FATAL_ERROR "${MSG_PREFIX} No any modules were built.")
endif()

option(HIDTOOL_BUILD_SHARED "Build shared library" OFF)
set(HIDTOOL_SHARED ${HIDTOOL_BUILD_SHARED})

########################
# Collect source files #
########################

# Configure config header.
set(CONFIG_HEADER_PATH ${CMAKE_CURRENT_BINARY_DIR}/include/hidtool/config.hpp)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/config.hpp.in
    ${CONFIG_HEADER_PATH}
    @ONLY
)

# Common export headers.
file(GLOB COMMON_EXPORT_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/hidtool/*.hpp)
list(APPEND COMMON_EXPORT_HEADERS ${CONFIG_HEADER_PATH})

# Sources
file(GLOB SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if(WIN32)
    set(PLATFORM_NAME win)
elseif(APPLE)
    set(PLATFORM_NAME apple)
elseif(LINUX)
    set(PLATFORM_NAME linux)
else()
    message(FATAL_ERROR "${MSG_PREFIX} Unsupported platform.")
endif()

file(GLOB COMMON_PLATFORM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/${PLATFORM_NAME}/*.cpp)
list(APPEND SRCS ${COMMON_PLATFORM_SRCS})

if(HIDTOOL_BUILD_WITH_KEYBOARD)
    file(GLOB COMMON_KEYBOARD_SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/keyboard/*.cpp)
    file(GLOB PLATFORM_KEYBOARD_SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/keyboard/${PLATFORM_NAME}/*.cpp)
    list(APPEND SRCS ${COMMON_KEYBOARD_SRCS} ${PLATFORM_KEYBOARD_SRCS})
endif()

if(HIDTOOL_BUILD_WITH_MOUSE)
    file(GLOB COMMON_MOUSE_SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse/*.cpp)
    file(GLOB PLATFORM_MOUSE_SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse/${PLATFORM_NAME}/*.cpp)
    list(APPEND SRCS ${COMMON_MOUSE_SRCS} ${PLATFORM_MOUSE_SRCS})
endif()

#########
# Build #
#########

if(HIDTOOL_BUILD_SHARED)
    message(STATUS "${MSG_PREFIX} Shared library will be built.")
    add_library(${PROJECT_NAME} SHARED ${SRCS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE HIDTOOL_BUILD_SHARED)
else()
    message(STATUS "${MSG_PREFIX} Static library will be built.")
    add_library(${PROJECT_NAME} STATIC ${SRCS})
endif()

# Set library alias.
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

####################
# Configure target #
####################

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

# If it is an MSVC compiler, explicitly specify the encoding to prevent garbled characters.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# Windows 10 for API `SetThreadDpiAwarenessContext()`.
if(WIN32 AND HIDTOOL_BUILD_WITH_MOUSE)
    target_compile_definitions(
        ${PROJECT_NAME} PRIVATE
        WINVER=0x0A00
        _WIN32_WINNT=0x0A00
    )
endif()

##########################
# Configure dependencies #
##########################

# Configure include directories.
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# Configure dependencies related to platform.
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE atomic)   # For 16 bytes atomic.
endif()

# Configure third-party dependencies.
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

###########
# Install #
###########

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# Install export headers.
install(FILES COMMON_EXPORT_HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hidtool)

if(HIDTOOL_BUILD_WITH_KEYBOARD)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/hidtool/keyboard DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hidtool)
endif()

if(HIDTOOL_BUILD_WITH_MOUSE)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/hidtool/mouse DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hidtool)
endif()

# Install license file.
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME})

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

#################
# Build Example #
#################

option(HIDTOOL_BUILD_EXAMPLE "Build example" ${HIDTOOL_IS_MASTER_PROJECT})
if(HIDTOOL_BUILD_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)
endif()
