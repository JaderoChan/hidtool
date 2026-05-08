#include "mouse_simulator_private.hpp"

#include <chrono>   // chrono
#include <thread>   // this_thread

#include <platforms/linux/sync_input_event_factory.hpp>
#include "input_event_factory.hpp"

namespace hidt
{

MouseSimulatorPrivate::~MouseSimulatorPrivate() { destroy(); }

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

AbsolutePosRange MouseSimulatorPrivate::getAbsoluteMoveRange()
{
    AbsolutePosRange result;

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
    switch (event.type)
    {
        case MouseEvent::ET_ABS_MOVE:   // Fallthrough
        case MouseEvent::ET_DRAG:
            setAbsoluteMoveInputEvent(ies[0], ies[1], event.absPos);
            setSyncReportEvent(ies[2]);
            return mouseUInput_.sendEvent(ies, 3);
        case MouseEvent::ET_REL_MOVE:
            setRelativeMoveInputEvent(ies[0], ies[1], event.relPos);
            setSyncReportEvent(ies[2]);
            return mouseUInput_.sendEvent(ies, 3);
        case MouseEvent::ET_WHEEL:
            setWheelInputEvent(ies[0], event.wheelDelta);
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        case MouseEvent::ET_PRESS:
            if (!setPressButtonInputEvent(ies[0], event.button))
                return false;
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        case MouseEvent::ET_RELEASE:
            if (setReleaseButtonInputEvent(ies[0], event.button))
                return false;
            setSyncReportEvent(ies[1]);
            return mouseUInput_.sendEvent(ies, 2);
        case MouseEvent::ET_SLEEP:
            std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
            return true;
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
        switch (event.type)
        {
            case MouseEvent::ET_ABS_MOVE:   // Fallthrough
            case MouseEvent::ET_DRAG:
                setAbsoluteMoveInputEvent(ies[0], ies[1], event.absPos);
                setSyncReportEvent(ies[2]);
                sent += mouseUInput_.sendEvent(ies, 3);
                break;
            case MouseEvent::ET_REL_MOVE:
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
                if (!setPressButtonInputEvent(ies[0], event.button))
                    continue;
                setSyncReportEvent(ies[1]);
                sent += mouseUInput_.sendEvent(ies, 2);
                break;
            case MouseEvent::ET_RELEASE:
                if (!setReleaseButtonInputEvent(ies[0], event.button))
                    continue;
                setSyncReportEvent(ies[1]);
                sent += mouseUInput_.sendEvent(ies, 2);
                break;
            case MouseEvent::ET_SLEEP:
                std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
                sent++;
                continue;
            default:
                continue;
        }
    }

    return sent;
}

} // namespace hidt
