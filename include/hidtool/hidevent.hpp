#ifndef HIDTOOL_HIDEVENT_HPP
#define HIDTOOL_HIDEVENT_HPP

#include "defines.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_event.hpp>
#endif // HIDTOOL_HAS_KEYBOARD

#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_event.hpp>
#endif // HIDTOOL_HAS_MOUSE

/**
 * @defgroup hid_events HID events
 */

namespace hidt
{

/**
 * @brief HID event
 * @ingroup hid_events
 */
struct HidEvent
{
    /** @brief HID event types */
    enum EventType : uint8_t
    {
        ET_NONE,        ///< Invalid event, default event type
        ET_KEYBOARD,    ///< Keyboard event
        ET_MOUSE,       ///< Mouse event
        ET_SLEEP        ///< Sleep event
    };

    constexpr HidEvent() noexcept
        : sleepMs(0) {}
    constexpr HidEvent(EventType hidEventType) noexcept
        : type(hidEventType), sleepMs(0) {}

    EventType type = ET_NONE;
    union
    {
    #ifdef HIDTOOL_HAS_KEYBOARD
        KeyboardEvent keyboardEvent;
    #endif // HIDTOOL_HAS_KEYBOARD
    #ifdef HIDTOOL_HAS_MOUSE
        MouseEvent mouseEvent;
    #endif // HIDTOOL_HAS_MOUSE
        uint64_t sleepMs;   ///< Sleep time in millisecond
    };
};

} // namespace hidt

#endif // !HIDTOOL_HIDEVENT_HPP
