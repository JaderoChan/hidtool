#ifndef HIDTOOL_INPUT_EVENT_FACTORY_HPP
#define HIDTOOL_INPUT_EVENT_FACTORY_HPP

#include <linux/input.h>

#include <hidtool/mouse/mouse_button.hpp>
#include <hidtool/mouse/mouse_pos.hpp>

namespace hidtool
{

inline void setAbsoluteMoveInputEvent(struct input_event& ieX, struct input_event& ieY, const AbsolutePos& absPos)
{
    // Clamp
    signed int x = static_cast<signed int>(absPos.x);
    signed int y = static_cast<signed int>(absPos.y);
    x = (x < 0 ? 0 : (x > ABS_MAX_X ? ABS_MAX_X : x));
    y = (y < 0 ? 0 : (y > ABS_MAX_Y ? ABS_MAX_Y : y));

    ieX.type = EV_ABS;
    ieX.code = ABS_X;
    ieX.value = x;

    ieY.type = EV_ABS;
    ieY.code = ABS_Y;
    ieY.value = y;
}

inline void setRelativeMoveInputEvent(struct input_event& ieX, struct input_event& ieY, const RelativePos& relPos)
{
    ieX.type = EV_REL;
    ieX.code = REL_X;
    ieX.value = static_cast<signed int>(relPos.dx);

    ieY.type = EV_REL;
    ieY.code = REL_Y;
    ieY.value = static_cast<signed int>(relPos.dy);
}

inline void setWheelInputEvent(struct input_event& ie, int32_t wheelDelta)
{
    ie.type = EV_REL;
    ie.code = REL_WHEEL;
    ie.value = static_cast<signed int>(wheelDelta / 120);
}

[[nodiscard]] inline bool setInputEventButton(struct input_event& ie, MouseButton button)
{
    switch (button)
    {
        case MouseButton::MSBTN_LEFT:
            ie.code = BTN_LEFT;
            break;
        case MouseButton::MSBTN_RIGHT:
            ie.code = BTN_RIGHT;
            break;
        case MouseButton::MSBTN_MIDDLE:
            ie.code = BTN_MIDDLE;
            break;
        case MouseButton::MSBTN_FORWARD:
            ie.code = BTN_FORWARD;
            break;
        case MouseButton::MSBTN_BACK:
            ie.code = BTN_BACK;
            break;
        default:
            return false;
    }

    return true;
}

[[nodiscard]] inline bool setPressButtonInputEvent(struct input_event& ie, MouseButton button)
{
    ie.type = EV_KEY;
    ie.value = 1;   // 1 is press.
    return setInputEventButton(ie, button);
}

[[nodiscard]] inline bool setReleaseButtonInputEvent(struct input_event& ie, MouseButton button)
{
    ie.type = EV_KEY;
    ie.value = 0;   // 0 is release.
    return setInputEventButton(ie, button);
}

} // namespace hidtool

#endif // !HIDTOOL_INPUT_EVENT_FACTORY_HPP
