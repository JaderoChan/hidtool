# HID Tool

[**简体中文** | [**English**](README_EN.md)]

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/JaderoChan/hidtool)

一个跨平台的 C++ HID（人机接口设备）输入模拟与事件监听库，支持键盘和鼠标。

## 特性

- **键盘模块**：全局键盘事件监听、键盘输入模拟
- **鼠标模块**：全局鼠标事件监听、鼠标输入模拟（移动、点击、滚轮、拖拽）
- 跨平台支持：Windows、macOS、Linux
- C++11 兼容
- 支持静态库 / 动态库构建
- 单例模式，线程安全

## 平台支持

| 平台 | 状态 | 备注 |
| ---- | ---- | ---- |
| Windows | ✅ | - |
| macOS | ✅ | 需要辅助功能权限（事件监听与模拟功能） |
| Linux | ✅ | 需要管理员权限（操作 input 与 uinput 字符设备） |

> **macOS 注意事项**：由于 macOS API 设计原因，所有模拟函数无法得知执行是否成功，即便返回 `true` 也可能不产生效果。通常需要为应用程序授予**辅助功能**相关权限。

## 要求

- CMake >= 3.26
- C++11 编译器
- **macOS**：CoreFoundation、Carbon、CoreGraphics（CMake 自动查找）
- **Linux**：pthreads

## 构建

```bash
git clone https://github.com/JaderoChan/hidtool.git
cd hitool
cmake -B build [选项]
cmake --build build
```

### CMake 选项

| 选项 | 默认值 | 说明 | 备注 |
| ---- | ------ | ---- | --- |
| `HIDTOOL_BUILD_WITH_KEYBOARD` | `ON` | 构建键盘模块 | - |
| `HIDTOOL_BUILD_WITH_MOUSE` | `ON` | 构建鼠标模块 | - |
| `HIDTOOL_BUILD_SHARED` | `OFF` | 构建为动态库 | - |
| `HIDTOOL_BUILD_EXAMPLE` | `ON`（主项目） | 构建示例程序 | - |
| `HIDTOOL_FORCE_IN_PIXEL` | `ON` | 强制以物理像素为单位操作鼠标指针位置相关 API | 仅限 Windows 10+ |

```bash
# 默认静态库构建
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 构建动态库
cmake -B build -DCMAKE_BUILD_TYPE=Release -DHIDTOOL_BUILD_SHARED=ON

# 仅构建键盘模块
cmake -B build -DHIDTOOL_BUILD_WITH_MOUSE=OFF
```

## 安装

```bash
cmake --install build --prefix /your/install/path
```

## 集成

### CMake — find_package

```cmake
find_package(hidtool REQUIRED)
target_link_libraries(your_target PRIVATE hidtool)
```

### CMake — add_subdirectory

```cmake
add_subdirectory(hidtool)
target_link_libraries(your_target PRIVATE hidtool)
```

## API 文档

### 总览

```cpp
#include <hidtool/hidtool.hpp>  // 包含所有模块
```

也可以单独包含各模块头文件：

```cpp
#include <hidtool/keyboard/keyboard_hooker.hpp>
#include <hidtool/keyboard/keyboard_simulator.hpp>
#include <hidtool/mouse/mouse_hooker.hpp>
#include <hidtool/mouse/mouse_simulator.hpp>
```

---

### 工具函数

```cpp
// 检查当前环境是否支持指定 HID 类型的子模块
bool isHidTypeSupported(HidType hidType) noexcept;
```

---

### 键盘模块

#### `KeyboardHooker` — 键盘事件监听

```cpp
using KeyboardHooker;

// 事件处理回调类型
// 返回 true：正常传播事件；返回 false：阻止事件向其他程序传播
// using KeyboardEventHandler = bool (*)(const KeyboardEvent&);
using KeyboardEventHandler;

KeyboardHooker& hooker = KeyboardHooker::getInstance();

// 检查当前平台是否支持阻断事件传播
bool supported = KeyboardHooker::isSupportBlockEventPropagation();

// 设置事件处理回调
hooker.setEventHandler([](const KeyboardEvent& event) -> bool
{
    if (event.type == KeyboardEvent::ET_PRESS)
    {
        // 处理按键按下事件
    }
    return true; // 继续传播事件
});

hooker.run();    // 开始监听
hooker.stop();   // 停止监听
hooker.isRunning();
```

