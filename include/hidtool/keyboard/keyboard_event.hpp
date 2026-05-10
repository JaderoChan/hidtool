#ifndef HIDTOOL_KEYBOARD_EVENT_HPP
#define HIDTOOL_KEYBOARD_EVENT_HPP

#include "keyboard_key.hpp"

namespace hidt
{

/**
 * @brief Keyboard event
 * @ingroup hid_events
 */
struct KeyboardEvent
{
    /** @brief Keyboard event types */
    enum EventType : uint8_t
    {
        ET_NONE,    ///< Invalid event, default event type
        ET_PRESS,   ///< Keyboard key press event
        ET_RELEASE, ///< Keyboard key release event
        ET_SLEEP    ///< Sleep event
    };

    constexpr KeyboardEvent() noexcept
        : sleepMs(0) {}
    constexpr explicit KeyboardEvent(EventType keyboardEventType) noexcept
        : type(keyboardEventType), sleepMs(0) {}

    /**
     * @defgroup keyboard_event_factory Keyboard event factory function
     * @brief Facilitates the construction of specified types of keyboard events.
     */

    /**
     * @ingroup keyboard_event_factory
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
        uint32_t nativeKey; ///< Platform native keyboard key
        size_t sleepMs;     ///< Sleep time in millisecond
    };
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_EVENT_HPP
