#include "mouse_hooker_private.hpp"

#include <unordered_map>    // unordered_map

#include <unistd.h>         // read
#include <sys/ioctl.h>      // ioctl()

#include <linux/input-event-codes.h>    // EV_*, ABS_*, REL_*, BTN_*

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
            break;
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

    // 由于 Linux 的移动事件可能只上报单轴移动事件，通过此结构体保留双轴位置信息。
    struct AbsState
    {
        int32_t x = 0;
        int32_t y = 0;
        bool hasX = false;
        bool hasY = false;
    };
    // 通过 `unordered_map` 存储不同设备的双轴位置信息。
    static std::unordered_map<int, AbsState> absStatesByFd;
    // 获取当前设备的双轴位置信息。
    auto& absState = absStatesByFd[fd];

    MouseEvent event;
    struct input_event ie;
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
                        // 更新当前设备的双轴位置信息。
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

                    // 仅当双轴位置都已赋值时才上报事件。
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
                            // 滚轮变化单位量为 120。
                            event.wheelDelta = static_cast<int32_t>(ie.value) * 120;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case EV_KEY:
                {
                    event.eventType = (ie.value == 1 ? MouseEvent::ET_PRESS : MouseEvent::ET_RELEASE);
                    setMouseEventButton(event, ie.code);
                    break;
                }
                case EV_SYN:
                {
                    // 由于鼠标事件可能在 `SYN_REPORT` 之前有多个事件，
                    // 所以需要在接收到 `SYN_REPORT` 事件时才调用事件处理程序。
                    if (ie.code == SYN_REPORT)
                    {
                        if (eventHandler && event.eventType != MouseEvent::ET_NONE)
                            eventHandler(event);
                    }

                    // 重置事件
                    event = MouseEvent();
                    break;
                }
                default:
                {
                    // 重置事件
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