> **注意**：不要在事件处理回调中调用 `KeyboardHooker` 的成员函数。

#### `KeyboardSimulator` — 键盘输入模拟

```cpp
using KeyboardSimulator;
using KeyboardKey;

KeyboardSimulator& sim = KeyboardSimulator::getInstance();
sim.initialize();

// 使用 KeyboardKey 枚举
sim.pressKey(KeyboardKey::KBDKEY_A);
sim.releaseKey(KeyboardKey::KBDKEY_A);
sim.clickKey(KeyboardKey::KBDKEY_ENTER);    // pressKey + releaseKey

// 使用平台原生键值
sim.pressKey(0x41u);    // Windows VK_A

// 发送组合键（Ctrl+C）
sim.pressKey(KeyboardKey::KBDKEY_CTRL);
sim.clickKey(KeyboardKey::KBDKEY_C);
sim.releaseKey(KeyboardKey::KBDKEY_CTRL);

// 批量发送事件
KeyboardEvent events[] =
{
    KeyboardEvent(KeyboardEvent::ET_PRESS, KeyboardKey::KBDKEY_A),
    KeyboardEvent(KeyboardEvent::ET_RELEASE, KeyboardKey::KBDKEY_A),
};
sim.sendEvent(events, 2);

sim.destroy();
```

#### `KeyboardKey` — 键值枚举

库定义了一套跨平台的键值枚举 `KeyboardKey`，可与平台原生键值互相转换：

```cpp
// KeyboardKey -> 平台原生键值（Win: VK_*, macOS: kVK_*, Linux: KEY_*）
uint32_t nativeKey = keyboardKeyToNativeKey(KeyboardKey::KBDKEY_A);

// 平台原生键值 -> KeyboardKey
KeyboardKey key = keyboardKeyFromNativeKey(0x41u);
```

常用键值：

| 枚举 | 说明 |
| ---- | ---- |
| `KBDKEY_0` ~ `KBDKEY_9` | 数字键 |
| `KBDKEY_A` ~ `KBDKEY_Z` | 字母键 |
| `KBDKEY_F1` ~ `KBDKEY_F24` | 功能键 |
| `KBDKEY_NUMPAD_0` ~ `KBDKEY_NUMPAD_9` | 小键盘数字键 |
| `KBDKEY_CTRL` / `KBDKEY_CTRL_LEFT` / `KBDKEY_CTRL_RIGHT` | Ctrl 键 |
| `KBDKEY_SHIFT` / `KBDKEY_SHIFT_LEFT` / `KBDKEY_SHIFT_RIGHT` | Shift 键 |
| `KBDKEY_ALT` / `KBDKEY_ALT_LEFT` / `KBDKEY_ALT_RIGHT` | Alt 键（别名 `KBDKEY_OPTION`） |
| `KBDKEY_META` / `KBDKEY_META_LEFT` / `KBDKEY_META_RIGHT` | Win/Command 键 |
| `KBDKEY_ENTER` / `KBDKEY_RETURN` | 回车键 |
| `KBDKEY_ESCAPE` / `KBDKEY_ESC` | ESC 键 |
| `KBDKEY_SPACE` | 空格键 |
| `KBDKEY_BACKSPACE` | 退格键 |
| `KBDKEY_TAB` | Tab 键 |
| `KBDKEY_LEFT` / `KBDKEY_RIGHT` / `KBDKEY_UP` / `KBDKEY_DOWN` | 方向键 |

---

### 鼠标模块

#### `MouseHooker` — 鼠标事件监听

