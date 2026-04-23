#ifndef HIDTOOL_HIDEVENT_HPP
#define HIDTOOL_HIDEVENT_HPP

#include "defines.hpp"
#include "hidtype.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_event.hpp>
#endif // HIDTOOL_HAS_KEYBOARD

#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_event.hpp>
#endif // HIDTOOL_HAS_MOUSE

namespace hidtool
{

struct HidEvent
{
    HidType hidType;
    union
    {
    #ifdef HIDTOOL_HAS_KEYBOARD
        KeyboardEvent keyboardEvent;
    #endif // HIDTOOL_HAS_KEYBOARD
    #ifdef HIDTOOL_HAS_MOUSE
        MouseEvent mouseEvent;
    #endif // HIDTOOL_HAS_MOUSE
    };
};

} // namespace hidtool

#endif // !HIDTOOL_HIDEVENT_HPP
