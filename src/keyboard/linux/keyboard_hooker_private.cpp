#include "keyboard_hooker_private.hpp"

#include <unistd.h>     // read
#include <sys/ioctl.h>  // ioctl()

#include <linux/input-event-codes.h>    // EV_*, KEY_*

namespace hidtool
{

KeyboardHookerPrivate::~KeyboardHookerPrivate() { stop(); }

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::setEventHandler(const KeyboardEventHandler& eventHandler)
{
    return HookerPrivate::setEventHandler<KeyboardEventHandler>(eventHandler);
}

bool KeyboardHookerPrivate::isAccessDevice(int fd)
{
    // 获取输入设备具备的事件。
    unsigned long evBits = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), &evBits) == -1)
        return false;

    // 必须含有 `EV_KEY` 事件。
    if ((evBits & (1u << EV_KEY)) == 0)
        return false;

    // 获取输入设备具备的键值。
    uint8_t keyBits[KEY_MAX / 8 + 1];
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits) == -1)
        return false;

    // TODO：使检查更加精确。
    // 待检查的键值。（未来可能修改）
    uint32_t checkedKeys[] = {KEY_0, KEY_A, KEY_SPACE, KEY_ESC};
    size_t checkedCount = sizeof(checkedKeys) / sizeof(uint32_t);

    static auto hasKey = [](uint8_t* keyBits, uint32_t key)
    { return (keyBits[key / 8] & (1u << (key % 8))) != 0; };

    // 判断此设备是否具备以上定义的键值。
    for (size_t i = 0; i < checkedCount; ++i)
    {
        if (!hasKey(keyBits, checkedKeys[i]))
            return false;
    }
    return true;
}

void KeyboardHookerPrivate::handleInputEvent(int fd)
{
    struct input_event ie;
    while (true)
    {
        ssize_t n = read(fd, &ie, sizeof(struct input_event));

        if (n == sizeof(struct input_event))
        {
            // 处理输入事件。
            auto eventHandler = getEventHandler<KeyboardEventHandler>();
            // 由于键盘事件始终以 `{EV_KEY, EV_SYN}` 对形式出现，
            // 所以不对 `EV_SYN` 进行判断，而在接收到 `EV_KEY` 事件时直接调用事件处理程序。
            if (eventHandler && ie.type == EV_KEY)
            {
                KeyboardEvent event;
                event.eventType = (ie.value == 1 ? KeyboardEvent::ET_PRESS : KeyboardEvent::ET_RELEASE);
                event.nativeKey = static_cast<int32_t>(ie.code);

                eventHandler(event);
            }

            continue;
        }

        if (n == -1 && errno == EINTR)
            continue;

        break;
    }
}

} // namespace hidtool
