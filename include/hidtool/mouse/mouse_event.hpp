#ifndef HIDTOOL_MOUSE_EVENT_HPP
#define HIDTOOL_MOUSE_EVENT_HPP

#include "mouse_button.hpp"
#include "mouse_pos.hpp"

namespace hidt
{

/**
 * @brief Mouse event
 * @ingroup hid_events
 */
struct MouseEvent
{
    /** @brief Mouse event types */
    enum EventType : uint8_t
    {
        ET_NONE,        ///< Invalid event, default event type

        /**
         * @brief Mouse absolute movement event
         * @note This type of event sent via \ref MouseSimulator will clamp the data within the valid range.
         * But \ref MouseHooker does not attempt to clamp this type of event it receives.
         * @sa \ref MouseSimulator::getAbsoluteMoveRange()
         */
        ET_ABS_MOVE,

        ET_REL_MOVE,    ///< Mouse relative movement event
        ET_WHEEL,       ///< Mouse wheel event

        /**
         * @brief Mouse drag event
         * @note On **Windows** and **Linux** platforms,
         * this type of event will never be received by the event handler of \ref MouseHooker.
         * When sending this type of event on the two platforms mentioned above,
         * it is equivalent to an absolute move event, and the field \ref MouseEvent::drag.button will be discarded.
         * @note On the **MacOS** platform,
         * both \ref MouseHooker and \ref MouseSimulator natively support this type of event.
         */
        ET_DRAG,

        ET_PRESS,       ///< Mouse button press event
        ET_RELEASE,     ///< Mouse button release event
        ET_SLEEP        ///< Sleep event
    };

    constexpr MouseEvent() noexcept
        : sleepMs(0) {}
    constexpr explicit MouseEvent(EventType mouseEventType) noexcept
        : type(mouseEventType), sleepMs(0) {}

    /**
     * @defgroup mouse_event_factory Mouse event factory function
     * @brief Facilitates the construction of specified types of mouse events.
     */

    /**
     * @ingroup mouse_event_factory
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
         * @brief Absolute movement event coordinates
         * @note
         * On **Windows** and **MacOS** platforms,
         * whether it is the absolute movement event obtained in the \ref MouseHooker event handler or
         * the absolute movement event sent through \ref MouseSimulator, this coordinate is always
         * based on the virtual screen space range.
         * @note
         * On the **Linux** platform, the range of absolute movement event coordinates
         * obtained through \ref MouseHooker depends on the device manufacturer.
         * Absolute movement events sent via \ref MouseSimulator are always constrained to
         * `[0, 65535]` on the X and Y axes.
         * @sa \ref MouseSimulator::getAbsoluteMoveRange()
         */
        AbsolutePos absPos;

        /**
         * @brief Relative movement event coordinates
         * @note Since the **MacOS** platform does not natively support relative movement APIs,
         * the implementation simulates relative movement through calculated absolute movement coordinates.
         */
        RelativePos relPos;

        /**
         * @brief Mouse wheel event scroll amount
         * @note The unit amount is `120`.
         * @note When the value is positive, the scroll wheel moves away from the user;
         * when the value is negative, the scroll wheel moves toward the user.
         * @attention If the absolute value of the scroll amount for the dispatched event is less than `120`,
         * it may not have any effect.
         */
        int32_t wheelDelta;

        struct
        {
            AbsolutePos pos;    ///< Drag event mouse absolute movement coordinates
            MouseButton button; ///< Drag event mouse button
        } drag; ///< Mouse drag event data
        MouseButton button;     ///< Mouse button
        size_t sleepMs;         ///< Sleep time in millisecond
    };
    uint64_t timestamp = 0; ///< Event timestamp in nanoseconds
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_EVENT_HPP
