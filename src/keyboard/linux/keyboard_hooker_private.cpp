#include "keyboard_hooker_private.hpp"

#include <unistd.h>     // read

#include <sys/ioctl.h>  // ioctl()

#include <linux/input-event-codes.h>

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
    unsigned long evBits = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), &evBits) == -1)
        return false;
    if ((evBits & (1 << EV_KEY)) == 0)
        return false;

    unsigned char keyBits[KEY_MAX / 8 + 1];
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits) == -1)
        return false;

    uint32_t checkedKeys[] = {
        KEY_0, KEY_A, KEY_SPACE, KEY_ESC
    };
    size_t checkedCount = sizeof(checkedKeys) / sizeof(uint32_t);

    for (size_t i = 0; i < checkedCount; ++i)
    {
        if ((keyBits[checkedKeys[i] / 8] & (1u << (checkedKeys[i] % 8))) == 0)
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
            auto eventHandler = getEventHandler<KeyboardEventHandler>();
            if (eventHandler && ie.type == EV_KEY)
            {
                KeyboardEvent event;
                event.eventType = (ie.value == 0 ? KeyboardEvent::ET_RELEASE : KeyboardEvent::ET_PRESS);
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
