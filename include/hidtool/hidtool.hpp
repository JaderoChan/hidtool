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
 * @mainpage HID Tool 文档
 * @brief 跨平台 C++ HID 输入模拟与事件监听库
 *
 * @section intro_sec 项目简介
 * HID Tool 是一个跨平台的 C++ HID（人机接口设备）输入模拟与事件监听库，
 * 支持**全局键盘监听**、**键盘模拟**、**全局鼠标监听**、**鼠标模拟**（移动、点击、滚轮、拖拽）。
 *
 * 采用 C++11 标准，单例模式，线程安全，支持 Windows / macOS / Linux。
 *
 * @section feature_sec 核心特性
 * - 键盘模块：全局键盘事件监听、键盘输入模拟
 * - 鼠标模块：全局鼠标事件监听、鼠标输入模拟
 * - 跨平台支持：Windows、macOS、Linux
 * - C++11 兼容
 * - 支持静态库 / 动态库构建
 * - 单例模式，线程安全
 *
 * @section platform_sec 平台支持
 * | 平台     | 状态 | 备注 |
 * |----------|------|------|
 * | Windows  | ✅ 支持 | - |
 * | macOS    | ✅ 支持 | 需要辅助功能权限 |
 * | Linux    | ✅ 支持 | 需要管理员权限 |
 *
 * @section require_sec 编译要求
 * - CMake >= 3.26
 * - C++11 编译器
 * - macOS：依赖 CoreFoundation、Carbon、CoreGraphics
 * - Linux：依赖 pthreads
 *
 * @section build_sec 构建与安装
 * @subsection build_sub 构建命令
 * @code{.sh}
 * git clone https://github.com/JaderoChan/hidtool.git
 * cd hidtool
 * cmake -B build -DCMAKE_BUILD_TYPE=Release
 * cmake --build build
 * @endcode
 *
 * @subsection install_sub 安装命令
 * @code{.sh}
 * cmake --install build --prefix /your/install/path
 * @endcode
 *
 * @section cmake_opt_sec CMake 选项
 * | 选项 | 默认值 | 说明 |
 * |------|--------|------|
 * | HIDTOOL_BUILD_WITH_KEYBOARD | ON | 构建键盘模块 |
 * | HIDTOOL_BUILD_WITH_MOUSE | ON | 构建鼠标模块 |
 * | HIDTOOL_BUILD_SHARED | OFF | 构建为动态库 |
 * | HIDTOOL_BUILD_EXAMPLE | ON | 构建示例程序 |
 *
 * @section integrate_sec 项目集成
 * @subsection integrate_findpkg find_package 方式
 * @code{.cmake}
 * find_package(hidtool REQUIRED)
 * target_link_libraries(your_target PRIVATE hidtool)
 * @endcode
 *
 * @subsection integrate_addsub add_subdirectory 方式
 * @code{.cmake}
 * add_subdirectory(hidtool)
 * target_link_libraries(your_target PRIVATE hidtool)
 * @endcode
 *
 * @section module_sec 模块总览
 *
 * @subsection keyboard_sec 键盘模块
 * - **KeyboardHooker**：全局键盘事件监听
 * - **KeyboardSimulator**：键盘按键模拟
 * - **KeyboardKey**：跨平台键值枚举（如 KBDKEY_A、KBDKEY_ENTER）
 *
 * @subsection mouse_sec 鼠标模块
 * - **MouseHooker**：全局鼠标事件监听
 * - **MouseSimulator**：鼠标操作模拟
 * - **MouseButton**：鼠标按键枚举（MSBTN_LEFT、MSBTN_RIGHT 等）
 *
 * @section example_sec 示例项目
 * - HID 事件监听：实时打印键盘鼠标事件
 * - 鼠标转圈：让鼠标沿屏幕内接圆/椭圆轨迹运动
 * - 简易点击器：按键触发鼠标自动点击
 *
 * @section author_sec 信息
 * @author 頔珞JaderoChan
 * @version 2.3.0
 * @date 2026
 * @copyright MIT License
 *
 * @see https://github.com/JaderoChan/hidtool
 */

/**
 * @file
 * @brief 包含所有子模块头文件
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
 * @brief HID 工具库顶级命名空间
 * @details 包含键盘/鼠标监听与模拟所有模块
 */
namespace hidt
{}

#endif // !HIDTOOL_HIDTOOL_HPP
