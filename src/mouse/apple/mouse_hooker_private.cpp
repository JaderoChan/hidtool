#include "mouse_hooker_private.hpp"

#include "mouse_event_converter.hpp"

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

CGEventMask MouseHookerPrivate::getCGEventMask() const
{
    // TODO：处理Dragged事件？
    return CGEventMaskBit(kCGEventMouseMoved) |
        CGEventMaskBit(kCGEventScrollWheel) |
        CGEventMaskBit(kCGEventLeftMouseDown) |
        CGEventMaskBit(kCGEventLeftMouseUp) |
        CGEventMaskBit(kCGEventRightMouseDown) |
        CGEventMaskBit(kCGEventRightMouseUp) |
        CGEventMaskBit(kCGEventOtherMouseDown) |
        CGEventMaskBit(kCGEventOtherMouseUp);

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
        // 如果事件处理程序返回 `false`，返回 `nullptr` 以阻断事件传播。
        if (eventHandler && !eventHandler(event))
            return nullptr;
    }

    return cgEvent;
}

} // namespace hidtool
