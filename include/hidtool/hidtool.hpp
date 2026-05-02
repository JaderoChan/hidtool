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
 * @defgroup Hooker HID 事件监听器
 */

/**
 * @defgroup Simulators HID 设备模拟器
 * @attention 在 **MacOS** 平台上，由于其 API 设计原因，实际上所有模拟函数都无法知晓其成功与否，
 * 所以，在 **MacOS** 平台上，即使你执行模拟函数并得到 `true` 值，其仍然可能不产生任何效果。通常来讲，
 * 你需要给你的应用程序获得 **辅助功能** 相关权限才能正常执行。
 */

#ifndef HIDTOOL_HIDTOOL_HPP
#define HIDTOOL_HIDTOOL_HPP

#include "defines.hpp"
#include "hidtype.hpp"
#include "hidevent.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_hooker.hpp>
    #include <hidtool/keyboard/keyboard_simulator.hpp>
#endif // HIDTOOL_HAS_KEYBOARD

#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_hooker.hpp>
    #include <hidtool/mouse/mouse_simulator.hpp>
#endif // HIDTOOL_HAS_MOUSE

#endif // !HIDTOOL_HIDTOOL_HPP
