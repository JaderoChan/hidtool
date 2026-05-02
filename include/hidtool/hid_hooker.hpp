#ifndef HIDTOOL_HID_HOOKER_HPP
#define HIDTOOL_HID_HOOKER_HPP

#include "defines.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_hooker.hpp>
#endif // HIDTOOL_HAS_KEYBOARD

#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_hooker.hpp>
#endif // HIDTOOL_HAS_MOUSE

/**
 * @defgroup Hooker HID 事件监听器
 */

namespace hidtool
{

}

#endif // !HIDTOOL_HID_HOOKER_HPP
