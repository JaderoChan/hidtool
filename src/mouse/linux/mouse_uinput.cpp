#include "mouse_uinput.hpp"

#include <cstdio>
#include <cstdint>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/uinput.h>
#include <linux/input-event-codes.h>

namespace hidtool
{

bool MouseUInput::setupUInputFd(int fd)
{
    if (ioctl(fd, UI_SET_EVBIT, EV_REL) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_ABS) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1 ||
        ioctl(fd, UI_SET_EVBIT, EV_SYN) == -1)
        return false;

    if (ioctl(fd, UI_SET_RELBIT, REL_X) == -1 ||
        ioctl(fd, UI_SET_RELBIT, REL_Y) == -1 ||
        ioctl(fd, UI_SET_RELBIT, REL_WHEEL) == -1)
        return false;

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

    if (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_FORWARD) == -1 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_BACK) == -1)
        return false;

    return true;
}

} // namespace hidtool
