#ifndef HIDTOOL_KEYBOARD_EVENT_HPP
#define HIDTOOL_KEYBOARD_EVENT_HPP

#include "keyboard_key.hpp"

namespace hidtool
{

struct KeyboardEvent
{
    enum EventType : uint8_t
    {
        ET_PRESS,
        ET_RELEASE
    };

    constexpr explicit KeyboardEvent(EventType eventType) noexcept
        : eventType(eventType) {}
    constexpr KeyboardEvent(EventType eventType, uint32_t nativeKey) noexcept
        : eventType(eventType), nativeKey(nativeKey) {};
    KeyboardEvent(EventType eventType, KeyboardKey key) noexcept
        : eventType(eventType), nativeKey(keyboardKeyToNativeKey(key)) {};

    EventType eventType;
    uint32_t nativeKey = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_EVENT_HPP
