# HID Tool

[[**简体中文**](doc/README_ZH.md) | **English**]

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/JaderoChan/hidtool)

A cross-platform C++ HID (Human Interface Device) input simulation and event listening library, supporting keyboard and mouse.

## Features

- **Keyboard Module**: Global keyboard event listening, keyboard input simulation
- **Mouse Module**: Global mouse event listening, mouse input simulation (move, click, wheel, drag)
- Cross-platform support: Windows, macOS, Linux
- C++11 compatible
- Supports static / shared library builds
- Singleton pattern, thread-safe

## Platform Support

| Platform | Status | Notes |
| -------- | ------ | ----- |
| Windows | ✅ | - |
| macOS | ✅ | Requires Accessibility permissions (event listening and simulation features) |
| Linux | ✅ | Requires administrator privileges (access to input & uinput character devices) |

> **macOS Note**: Due to macOS API design, all simulation functions cannot determine whether execution was successful. Even if `true` is returned, the action may have no effect. You typically need to grant **Accessibility** permissions to the application.

## Requirements

- CMake >= 3.26
- C++11 compiler
- **macOS**: CoreFoundation, Carbon, CoreGraphics (found automatically by CMake)
- **Linux**: pthreads

## Build

```bash
git clone https://github.com/JaderoChan/hidtool.git
cd hidtool
cmake -B build [options]
cmake --build build
```

### CMake Options

| Option | Default | Description | Notes |
| ------ | ------- | ----------- | ----- |
| `HIDTOOL_BUILD_WITH_KEYBOARD` | `ON` | Build keyboard module | - |
| `HIDTOOL_BUILD_WITH_MOUSE` | `ON` | Build mouse module | - |
| `HIDTOOL_BUILD_SHARED` | `OFF` | Build as shared library | - |
| `HIDTOOL_BUILD_EXAMPLE` | `ON` (top-level) | Build example programs | - |
| `HIDTOOL_FORCE_IN_PIXEL` | `ON` | Force APIs related to mouse pointer position to be operated in physical pixel units | Winodws 10+ only |

```bash
# Default static library build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build shared library
cmake -B build -DCMAKE_BUILD_TYPE=Release -DHIDTOOL_BUILD_SHARED=ON

# Build keyboard module only
cmake -B build -DHIDTOOL_BUILD_WITH_MOUSE=OFF
```

## Install

```bash
cmake --install build --prefix /your/install/path
```

## Integration

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

## API Documentation

### Overview

```cpp
#include <hidtool/hidtool.hpp>  // Include all modules
```

Individual module headers can also be included separately:

```cpp
#include <hidtool/keyboard/keyboard_hooker.hpp>
#include <hidtool/keyboard/keyboard_simulator.hpp>
#include <hidtool/mouse/mouse_hooker.hpp>
#include <hidtool/mouse/mouse_simulator.hpp>
```

---

### Utility Functions

```cpp
// Check whether the current environment supports submodules for the specified HID type
bool isHidTypeSupported(HidType hidType) noexcept;
```

---

### Keyboard Module

#### `KeyboardHooker` — Keyboard Event Listening

```cpp
using KeyboardHooker;

// Event handler callback type
// Return true: propagate event normally; Return false: block event propagation to other programs
// using KeyboardEventHandler = bool (*)(const KeyboardEvent&);
using KeyboardEventHandler;

KeyboardHooker& hooker = KeyboardHooker::getInstance();

// Set event handler callback
hooker.setEventHandler([](const KeyboardEvent& event) -> bool
{
    if (event.type == KeyboardEvent::ET_PRESS)
    {
        // Handle key press event
    }
    return true; // Continue propagating event
});

hooker.run();        // Start listening
hooker.stop();       // Stop listening
hooker.isRunning();
```

> **Note**: Do not call `KeyboardHooker` member functions inside the event handler callback.

#### `KeyboardSimulator` — Keyboard Input Simulation

```cpp
using KeyboardSimulator;
using KeyboardKey;

KeyboardSimulator& sim = KeyboardSimulator::getInstance();
sim.initialize();

// Using KeyboardKey enum
sim.pressKey(KBDKEY_A);
sim.releaseKey(KBDKEY_A);
sim.clickKey(KBDKEY_ENTER);    // pressKey + releaseKey

// Using platform native key values
sim.pressKey(0x41u);    // Windows VK_A

// Send key combination (Ctrl+C)
sim.pressKey(KBDKEY_CTRL);
sim.clickKey(KBDKEY_C);
sim.releaseKey(KBDKEY_CTRL);

// Batch send events
KeyboardEvent events[] =
{
    KeyboardEvent::createPressEvent(KBDKEY_A),
    KeyboardEvent::createReleaseEvent(KBDKEY_A)
};
sim.sendEvent(events, 2);

sim.destroy();
```

#### `KeyboardKey` — Key Value Enum

The library defines a cross-platform key value enum `KeyboardKey` that can be converted to and from platform native key values:

```cpp
// KeyboardKey -> platform native key value (Win: VK_*, macOS: kVK_*, Linux: KEY_*)
uint32_t nativeKey = keyboardKeyToNativeKey(KBDKEY_A);

// Platform native key value -> KeyboardKey
KeyboardKey key = keyboardKeyFromNativeKey(0x41u);
```

Common key values:

