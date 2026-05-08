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

namespace hidt
{

struct HidEvent
{
    constexpr HidEvent() noexcept
        : sleepMs(0) {}
    constexpr HidEvent(HidType hidType) noexcept
        : hidType(hidType), sleepMs(0) {}

    HidType hidType = HIDTYPE_NONE;
    union
    {
        uint32_t sleepMs;
    #ifdef HIDTOOL_HAS_KEYBOARD
        KeyboardEvent keyboardEvent;
    #endif // HIDTOOL_HAS_KEYBOARD
    #ifdef HIDTOOL_HAS_MOUSE
        MouseEvent mouseEvent;
    #endif // HIDTOOL_HAS_MOUSE
    };
};

} // namespace hidt

#endif // !HIDTOOL_HIDEVENT_HPP
