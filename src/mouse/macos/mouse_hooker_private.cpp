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

bool MouseHookerPrivate::isButtonPressed(MouseButton button)
{
    CGMouseButton cgButton;
    switch (button)
    {
        case MSBTN_LEFT:    cgButton = kCGMouseButtonLeft;            break;
        case MSBTN_RIGHT:   cgButton = kCGMouseButtonRight;           break;
        case MSBTN_MIDDLE:  cgButton = kCGMouseButtonCenter;          break;
        case MSBTN_BACK:    cgButton = static_cast<CGMouseButton>(3); break;
        case MSBTN_FORWARD: cgButton = static_cast<CGMouseButton>(4); break;
        default: return false;
    }
    return CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState, cgButton);
}

AbsolutePos MouseHookerPrivate::getCursorPos()
{
    CGEventRef event = CGEventCreate(nullptr);
    CGPoint pt = CGEventGetLocation(event);
    CFRelease(event);
    return AbsolutePos(static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y));
}

bool MouseHookerPrivate::setEventHandler(MouseEventHandler eventHandler, void* userData)
{
    return HookerPrivate::setEventHandler<MouseEventHandler>(eventHandler, userData);
}

bool MouseHookerPrivate::setUserData(void* userData)
{
    return HookerPrivate::setUserData(userData);
}

CGEventMask MouseHookerPrivate::getCGEventMask() const
{
    return
        CGEventMaskBit(kCGEventMouseMoved) |
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
        if (eventHandler && !eventHandler(event, hooker.getUserData()))
            return nullptr;
    }

    return cgEvent;
}

} // namespace hidt
