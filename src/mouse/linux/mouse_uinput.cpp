#include "mouse_uinput.hpp"

#include <sys/ioctl.h>  // ioctl()

#include <linux/uinput.h>               // uinput ...
#include <linux/input-event-codes.h>    // EV_*, ABS_*, REL_*, BTN_*

namespace hidt
{

bool MouseUInput::setupUInputFd(int fd)
{
    // 鼠标 UInput 需要配置 EV_ABS、EV_REL、EV_KEY 和 EV_SYN 事件。
    if (ioctl(fd, UI_SET_EVBIT, EV_ABS) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_REL) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_SYN) == -1)
        return false;

    // 配置 ABS_X 和 ABS_Y 绝对移动事件。
    if (ioctl(fd, UI_SET_ABSBIT, ABS_X) == -1 ||
        ioctl(fd, UI_SET_ABSBIT, ABS_Y) == -1)
        return false;

    struct uinput_abs_setup absSetupX = {0};
    absSetupX.code = ABS_X;
    absSetupX.absinfo.value = 0;
    absSetupX.absinfo.minimum = ABS_MIN_X;
    absSetupX.absinfo.maximum = ABS_MAX_X;
    if (ioctl(fd, UI_ABS_SETUP, &absSetupX) == -1)
        return false;

    struct uinput_abs_setup absSetupY = {0};
    absSetupY.code = ABS_Y;
    absSetupY.absinfo.value = 0;
    absSetupY.absinfo.minimum = ABS_MIN_Y;
    absSetupY.absinfo.maximum = ABS_MAX_Y;
    if (ioctl(fd, UI_ABS_SETUP, &absSetupY) == -1)
        return false;

    // 配置 REL_X 和 REL_Y 相对移动事件。
    if (ioctl(fd, UI_SET_RELBIT, REL_X) == -1 ||
        ioctl(fd, UI_SET_RELBIT, REL_Y) == -1)
        return false;

    // 配置 REL_WHEEL 滚轮事件。
    if (ioctl(fd, UI_SET_RELBIT, REL_WHEEL) == -1)
        return false;

    // 配置鼠标按键。
    if (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_FORWARD) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_BACK) == -1)
        return false;

    return true;
}

} // namespace hidt
