#include "keyboard_uinput.hpp"

#include <cstdio>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/uinput.h>
#include <linux/input-event-codes.h>

namespace hidtool
{

bool KeyboardUInput::setupUInputFd(int fd)
{
    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1 || ioctl(fd, UI_SET_EVBIT, EV_SYN) == -1)
        return false;

    for (size_t i = 0; i <= KEY_MAX; ++i)
    {
        if (ioctl(fd, UI_SET_KEYBIT, i) == -1)
            return false;
    }

    if (ioctl(fd, UI_DEV_CREATE) == -1)
        return false;

    return true;
}

} // namespace hidtool
