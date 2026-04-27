#include "mouse_simulator_private.hpp"

#include <platforms/linux/sync_input_event_factory.hpp>
#include "input_event_factory.hpp"

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate() { destroy(); }

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

bool MouseSimulatorPrivate::initialize()
{
    bool expected = false;
    if (!isInitialized_.compare_exchange_strong(expected, true))
        return false;

    // 配置鼠标 UInput。
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

    // 清理鼠标 UInput。
    mouseUInput_.cleanup();
}

bool MouseSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool MouseSimulatorPrivate::sendEvent(const MouseEvent& event)
{
    if (!isInitialized_.load())
        return 0;

    struct input_event ies[3] = {0};
    switch (event.eventType)
    {
        case MouseEvent::ET_REL_MOVE:
            setAbsoluteMoveInputEvent(ies[0], ies[1], event.absPos);
            setSyncReportEvent(ies[2]);
            return mouseUInput_.sendEvent(ies, 3);
        case MouseEvent::ET_ABS_MOVE:
            setRelativeMoveInputEvent(ies[0], ies[1], event.relPos);
            setSyncReportEvent(ies[2]);
            return mouseUInput_.sendEvent(ies, 3);
        case MouseEvent::ET_WHEEL:
            setWheelInputEvent(ies[0], event.wheelDelta);
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        case MouseEvent::ET_PRESS:
            setPressButtonInputEvent(ies[0], event.button);
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        case MouseEvent::ET_RELEASE:
            setReleaseButtonInputEvent(ies[0], event.button);
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        default:
            return false;
    }
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    size_t sent = 0;

    struct input_event ies[3] = {0};
    for (size_t i = 0; i < count; ++i)
    {
        const auto& event = events[i];
        switch (event.eventType)
        {
            case MouseEvent::ET_REL_MOVE:
                setAbsoluteMoveInputEvent(ies[0], ies[1], event.absPos);
                setSyncReportEvent(ies[2]);
                sent += mouseUInput_.sendEvent(ies, 3);
                break;
            case MouseEvent::ET_ABS_MOVE:
                setRelativeMoveInputEvent(ies[0], ies[1], event.relPos);
                setSyncReportEvent(ies[2]);
                sent += mouseUInput_.sendEvent(ies, 3);
                break;
            case MouseEvent::ET_WHEEL:
                setWheelInputEvent(ies[0], event.wheelDelta);
                setSyncReportEvent(ies[1]);
                sent += mouseUInput_.sendEvent(ies, 2);
                break;
            case MouseEvent::ET_PRESS:
                setPressButtonInputEvent(ies[0], event.button);
                setSyncReportEvent(ies[1]);
                sent += mouseUInput_.sendEvent(ies, 2);
                break;
            case MouseEvent::ET_RELEASE:
                setReleaseButtonInputEvent(ies[0], event.button);
                setSyncReportEvent(ies[1]);
                sent += mouseUInput_.sendEvent(ies, 2);
                break;
            default:
                continue;
        }
    }

    return sent;
}

bool MouseSimulatorPrivate::moveTo(const AbsolutePos& absPos)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[3] = {0};
    setAbsoluteMoveInputEvent(ies[0], ies[1], absPos);
    setSyncReportEvent(ies[2]);

    return mouseUInput_.sendEvent(ies, 3);
}

bool MouseSimulatorPrivate::moveBy(const RelativePos& relPos)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[3] = {0};
    setRelativeMoveInputEvent(ies[0], ies[1], relPos);
    setSyncReportEvent(ies[2]);

    return mouseUInput_.sendEvent(ies, 3);
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};

    setWheelInputEvent(ies[0], wheelDelta);
    setSyncReportEvent(ies[1]);

    return mouseUInput_.sendEvent(ies, 2);
}

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};
    if (!setPressButtonInputEvent(ies[0], button))
        return false;
    setSyncReportEvent(ies[1]);

    return mouseUInput_.sendEvent(ies, 2);
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[2] = {0};
    if (!setReleaseButtonInputEvent(ies[0], button))
        return false;
    setSyncReportEvent(ies[1]);

    return mouseUInput_.sendEvent(ies, 2);
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[4] = {0};
    if (!setPressButtonInputEvent(ies[0], button))
        return false;
    setSyncReportEvent(ies[1]);
    if (!setReleaseButtonInputEvent(ies[2], button))
        return false;
    setSyncReportEvent(ies[3]);

    return mouseUInput_.sendEvent(ies, 4);
}

bool MouseSimulatorPrivate::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[5] = {0};

    setAbsoluteMoveInputEvent(ies[0], ies[1], absPos);
    setSyncReportEvent(ies[2]);

    setWheelInputEvent(ies[3], wheelDelta);
    setSyncReportEvent(ies[4]);

    return mouseUInput_.sendEvent(ies, 5);
}

bool MouseSimulatorPrivate::pressButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[5] = {0};

    setAbsoluteMoveInputEvent(ies[0], ies[1], absPos);
    setSyncReportEvent(ies[2]);

    if (!setPressButtonInputEvent(ies[3], button))
        return false;
    setSyncReportEvent(ies[4]);

    return mouseUInput_.sendEvent(ies, 5);
}

bool MouseSimulatorPrivate::releaseButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[5] = {0};

    setAbsoluteMoveInputEvent(ies[0], ies[1], absPos);
    setSyncReportEvent(ies[2]);

    if (!setReleaseButtonInputEvent(ies[3], button))
        return false;
    setSyncReportEvent(ies[4]);

    return mouseUInput_.sendEvent(ies, 5);
}

bool MouseSimulatorPrivate::clickButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[7] = {0};

    setAbsoluteMoveInputEvent(ies[0], ies[1], absPos);
    setSyncReportEvent(ies[2]);

    if (!setPressButtonInputEvent(ies[3], button))
        return false;
    setSyncReportEvent(ies[4]);

    if (!setReleaseButtonInputEvent(ies[5], button))
        return false;
    setSyncReportEvent(ies[6]);

    return mouseUInput_.sendEvent(ies, 7);
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[7] = {0};

    if (!setPressButtonInputEvent(ies[0], button))
        return false;
    setSyncReportEvent(ies[1]);

    setAbsoluteMoveInputEvent(ies[2], ies[3], endPos);
    setSyncReportEvent(ies[4]);

    if (!setReleaseButtonInputEvent(ies[5], button))
        return false;
    setSyncReportEvent(ies[6]);

    return mouseUInput_.sendEvent(ies, 7);
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[10] = {0};

    setAbsoluteMoveInputEvent(ies[0], ies[1], startPos);
    setSyncReportEvent(ies[2]);

    if (!setPressButtonInputEvent(ies[3], button))
        return false;
    setSyncReportEvent(ies[4]);

    setAbsoluteMoveInputEvent(ies[5], ies[6], endPos);
    setSyncReportEvent(ies[7]);

    if (!setReleaseButtonInputEvent(ies[8], button))
        return false;
    setSyncReportEvent(ies[9]);

    return mouseUInput_.sendEvent(ies, 10);
}

} // namespace hidtool
