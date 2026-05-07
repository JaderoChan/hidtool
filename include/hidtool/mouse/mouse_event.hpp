#ifndef HIDTOOL_MOUSE_EVENT_HPP
#define HIDTOOL_MOUSE_EVENT_HPP

#include "mouse_button.hpp"
#include "mouse_pos.hpp"

namespace hidt
{

struct MouseEvent
{
    enum EventType : uint8_t
    {
        ET_NONE,
        ET_ABS_MOVE,
        ET_REL_MOVE,
        ET_WHEEL,
        /**
         * @details 在 **Windows** 和 **Linux** 平台下，此事件永远不会被 `MouseHooker` 的事件处理程序接收到。
         * 当在上述两个平台下发送此类型的事件时，其等同于绝对移动事件与鼠标按键事件的组合。在 **MacOS** 平台下，
         * 无论是 `MouseHooker` 还是 `MouseSimulator`，都原生支持此类型事件。
         */
        ET_DRAG,
        ET_PRESS,
        ET_RELEASE
    };

    constexpr MouseEvent() noexcept
        : absPos() {}

    constexpr explicit MouseEvent(EventType type) noexcept
        : type(type), absPos() {}

    /** @sa \ref `absPos` */
    static MouseEvent createAbsMoveEvent(int32_t x, int32_t y) noexcept
    {
        MouseEvent result(ET_ABS_MOVE);
        result.absPos = {x, y};
        return result;
    }

    /** @overload */
    static MouseEvent createAbsMoveEvent(const AbsolutePos& absPos) noexcept
    {
        MouseEvent result(ET_ABS_MOVE);
        result.absPos = absPos;
        return result;
    }

    /** @sa \ref `relPos` */
    static MouseEvent createRelMoveEvent(int32_t dx, int32_t dy) noexcept
    {
        MouseEvent result(ET_REL_MOVE);
        result.relPos = {dx, dy};
        return result;
    }

    /** @overload */
    static MouseEvent createRelMoveEvent(const RelativePos& relPos) noexcept
    {
        MouseEvent result(ET_REL_MOVE);
        result.relPos = relPos;
        return result;
    }

    /** @sa \ref `wheelDelta`, \ref `MouseSimulator::wheel()` */
    static MouseEvent createWheelEvent(int32_t wheelDelta)
    {
        MouseEvent result(ET_WHEEL);
        result.wheelDelta = wheelDelta;
        return result;
    }

    static MouseEvent createDragEvent(const AbsolutePos& absPos) noexcept
    {
        MouseEvent result(ET_DRAG);
        result.absPos = absPos;
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

    EventType type = ET_NONE;
    union
    {
        /**
         * @note
         * 在 **Windows** 和 **MacOS** 平台下，无论是 `MouseHooker` 事件处理函数中获得的绝对移动事件还是
         * 通过 `MouseSimulator` 发送的绝对移动事件，此坐标始终以虚拟屏幕空间范围为基准。
         * 在 **Linux** 平台下，通过 `MouseSimulator` 发送的绝对移动事件，此坐标在 X 和 Y 轴上始终限定为 `[0, 65535]`。
         * @sa \ref `getAbsolutePosRange()'
         */
        AbsolutePos absPos;
        /**
         * @details 由于 **MacOS** 平台下没有原生支持相对移动的 API，
         * 所以在实现中将利用鼠标当前位置与相对移动值的和，通过绝对移动模拟相对移动。
         */
        RelativePos relPos;
        /** @note 单位量为 `120`。值为正时，滚轮朝远离用户的方向滚动；值为负时，滚轮朝靠近用户的方向滚动。 */
        int32_t wheelDelta;
        struct
        {
            AbsolutePos dragPos;
            MouseButton dragButton;
        };
        MouseButton button;
    };
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_EVENT_HPP
