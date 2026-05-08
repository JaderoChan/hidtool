#include "mouse_simulator_private.hpp"

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
        if (mouseEventToCGEvent(events[i], cgEvent))
        {
            CGEventPost(kCGHIDEventTap, cgEvent);
            CFRelease(cgEvent);
            sent++;
        }
    }

    return sent;
}

// TODO：下列函数，考虑是否统一通过sendEvent函数对其进行简化？

bool MouseSimulatorPrivate::moveTo(const AbsolutePos& absPos)
{
    if (!isInitialized_.load())
        return false;

    CGPoint pt = CGPointMake(static_cast<CGFloat>(absPos.x), static_cast<CGFloat>(absPos.y));

    CGEventRef cgEvent = nullptr;
    cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::moveBy(const RelativePos& relPos)
{
    if (!isInitialized_.load())
        return false;

    CGPoint pt = getCurrentLocation();
    pt.x += static_cast<CGFloat>(relPos.dx);
    pt.y += static_cast<CGFloat>(relPos.dy);

    CGEventRef cgEvent = nullptr;
    cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    CGEventRef cgEvent = nullptr;
    cgEvent = CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 1, wheelDelta);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::dragTo(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    CGEventType cgEventType;
    CGMouseButton cgButton;
    if (!mouseButtonToCGMouseButton(button, BS_DRAGGED, cgEventType, cgButton))
        return false;

    CGPoint pt = CGPointMake(static_cast<CGFloat>(absPos.x), static_cast<CGFloat>(absPos.y));
    CGEventRef cgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    CGPoint pt = getCurrentLocation();
    AbsolutePos pos = {static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y)};
    return pressButton(pos, button);
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    CGPoint pt = getCurrentLocation();
    AbsolutePos pos = {static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y)};
    return releaseButton(pos, button);
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    CGPoint pt = getCurrentLocation();
    AbsolutePos pos = {static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y)};
    return clickButton(pos, button);
}

bool MouseSimulatorPrivate::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{
    size_t sent = 0;

    sent += moveTo(absPos);
    sent += wheel(wheelDelta);

    return sent == 2;
}

bool MouseSimulatorPrivate::pressButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    CGEventType cgEventType;
    CGMouseButton cgButton;
    if (!mouseButtonToCGMouseButton(button, BS_DOWN, cgEventType, cgButton))
        return false;

    CGPoint pt = CGPointMake(static_cast<CGFloat>(absPos.x), static_cast<CGFloat>(absPos.y));
    CGEventRef cgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::releaseButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    CGEventType cgEventType;
    CGMouseButton cgButton;
    if (!mouseButtonToCGMouseButton(button, BS_UP, cgEventType, cgButton))
        return false;

    CGPoint pt = CGPointMake(static_cast<CGFloat>(absPos.x), static_cast<CGFloat>(absPos.y));
    CGEventRef cgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool MouseSimulatorPrivate::clickButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    size_t sent = 0;

    CGPoint pt = CGPointMake(static_cast<CGFloat>(absPos.x), static_cast<CGFloat>(absPos.y));

    CGEventType cgEventType;
    CGMouseButton cgButton;

    if (!mouseButtonToCGMouseButton(button, BS_DOWN, cgEventType, cgButton))
        return false;
    CGEventRef pressCgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);

    if (!mouseButtonToCGMouseButton(button, BS_UP, cgEventType, cgButton))
        return false;
    CGEventRef releaseCgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);

    if (pressCgEvent)
    {
        CGEventPost(kCGHIDEventTap, pressCgEvent);
        CFRelease(pressCgEvent);
        sent++;
    }

    if (releaseCgEvent)
    {
        CGEventPost(kCGHIDEventTap, releaseCgEvent);
        CFRelease(releaseCgEvent);
        sent++;
    }

    return sent == 2;
}

bool MouseSimulatorPrivate::dragCombo(const AbsolutePos& endPos, MouseButton button)
{
    CGPoint startPt = getCurrentLocation();
    AbsolutePos startPos = {static_cast<int32_t>(startPt.x), static_cast<int32_t>(startPt.y)};
    return dragCombo(startPos, endPos, button);
}

bool MouseSimulatorPrivate::dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    size_t sent = 0;

    CGPoint startPt = CGPointMake(static_cast<CGFloat>(startPos.x), static_cast<CGFloat>(startPos.y));
    CGPoint endPt = CGPointMake(static_cast<CGFloat>(endPos.x), static_cast<CGFloat>(endPos.y));

    CGEventType cgEventType;
    CGMouseButton cgButton;

    if (!mouseButtonToCGMouseButton(button, BS_DOWN, cgEventType, cgButton))
        return false;
    CGEventRef pressCgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, startPt, cgButton);

    if (!mouseButtonToCGMouseButton(button, BS_DRAGGED, cgEventType, cgButton))
        return false;
    CGEventRef dragCgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, endPt, cgButton);

    if (!mouseButtonToCGMouseButton(button, BS_UP, cgEventType, cgButton))
        return false;
    CGEventRef releaseCgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, endPt, cgButton);

    if (pressCgEvent)
    {
        CGEventPost(kCGHIDEventTap, pressCgEvent);
        CFRelease(pressCgEvent);
        sent++;
    }

    if (dragCgEvent)
    {
        CGEventPost(kCGHIDEventTap, dragCgEvent);
        CFRelease(dragCgEvent);
        sent++;
    }

    if (releaseCgEvent)
    {
        CGEventPost(kCGHIDEventTap, releaseCgEvent);
        CFRelease(releaseCgEvent);
        sent++;
    }

    return sent == 3;
}

} // namespace hidt
