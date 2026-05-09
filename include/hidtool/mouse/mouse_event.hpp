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
        /**
         * @details 通过 \ref MouseSimulator 发送的此类型事件会将数据钳制在合法范围中，
         * 但 \ref MouseHooker 并不会尝试钳制其接收到的此类型事件。
         * @sa \ref MouseSimulator::getAbsoluteMoveRange()
         */
        ET_ABS_MOVE,
        ET_REL_MOVE,
        ET_WHEEL,
        /**
         * @details 在 **Windows** 和 **Linux** 平台下，此事件永远不会被 \ref MouseHooker 的事件处理程序接收到。
         * 当在上述两个平台下发送此类型的事件时，其等同于绝对移动事件，字段 \ref MouseEvent::drag.button 将被丢弃。
         * 在 **MacOS** 平台下，无论是 \ref MouseHooker 还是 \ref MouseSimulator，都原生支持此类型事件。
         */
        ET_DRAG,
        ET_PRESS,
        ET_RELEASE,
        ET_SLEEP
    };

    constexpr MouseEvent() noexcept
        : sleepMs(0) {}
    constexpr explicit MouseEvent(EventType type) noexcept
        : type(type), sleepMs(0) {}

    /**
     * @defgroup mouse_event_factory 鼠标事件工厂函数
     * @brief 便于构造指定类型的鼠标事件。
     */

    /**
     * @ingroup mouse_event_factory
     *
     * @{
     */

    /** @sa \ref absPos, \ref MouseSimulator::moveTo() */
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

    /** @sa \ref relPos, \ref MouseSimulator::moveBy() */
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

    /** @sa \ref wheelDelta, \ref MouseSimulator::wheel() */
    static MouseEvent createWheelEvent(int32_t wheelDelta) noexcept
    {
        MouseEvent result(ET_WHEEL);
        result.wheelDelta = wheelDelta;
        return result;
    }

    /** @sa \ref MouseSimulator::drag() */
    static MouseEvent createDragEvent(const AbsolutePos& absPos, MouseButton button) noexcept
    {
        MouseEvent result(ET_DRAG);
        result.drag.pos = absPos;
        result.drag.button = button;
        return result;
    }

    /** @sa \ref MouseSimulator::press() */
    static MouseEvent createPressButtonEvent(MouseButton button) noexcept
    {
        MouseEvent result(ET_PRESS);
        result.button = button;
        return result;
    }

    /** @sa \ref MouseSimulator::release() */
    static MouseEvent createReleaseButtonEvent(MouseButton button) noexcept
    {
        MouseEvent result(ET_RELEASE);
        result.button = button;
        return result;
    }

    static MouseEvent createSleepEvent(size_t sleepMs) noexcept
    {
        MouseEvent result(ET_SLEEP);
        result.sleepMs = sleepMs;
        return result;
    }

    /** @} */

    EventType type = ET_NONE;
    union
    {
        /**
         * @note
         * 在 **Windows** 和 **MacOS** 平台下，无论是 \ref MouseHooker 事件处理函数中获得的绝对移动事件还是
         * 通过 \ref MouseSimulator 发送的绝对移动事件，此坐标始终以虚拟屏幕空间范围为基准。
         * 在 **Linux** 平台下，通过 \ref MouseHooker 获得的绝对移动事件坐标范围依赖于设备厂商，
         * 通过 \ref MouseSimulator 发送的绝对移动事件，此坐标在 X 和 Y 轴上始终限定为 `[0, 65535]`。
         * @sa \ref MouseSimulator::getAbsoluteMoveRange()
         */
        AbsolutePos absPos;
        /**
         * @details 由于 **MacOS** 平台下没有原生支持相对移动的 API，
         * 所以在实现中将通过计算后的绝对移动坐标模拟相对移动。
         */
        RelativePos relPos;
        /**
         * @note 单位量为 `120`。值为正时，滚轮朝远离用户的方向滚动；值为负时，滚轮朝靠近用户的方向滚动。
         * @attention 若发送事件的滚动量绝对值小于 `120`，可能不会有效果。
         */
        int32_t wheelDelta;
        struct
        {
            AbsolutePos pos;
            MouseButton button;
        } drag;
        MouseButton button;
        size_t sleepMs;
    };
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_EVENT_HPP
