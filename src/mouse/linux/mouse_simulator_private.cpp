#include "mouse_simulator_private.hpp"

#include <platforms/linux/input_event_factory.hpp>

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate()
{
    if (!isInitialized_.load())
        return;

    isInitialized_.store(false);
}

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

MouseSimulator::AbsMoveRange MouseSimulatorPrivate::getAbsMoveRange()
{
    MouseSimulator::AbsMoveRange result;

    result.minX = ABS_MIN_X;
    result.maxX = ABS_MAX_X;
    result.minY = ABS_MIN_Y;
    result.maxY = ABS_MAX_Y;

    return result;
}

bool MouseSimulatorPrivate::initialize()
{
    bool expected = false;
    if (!isInitialized_.compare_exchange_strong(expected, true))
        return false;

    if (!mouseUInput_.setup("JaderoChan HIDTOOL Mouse UInput", 0x0310, 0x0002, 0x0001))
    {
        isInitialized_.store(false);
        return false;
    }

    return true;
}

void MouseSimulatorPrivate::destroy()
{
    bool expected = true;
    if (!isInitialized_.compare_exchange_strong(expected, false))
        return;

    mouseUInput_.cleanup();
}

bool MouseSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool MouseSimulatorPrivate::sendEvent(const MouseEvent& event)
{
    switch (event.eventType)
    {
        case MouseEvent::ET_REL_MOVE:
            return moveBy(event.pos.x, event.pos.y);
        case MouseEvent::ET_ABS_MOVE:
            return moveTo(event.pos.x, event.pos.y);
        case MouseEvent::ET_WHEEL:
            return wheel(event.wheelDelta);
        case MouseEvent::ET_PRESS:
            return pressButton(event.button);
        case MouseEvent::ET_RELEASE:
            return releaseButton(event.button);
        default:
            return false;
    }
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    size_t sent = 0;
    for (size_t i = 0; i < count; ++i)
    {
        const auto& event = events[i];
        switch (event.eventType)
        {
            case MouseEvent::ET_REL_MOVE:
                sent += moveBy(event.pos.x, event.pos.y);
                break;
            case MouseEvent::ET_ABS_MOVE:
                sent += moveTo(event.pos.x, event.pos.y);
                break;
            case MouseEvent::ET_WHEEL:
                sent += wheel(event.wheelDelta);
                break;
            case MouseEvent::ET_PRESS:
                sent += pressButton(event.button);
                break;
            case MouseEvent::ET_RELEASE:
                sent += releaseButton(event.button);
                break;
            default:
                continue;
        }
    }

    return sent;
}

bool MouseSimulatorPrivate::moveBy(int32_t dx, int32_t dy)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[3] = {0};

    ies[0].type = EV_REL;
    ies[0].code = REL_X;
    ies[0].value = dx;

    ies[1].type = EV_REL;
    ies[1].code = REL_Y;
    ies[1].value = dy;

    ies[2] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 3);
}

bool MouseSimulatorPrivate::moveTo(int32_t x, int32_t y)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[3] = {0};

    ies[0].type = EV_ABS;
    ies[0].code = ABS_X;
    ies[0].value = x;

    ies[1].type = EV_ABS;
    ies[1].code = ABS_Y;
    ies[1].value = y;

    ies[2] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 3);
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};

    ies[0].type = EV_REL;
    ies[0].code = REL_WHEEL;
    ies[0].value = wheelDelta;

    ies[1] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 2);
}

static inline bool setInputEventButton(struct input_event& ie, MouseButton button)
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

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};

    ies[0].type = EV_KEY;
    ies[0].value = 1;   // 1 is press.
    if (!setInputEventButton(ies[0], button))
        return false;

    ies[1] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 2);
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};

    ies[0].type = EV_KEY;
    ies[0].value = 0;   // 0 is release.
    if (!setInputEventButton(ies[0], button))
        return false;

    ies[1] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 2);
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[4] = {0};

    ies[0].type = EV_KEY;
    ies[0].value = 1;   // 1 is press.
    if (!setInputEventButton(ies[0], button))
        return false;

    ies[1] = createSyncEvent();

    ies[2].type = EV_KEY;
    ies[2].value = 0;   // 0 is release.
    if (!setInputEventButton(ies[2], button))
        return false;

    ies[3] = createSyncEvent();

    return mouseUInput_.sendEvent(ies, 4);
}

} // namespace hidtool
