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
 * @defgroup HidEvents HID 设备事件
 */

namespace hidt
{

/**
 * @brief HID 设备事件
 * @ingroup HidEvents
 */
struct HidEvent
{
    /** @brief HID 设备事件类型 */
    enum EventType : uint8_t
    {
        ET_NONE,        ///< 空事件，事件默认类型
        ET_KEYBOARD,    ///< 键盘事件
        ET_MOUSE,       ///< 鼠标事件
        ET_SLEEP        ///< 睡眠事件
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
        size_t sleepMs; ///< 睡眠时间，单位为毫秒
    };
};

} // namespace hidt

#endif // !HIDTOOL_HIDEVENT_HPP
