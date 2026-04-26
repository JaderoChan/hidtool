#include "mouse_hooker_private.hpp"

#include <unordered_map>

#include <unistd.h>     // read

#include <sys/ioctl.h>  // ioctl()

#include <linux/input-event-codes.h>

namespace hidtool
{

MouseHookerPrivate::~MouseHookerPrivate() { stop(); }

MouseHookerPrivate& MouseHookerPrivate::getInstance()
{
    static MouseHookerPrivate instance;
    return instance;
}

bool MouseHookerPrivate::setEventHandler(const MouseEventHandler& eventHandler)
{
    return HookerPrivate::setEventHandler<MouseEventHandler>(eventHandler);
}

bool MouseHookerPrivate::isAccessDevice(int fd)
{
    unsigned long evBits = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), &evBits) == -1)
        return false;

    if (((evBits & (1 << EV_ABS)) == 0 && (evBits & (1 << EV_REL)) == 0) || (evBits & (1 << EV_KEY)) == 0)
        return false;

    // Check absolute move.

    // Check relative move.

    // Check mouse wheel.
//     unsigned char relBits[REL_MAX / 8 + 1];
//     if (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(relBits)), relBits) == -1)
//         return false;
//
//     if ((relBits[REL_WHEEL / 8] & (1u << (REL_WHEEL % 8))) == 0)
//         return false;

    // Check mouse buttons.
    unsigned char keyBits[KEY_MAX / 8 + 1];
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits) == -1)
        return false;

    uint32_t checkedKeys[] = {BTN_LEFT, BTN_RIGHT};
    size_t checkedCount = sizeof(checkedKeys) / sizeof(uint32_t);

    for (size_t i = 0; i < checkedCount; ++i)
    {
        if ((keyBits[checkedKeys[i] / 8] & (1u << (checkedKeys[i] % 8))) == 0)
            return false;
    }

    return true;
}

static inline void setMouseEventButton(MouseEvent& event, unsigned short inputEventCode)
{
    switch (inputEventCode)
    {
        case BTN_LEFT:
            event.button = MouseButton::MSBTN_LEFT;
            break;
        case BTN_RIGHT:
            event.button = MouseButton::MSBTN_RIGHT;
            break;
        case BTN_MIDDLE:
            event.button = MouseButton::MSBTN_MIDDLE;
            break;
        case BTN_BACK:
            event.button = MouseButton::MSBTN_BACK;
            break;;
        case BTN_FORWARD:
            event.button = MouseButton::MSBTN_FORWARD;
            break;
        default:
            break;
    }
}

void MouseHookerPrivate::handleInputEvent(int fd)
{
    auto eventHandler = getEventHandler<MouseEventHandler>();

    struct AbsState
    {
        int32_t x = 0;
        int32_t y = 0;
        bool hasX = false;
        bool hasY = false;
    };
    static std::unordered_map<int, AbsState> absStatesByFd;
    auto& absState = absStatesByFd[fd];

    struct input_event ie;
    MouseEvent event;
    while (true)
    {
        ssize_t n = read(fd, &ie, sizeof(struct input_event));

        if (n == sizeof(struct input_event))
        {
            switch (ie.type)
            {
                case EV_ABS:
                {
                    switch (ie.code)
                    {
                        case ABS_X:
                            absState.x = static_cast<int32_t>(ie.value);
                            absState.hasX = true;
                            break;
                        case ABS_Y:
                            absState.y = static_cast<int32_t>(ie.value);
                            absState.hasY = true;
                            break;
                        default:
                            break;
                    }

                    if (absState.hasX && absState.hasY)
                    {
                        event.eventType = MouseEvent::ET_ABS_MOVE;
                        event.pos.x = absState.x;
                        event.pos.y = absState.y;
                    }

                    break;
                }
                case EV_REL:
                {
                    switch (ie.code)
                    {
                        case REL_X:
                            event.eventType = MouseEvent::ET_REL_MOVE;
                            event.pos.x = static_cast<int32_t>(ie.value);
                            break;
                        case REL_Y:
                            event.eventType = MouseEvent::ET_REL_MOVE;
                            event.pos.y = static_cast<int32_t>(ie.value);
                            break;
                        case REL_WHEEL:
                            event.eventType = MouseEvent::ET_WHEEL;
                            event.wheelDelta = static_cast<int32_t>(ie.value);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case EV_KEY:
                {
                    event.eventType = (ie.value == 0 ? MouseEvent::ET_RELEASE : MouseEvent::ET_PRESS);
                    setMouseEventButton(event, ie.code);
                    break;
                }
                case EV_SYN:
                {
                    if (ie.code == SYN_REPORT)
                    {
                        if (event.eventType != MouseEvent::ET_NONE && eventHandler)
                            eventHandler(event);
                    }

                    event = MouseEvent();
                    break;
                }
                default:
                {
                    event = MouseEvent();
                    break;
                }
            }

            continue;
        }

        if (n == -1 && errno == EINTR)
            continue;

        break;
    }
}

} // namespace hidtool
