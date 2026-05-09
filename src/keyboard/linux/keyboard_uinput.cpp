#include "keyboard_uinput.hpp"

#include <sys/ioctl.h>  // ioctl()

#include <linux/uinput.h>               // uinput...
#include <linux/input-event-codes.h>    // EV_*, KEY_*

namespace hidt
{

bool KeyboardUInput::setupUInputFd(int fd)
{
    // 键盘 UInput 需要配置 EV_KEY 和 EV_SYN 事件。
    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1 || ioctl(fd, UI_SET_EVBIT, EV_SYN) == -1)
        return false;

    // TODO：仅配置需要的值。
    // 配置所有键值。（未来可能修改）
    for (size_t i = 0; i <= KEY_MAX; ++i)
    {
        if (ioctl(fd, UI_SET_KEYBIT, i) == -1)
            return false;
    }

    return true;
}

} // namespace hidt
