#ifndef HIDTOOL_MOUSE_EVENT_HPP
#define HIDTOOL_MOUSE_EVENT_HPP

#include "mouse_button.hpp"

namespace hidtool
{

struct MouseEvent
{
    enum EventType : uint8_t
    {
        ET_NONE,
        ET_ABS_MOVE,    ///< Absolute move event.
        ET_REL_MOVE,    ///< Relative move event.
        ET_WHEEL,
        ET_PRESS,
        ET_RELEASE
    };

    constexpr MouseEvent() noexcept
        : pos{0, 0} {}

    constexpr explicit MouseEvent(EventType eventType) noexcept
        : eventType(eventType), pos{0, 0} {}

    static MouseEvent createAbsMoveEvent(int32_t x, int32_t y) noexcept
    {
        MouseEvent result(ET_ABS_MOVE);
        result.pos = {x, y};
        return result;
    }

    static MouseEvent createRelMoveEvent(int32_t x, int32_t y) noexcept
    {
        MouseEvent result(ET_REL_MOVE);
        result.pos = {x, y};
        return result;
    }

    static MouseEvent createWheelEvent(int32_t wheelDelta)
    {
        MouseEvent result(ET_WHEEL);
        result.wheelDelta = wheelDelta;
        return result;
    }

    static MouseEvent createPressButtonEvent(MouseButton button)
    {
        MouseEvent result(ET_PRESS);
        result.button = button;
        return result;
    }

    static MouseEvent createReleaseButtonEvent(MouseButton button)
    {
        MouseEvent result(ET_RELEASE);
        result.button = button;
        return result;
    }

    EventType eventType = ET_NONE;
    union
    {
        /**
         * @note 在 **Windows** 和 **MacOS** 平台下，无论是 `MouseHooker` 事件处理函数中获得的绝对移动事件还是
         * 通过 `MouseSimulator` 发送的绝对移动事件，此坐标始终以虚拟屏幕空间大小为参考。
         * 而对于 **Linux** 平台下的绝对移动事件，此坐标在 X 和 Y 轴上始终限定为 `[0, 65535]`。
         * 此外，在 **Linux** 下发送超过此坐标限制的绝对移动事件时，我们不会尝试将其钳制。
         */
        struct
        {
            int32_t x;
            int32_t y;
        } pos;
        /**
         * @note 此值的单位量是平台相关的，所以不要在跨平台程序中使用相同的度量以期望其在不同平台上的行为一致。
         * （未来可能会进行优化）
         */
        int32_t wheelDelta;
        MouseButton button;
    };
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_EVENT_HPP