| Enum | Description |
| ---- | ----------- |
| `KBDKEY_0` ~ `KBDKEY_9` | Number keys |
| `KBDKEY_A` ~ `KBDKEY_Z` | Letter keys |
| `KBDKEY_F1` ~ `KBDKEY_F24` | Function keys |
| `KBDKEY_NUMPAD_0` ~ `KBDKEY_NUMPAD_9` | Numpad digit keys |
| `KBDKEY_CTRL` / `KBDKEY_CTRL_LEFT` / `KBDKEY_CTRL_RIGHT` | Ctrl key |
| `KBDKEY_SHIFT` / `KBDKEY_SHIFT_LEFT` / `KBDKEY_SHIFT_RIGHT` | Shift key |
| `KBDKEY_ALT` / `KBDKEY_ALT_LEFT` / `KBDKEY_ALT_RIGHT` | Alt key (alias `KBDKEY_OPTION`) |
| `KBDKEY_META` / `KBDKEY_META_LEFT` / `KBDKEY_META_RIGHT` | Win/Command key |
| `KBDKEY_ENTER` / `KBDKEY_RETURN` | Enter key |
| `KBDKEY_ESCAPE` / `KBDKEY_ESC` | Escape key |
| `KBDKEY_SPACE` | Space key |
| `KBDKEY_BACKSPACE` | Backspace key |
| `KBDKEY_TAB` | Tab key |
| `KBDKEY_LEFT` / `KBDKEY_RIGHT` / `KBDKEY_UP` / `KBDKEY_DOWN` | Arrow keys |

---

### Mouse Module

#### `MouseHooker` — Mouse Event Listening

```cpp
using MouseHooker;

MouseHooker& hooker = MouseHooker::getInstance();

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
            // event.wheelDelta (unit 120, positive = away from user, negative = toward user)
            break;
        case MouseEvent::ET_DRAG:
            // event.drag.pos, event.drag.button
            break;
        case MouseEvent::ET_PRESS:
        case MouseEvent::ET_RELEASE:
            // event.button
            break;
        default:
            break;
    }

    return true;
});

hooker.run();
hooker.stop();
```

> **Note**: Do not call `MouseHooker` member functions inside the event handler callback.

#### `MouseSimulator` — Mouse Input Simulation

```cpp
using MouseSimulator;
using AbsolutePos;
using RelativePos;
using MouseButton;

MouseSimulator& sim = MouseSimulator::getInstance();
sim.initialize();

// Move mouse (absolute coordinates, based on virtual screen space)
sim.moveTo(AbsolutePos(100, 200));

// Move mouse (relative offset)
sim.moveBy(RelativePos(10, -5));

// Scroll wheel (unit 120, positive = up, negative = down)
sim.wheel(120);     // Scroll up one notch
sim.wheel(-120);    // Scroll down one notch

// Mouse buttons (at current position)
sim.pressButton(MSBTN_LEFT);
sim.releaseButton(MSBTN_LEFT);
sim.clickButton(MSBTN_LEFT);
sim.doubleClickButton(MSBTN_LEFT);

// Mouse operations at a specified position
sim.clickButton(AbsolutePos(500, 300), MSBTN_LEFT);
sim.wheel(AbsolutePos(500, 300), 120);

// Drag (from current position to target position)
sim.dragCombo(AbsolutePos(800, 400));
// Drag (specify start and end positions)
sim.dragCombo(AbsolutePos(100, 100), AbsolutePos(800, 400), MSBTN_LEFT);

sim.destroy();
```

> **macOS Note**: Drag operations must use the `dragCombo()` function or the combination of `pressButton()` + `dragTo()` + `releaseButton()` to produce. `dragTo()` same as `moveTo()` on other platforms.

The coordinate range of the emitted mouse absolute movement event can be obtained through the following method. Coordinates beyond this range will be clamped.

```cpp
// Get the absolute coordinate range for the current environment.
AbsolutePosRange range = MouseSimulator::getAbsoluteMoveRange();
// Windows/macOS: virtual screen space range
// Linux: always {0, 65535, 0, 65535}
```

#### `MouseButton` — Mouse Button Enum

| Enum | Description |
| ---- | ----------- |
| `MSBTN_LEFT` | Left button |
| `MSBTN_RIGHT` | Right button |
| `MSBTN_MIDDLE` | Middle button |
| `MSBTN_BACK` | Back button |
| `MSBTN_FORWARD` | Forward button |

---

## Full Usage Example

### Example: Listen for Keyboard Events and Simulate Mouse Click

```cpp
#include <hidtool/hidtool.hpp>

int main()
{
    auto& kbdHooker = KeyboardHooker::getInstance();

    mouseSimulator.initialize();

    kbdHooker.setEventHandler([](const KeyboardEvent& event) -> bool
    {
        // When F1 is pressed, left-click at (500, 300)
        if (event.type == KeyboardEvent::ET_PRESS &&
            event.nativeKey == keyboardKeyToNativeKey(KBDKEY_F1))
        {
            MouseSimulator::getInstance()
                .clickButton(AbsolutePos(500, 300), MSBTN_LEFT);
        }
        return true;
    });

    kbdHooker.run();

    // Main thread waiting...

    kbdHooker.stop();
    mouseSimulator.destroy();
    return 0;
}
```

## Example subproject

[HID event listener](../example/echo/main.cpp)

> Listen for keyboard and mouse events and print event information.

[Mouse circle](../example//mouse_circle/main.cpp)

> Move the mouse around the circle/ellipse inside the screen as the trajectory.

[Simple clicker](../example/simple_clicker/main.cpp)

> Double-click of the left mouse button at the specified position by pressing the designated key.

## License

This project is open-sourced under the MIT License.

Copyright (c) 2026 頔珞JaderoChan
