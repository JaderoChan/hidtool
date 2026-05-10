// Repository: https://github.com/JaderoChan/hidtool
// Author: 頔珞JaderoChan

// MIT License
//
// Copyright (c) 2026 頔珞JaderoChan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/**
 * @mainpage HID Tool Documentation
 * @brief Cross-platform C++ HID input simulation and event listening library
 *
 * @section intro_sec Project Introduction
 *
 * HID Tool is a cross-platform C++ HID (Human Interface Device) input simulation and event listening library,
 * supporting global keyboard listening, keyboard simulation, global mouse listening, and mouse simulation (movement, click, scroll, drag).
 *
 * It adopts the C++11 standard, singleton pattern, thread-safe, and supports Windows / macOS / Linux.
 *
 * @section feature_sec Core Features
 *
 * - Keyboard Module: Global keyboard event listening, keyboard input simulation
 * - Mouse Module: Global mouse event listening, mouse input simulation
 * - Cross-platform Support: Windows, macOS, Linux
 * - C++11 Compatible
 * - Supports static library / dynamic library builds
 * - Singleton pattern, thread-safe
 *
 * @section platform_sec Platform Support
 *
 * | Platform  | Status | Notes |
 * |-----------|--------|-------|
 * | Windows   | ✅ Supported | - |
 * | macOS     | ✅ Supported | Requires accessibility permissions |
 * | Linux     | ✅ Supported | Requires administrator permissions |
 *
 * @section require_sec Compilation Requirements
 *
 * - CMake >= 3.26
 * - C++11 Compiler
 * - macOS: Depends on CoreFoundation, Carbon, CoreGraphics
 * - Linux: Depends on pthreads
 *
 * @section build_sec Build and Installation
 *
 * @subsection build_subsec Build Commands
 *
 * @code{.sh}
 * git clone https://github.com/JaderoChan/hidtool.git
 * cd hidtool
 * cmake -B build -DCMAKE_BUILD_TYPE=Release
 * cmake --build build
 * @endcode
 *
 * @subsection install_subsec Installation Commands
 *
 * @code{.sh}
 * cmake --install build --prefix /your/install/path
 * @endcode
 *
 * @section cmake_option_sec CMake Options
 *
 * | Option | Default Value | Description |
 * |--------|---------------|-------------|
 * | HIDTOOL_BUILD_WITH_KEYBOARD | ON | Build keyboard module |
 * | HIDTOOL_BUILD_WITH_MOUSE | ON | Build mouse module |
 * | HIDTOOL_BUILD_SHARED | OFF | Build as shared library |
 * | HIDTOOL_BUILD_EXAMPLE | ON | Build example programs |
 *
 * @section integrate_sec Project Integration
 *
 * @subsection integrate_find_package_subsec find_package Method
 *
 * @code{.cmake}
 * find_package(hidtool REQUIRED)
 * target_link_libraries(your_target PRIVATE hidtool)
 * @endcode
 *
 * @subsection integrate_add_subdirectory_subsec add_subdirectory Method
 *
 * @code{.cmake}
 * add_subdirectory(hidtool)
 * target_link_libraries(your_target PRIVATE hidtool)
 * @endcode
 *
 * @section module_sec Module Overview
 *
 * @subsection keyboard_sec Keyboard Module
 *
 * - <b>KeyboardHooker</b>: Global keyboard event listener
 * - <b>KeyboardSimulator</b>: Keyboard key simulation
 * - <b>KeyboardKey</b>: Cross-platform key enumeration (e.g., KBDKEY_A, KBDKEY_ENTER)
 *
 * @subsection mouse_sec Mouse Module
 *
 * - <b>MouseHooker</b>: Global mouse event listener
 * - <b>MouseSimulator</b>: Mouse operation simulator
 * - <b>MouseButton</b>: Mouse button enumeration (MSBTN_LEFT, MSBTN_RIGHT, etc.)
 *
 * @section example_sec Example Projects
 *
 * - HID Event Listener: Real-time printing of keyboard and mouse events.
 * - Mouse Circling: Make the mouse move along a circular/elliptical trajectory on the screen.
 * - Simple Clicker: Trigger automatic mouse clicks with a key press.
 *
 * @section author_sec Information
 *
 * @author 頔珞JaderoChan
 * @version 2.3.2
 * @date 2026
 * @copyright MIT License
 *
 * @see https://github.com/JaderoChan/hidtool
 */

/**
 * @file
 * @brief Include all submodule header files
 */

#ifndef HIDTOOL_HIDTOOL_HPP
#define HIDTOOL_HIDTOOL_HPP

#include "defines.hpp"
#include "hidtype.hpp"
#include "hidevent.hpp"
#include "hid_hooker.hpp"
#include "hid_simulator.hpp"

/**
 * @namespace hidt
 * @brief Top-level namespace of HID Tool library.
 */
namespace hidt
{}

#endif // !HIDTOOL_HIDTOOL_HPP
