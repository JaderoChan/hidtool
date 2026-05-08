#include "mouse_simulator_private.hpp"

#include <chrono>   // chrono
#include <thread>   // this_thread
#include <vector>   // vector

#include <CoreFoundation/CoreFoundation.h>  // CF*

#include "mouse_event_converter.hpp"

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
    uint32_t displayCount = 0;
    CGError err = CGGetActiveDisplayList(0, nullptr, &displayCount);
    if (err != kCGErrorSuccess || displayCount == 0)
        return AbsolutePosRange();

    std::vector<CGDirectDisplayID> displays(displayCount);
    err = CGGetActiveDisplayList(displayCount, displays.data(), &displayCount);
    if (err != kCGErrorSuccess)
        return AbsolutePosRange();

    CGRect unionRect = CGDisplayBounds(displays[0]);
    for (const auto& id : displays)
        unionRect = CGRectUnion(unionRect, CGDisplayBounds(id));

    AbsolutePosRange result;

    result.minX = unionRect.origin.x;
    result.minY = unionRect.origin.y;
    result.maxX = unionRect.origin.x + unionRect.size.width;
    result.maxY = unionRect.origin.y + unionRect.size.height;

    return result;
}

bool MouseSimulatorPrivate::initialize()
{
    // 无实际作用，仅与其他子模块保持一致性。`destroy()` 同理。
    bool expected = false;
    return isInitialized_.compare_exchange_strong(expected, true);
}

void MouseSimulatorPrivate::destroy()
{
    bool expected = true;
    isInitialized_.compare_exchange_strong(expected, false);
}

bool MouseSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool MouseSimulatorPrivate::sendEvent(const MouseEvent& event)
{
    if (!isInitialized_.load())
        return false;

    if (event.type == MouseEvent::ET_SLEEP)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
        return true;
    }

    CGEventRef cgEvent = nullptr;
    if (mouseEventToCGEvent(event, cgEvent))
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    size_t sent = 0;

    CGEventRef cgEvent = nullptr;
    for (size_t i = 0; i < count; ++i)
    {
        if (event.type == MouseEvent::ET_SLEEP)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
            sent++;
            continue;
        }

        if (mouseEventToCGEvent(events[i], cgEvent))
        {
            CGEventPost(kCGHIDEventTap, cgEvent);
            CFRelease(cgEvent);
            sent++;
        }
    }

    return sent;
}

} // namespace hidt