```cpp
using MouseHooker;

MouseHooker& hooker = MouseHooker::getInstance();

// 检查当前平台是否支持阻断事件传播
bool supported = MouseHooker::isSupportBlockEventPropagation();

hooker.setEventHandler([](const MouseEvent& event) -> bool
{
    switch (event.type)
    {
        case MouseEvent::ET_ABS_MOVE:
            // event.absPos.x, event.absPos.y
            break;
        case MouseEvent::ET_REL_MOVE:
            // event.relPos.x, event.relPos.y
            break;
        case MouseEvent::ET_WHEEL:
            // event.wheelDelta（单位量 120，正值远离用户，负值靠近用户）
            break;
        case MouseEvent::ET_DRAG:
            // event.absPos.x, event.absPos.y
            break;
        case MouseEvent::ET_PRESS:
        case MouseEvent::ET_RELEASE:
            // event.button
            break;
    }
    return true;
});

hooker.run();
hooker.stop();
```

> **注意**：不要在事件处理回调中调用 `MouseHooker` 的成员函数。

#### `MouseSimulator` — 鼠标输入模拟

```cpp
using MouseSimulator;
using AbsolutePos;
using RelativePos;
using MouseButton;

MouseSimulator& sim = MouseSimulator::getInstance();
sim.initialize();

// 移动鼠标（绝对坐标，以虚拟屏幕空间为基准）
sim.moveTo(AbsolutePos(100, 200));

// 移动鼠标（相对偏移）
sim.moveBy(RelativePos(10, -5));

// 滚轮（单位量 120，正值向上，负值向下）
sim.wheel(120);     // 向上滚动一格
sim.wheel(-120);    // 向下滚动一格

// 鼠标按键（在当前位置）
sim.pressButton(MouseButton::MSBTN_LEFT);
sim.releaseButton(MouseButton::MSBTN_LEFT);
sim.clickButton(MouseButton::MSBTN_LEFT);

// 在指定位置执行鼠标操作
sim.clickButton(AbsolutePos(500, 300), MouseButton::MSBTN_LEFT);
sim.wheel(AbsolutePos(500, 300), 120);

// 拖拽（从当前位置拖拽到目标位置）
sim.dragCombo(AbsolutePos(800, 400));
// 拖拽（指定起始和终止位置）
sim.dragCombo(AbsolutePos(100, 100), AbsolutePos(800, 400), MouseButton::MSBTN_LEFT);

sim.destroy();
```

> **macOS 注意**：拖拽操作必须使用 `drag()` 函数或发送拖拽类型的事件，通过 `pressButton()` + `moveTo()` + `releaseButton()` 的组合无法实现拖拽。

#### `MouseButton` — 鼠标按键枚举

| 枚举 | 说明 |
| ---- | ---- |
| `MSBTN_LEFT` | 左键 |
| `MSBTN_RIGHT` | 右键 |
| `MSBTN_MIDDLE` | 中键 |
| `MSBTN_BACK` | 后退键 |
| `MSBTN_FORWARD` | 前进键 |

#### 坐标系

```cpp
// 获取当前平台绝对坐标范围
AbsolutePosRange range = getAbsolutePosRange();
// Windows/macOS：虚拟屏幕空间范围
// Linux：始终为 {0, 65535, 0, 65535}
```

---

## 完整使用示例

### 示例：监听键盘并模拟鼠标点击

```cpp
#include <hidtool/hidtool.hpp>

int main()
{
    auto& kbdHooker = KeyboardHooker::getInstance();

    mouseSimulator.initialize();

    kbdHooker.setEventHandler([](const KeyboardEvent& event) -> bool
    {
        // 按下 F1 时，在 (500, 300) 处左键单击
        if (event.type == KeyboardEvent::ET_PRESS &&
            event.nativeKey == keyboardKeyToNativeKey(KBDKEY_F1))
        {
            MouseSimulator::getInstance()
                .clickButton(AbsolutePos(500, 300), MSBTN_LEFT);
        }
        return true;
    });

    kbdHooker.run();

    // 主线程等待...

    kbdHooker.stop();
    mouseSimulator.destroy();
    return 0;
}
```

## 许可证

本项目基于 [MIT License](LICENSE) 开源。

Copyright (c) 2026 頔珞JaderoChan
