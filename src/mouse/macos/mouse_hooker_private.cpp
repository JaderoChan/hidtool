#include "mouse_hooker_private.hpp"

#include "mouse_event_converter.hpp"

namespace hidt
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

CGEventMask MouseHookerPrivate::getCGEventMask() const
{
    return CGEventMaskBit(kCGEventMouseMoved) |
        CGEventMaskBit(kCGEventScrollWheel) |
        CGEventMaskBit(kCGEventLeftMouseDown) |
        CGEventMaskBit(kCGEventLeftMouseUp) |
        CGEventMaskBit(kCGEventLeftMouseDragged) |
        CGEventMaskBit(kCGEventRightMouseDown) |
        CGEventMaskBit(kCGEventRightMouseUp) |
        CGEventMaskBit(kCGEventRightMouseDragged) |
        CGEventMaskBit(kCGEventOtherMouseDown) |
        CGEventMaskBit(kCGEventOtherMouseUp) |
        CGEventMaskBit(kCGEventOtherMouseDragged);
}

CGEventTapCallBack MouseHookerPrivate::getCGEventTapCallback() const
{
    return &mouseTapCallback;
}

CGEventRef MouseHookerPrivate::mouseTapCallback(
    CGEventTapProxy proxy, CGEventType cgEventType, CGEventRef cgEvent, void* data)
{
    static MouseHookerPrivate& hooker = getInstance();

    MouseEvent event;
    if (mouseEventFromCGEvent(event, cgEventType, cgEvent))
    {
        // 取得事件处理程序。
        auto eventHandler = hooker.getEventHandler<MouseEventHandler>();
        // 如果事件处理程序返回 false，则返回 nullptr 以阻断事件传播。
        if (eventHandler && !eventHandler(event))
            return nullptr;
    }

    return cgEvent;
}

} // namespace hidt
