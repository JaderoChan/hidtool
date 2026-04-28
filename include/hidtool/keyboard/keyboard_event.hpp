#ifndef HIDTOOL_KEYBOARD_EVENT_HPP
#define HIDTOOL_KEYBOARD_EVENT_HPP

#include "keyboard_key.hpp"

namespace hidtool
{

struct KeyboardEvent
{
    enum EventType : uint8_t
    {
        ET_NONE,
        ET_PRESS,
        ET_RELEASE
    };

    constexpr KeyboardEvent() noexcept = default;
    constexpr explicit KeyboardEvent(EventType type) noexcept
        : type(type) {}
    constexpr KeyboardEvent(EventType type, uint32_t nativeKey) noexcept
        : type(type), nativeKey(nativeKey) {};
    KeyboardEvent(EventType type, KeyboardKey key) noexcept
        : type(type), nativeKey(keyboardKeyToNativeKey(key)) {};

    EventType type = ET_NONE;
    uint32_t nativeKey = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_EVENT_HPP
