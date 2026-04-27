#ifndef HIDTOOL_MOUSE_EVENT_HPP
#define HIDTOOL_MOUSE_EVENT_HPP

#include "mouse_button.hpp"
#include "mouse_pos.hpp"

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
        : absPos() {}

    constexpr explicit MouseEvent(EventType eventType) noexcept
        : eventType(eventType), absPos() {}

    static MouseEvent createAbsMoveEvent(int32_t x, int32_t y) noexcept
    {
        MouseEvent result(ET_ABS_MOVE);
        result.absPos = {x, y};
        return result;
    }

    static MouseEvent createRelMoveEvent(int32_t dx, int32_t dy) noexcept
    {
        MouseEvent result(ET_REL_MOVE);
        result.relPos = {dx, dy};
        return result;
    }

    /** @sa `wheelDelta` `MouseSimulator::wheel()` */
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
         * @note
         * 在 **Windows** 和 **MacOS** 平台下，无论是 `MouseHooker` 事件处理函数中获得的绝对移动事件还是
         * 通过 `MouseSimulator` 发送的绝对移动事件，此坐标始终以虚拟屏幕空间范围为基准。
         * 在 **Linux** 平台下，通过 `MouseSimulator` 发送的绝对移动事件，此坐标在 X 和 Y 轴上始终限定为 `[0, 65535]`。
         * @sa `getAbsolutePosRange()'
         */
        AbsolutePos absPos;
        RelativePos relPos;
        /** @note 单位量为 `120`。值为正时，滚轮朝远离用户的方向滚动；值为负时，滚轮朝靠近用户的方向滚动。 */
        int32_t wheelDelta;
        MouseButton button;
    };
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_EVENT_HPP
