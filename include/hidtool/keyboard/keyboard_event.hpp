#ifndef HIDTOOL_KEYBOARD_EVENT_HPP
#define HIDTOOL_KEYBOARD_EVENT_HPP

#include "keyboard_key.hpp"

namespace hidt
{

struct KeyboardEvent
{
    enum EventType : uint8_t
    {
        ET_NONE,
        ET_PRESS,
        ET_RELEASE,
        ET_SLEEP
    };

    constexpr KeyboardEvent() noexcept
        : sleepMs(0) {}
    constexpr explicit KeyboardEvent(EventType type) noexcept
        : type(type), sleepMs(0) {}

    /**
     * @defgroup keyboard_event_factory 键盘事件工厂函数
     * @brief 便于构造指定类型的键盘事件。
     */

    /**
     * @ingroup keyboard_event_factory
     *
     * @{
     */

    static KeyboardEvent createPressEvent(uint32_t nativeKey) noexcept
    {
        KeyboardEvent result(ET_PRESS);
        result.nativeKey = nativeKey;
        return result;
    }

    static KeyboardEvent createPressEvent(KeyboardKey key) noexcept
    {
        KeyboardEvent result(ET_PRESS);
        result.nativeKey = keyboardKeyToNativeKey(key);
        return result;
    }

    static KeyboardEvent createReleaseEvent(uint32_t nativeKey) noexcept
    {
        KeyboardEvent result(ET_RELEASE);
        result.nativeKey = nativeKey;
        return result;
    }

    static KeyboardEvent createReleaseEvent(KeyboardKey key) noexcept
    {
        KeyboardEvent result(ET_RELEASE);
        result.nativeKey = keyboardKeyToNativeKey(key);
        return result;
    }

    static KeyboardEvent createSleepEvent(size_t sleepMs) noexcept
    {
        KeyboardEvent result(ET_SLEEP);
        result.sleepMs = sleepMs;
        return result;
    }

    /** @} */

    EventType type = ET_NONE;
    union
    {
        uint32_t nativeKey;
        size_t sleepMs;
    };
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_EVENT_HPP
