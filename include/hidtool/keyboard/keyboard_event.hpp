#ifndef HIDTOOL_KEYBOARD_EVENT_HPP
#define HIDTOOL_KEYBOARD_EVENT_HPP

#include "keyboard_key.hpp"

namespace hidt
{

/** @brief 键盘事件 */
struct KeyboardEvent
{
    /** @brief 键盘事件类型 */
    enum EventType : uint8_t
    {
        ET_NONE,    ///< 空事件，事件默认类型
        ET_PRESS,   ///< 键盘按下事件
        ET_RELEASE, ///< 键盘释放事件
        ET_SLEEP    ///< 睡眠事件
    };

    constexpr KeyboardEvent() noexcept
        : sleepMs(0) {}
    constexpr explicit KeyboardEvent(EventType keyboardEventType) noexcept
        : type(keyboardEventType), sleepMs(0) {}

    /**
     * @defgroup KeyboardEventFactory 键盘事件工厂函数
     * @brief 便于构造指定类型的键盘事件。
     */

    /**
     * @ingroup KeyboardEventFactory
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
        uint32_t nativeKey; ///< 系统原生键值
        size_t sleepMs;     ///< 睡眠时间，单位为毫秒
    };
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_EVENT_HPP
