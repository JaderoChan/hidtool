#ifndef HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
#define HIDTOOL_MOUSE_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>           // CG*
#include <CoreFoundation/CoreFoundation.h>  // CF*

#include <hidtool/mouse/mouse_event.hpp>

namespace hidtool
{

enum ButtonState : uint8_t
{
    BS_DOWN,
    BS_UP,
    BS_DRAGGED
};

inline CGPoint getCurrentLocation()
{
    CGEventRef cur = CGEventCreate(nullptr);
    CGPoint pt = CGEventGetLocation(cur);
    CFRelease(cur);
    return pt;
}

inline bool mouseButtonToCGMouseButton(
    MouseButton button, ButtonState state,
    CGEventType& cgEt, CGMouseButton& cgButton)
{
    switch (button)
    {
        case MSBTN_LEFT:
            switch (state)
            {
                case BS_DOWN: cgEt = kCGEventLeftMouseDown; break;
                case BS_UP: cgEt = kCGEventLeftMouseUp; break;
                case BS_DRAGGED: cgEt = kCGEventLeftMouseDragged; break;
                default: return false;
            }
            cgButton = kCGMouseButtonLeft;
            break;
        case MSBTN_RIGHT:
            switch (state)
            {
                case BS_DOWN: cgEt = kCGEventRightMouseDown; break;
                case BS_UP: cgEt = kCGEventRightMouseUp; break;
                case BS_DRAGGED: cgEt = kCGEventRightMouseDragged; break;
                default: return false;
            }
            cgButton = kCGMouseButtonRight;
            break;
        case MSBTN_MIDDLE:  // Fallthrough
        case MSBTN_BACK:    // Fallthrough
        case MSBTN_FORWARD:
            switch (state)
            {
                case BS_DOWN: cgEt = kCGEventOtherMouseDown; break;
                case BS_UP: cgEt = kCGEventOtherMouseUp; break;
                case BS_DRAGGED: cgEt = kCGEventOtherMouseDragged; break;
                default: return false;
            }

            switch (button)
            {
                case MSBTN_MIDDLE: cgButton = kCGMouseButtonCenter; break;
                // MacOS下，未来可能弃用前进和后退侧键，因为他们在不同的设备厂商上可能具有不一致的值。
                case MSBTN_BACK: cgButton = static_cast<CGMouseButton>(3); break;       // 3 is back button.
                case MSBTN_FORWARD: cgButton = static_cast<CGMouseButton>(4); break;    // 4 is forward button.
                default: return false;
            }

            break;
        default:
            return false;
    }

    return true;
}

inline bool mouseEventToCGEvent(const MouseEvent& event, CGEventRef& cgEvent)
{
    switch (event.type)
    {
        case MouseEvent::ET_ABS_MOVE:
        {
            CGPoint pt = CGPointMake(event.absPos.x, event.absPos.y);
            cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
            break;
        }
        case MouseEvent::ET_REL_MOVE:
        {
            CGPoint pt = getCurrentLocation();
            pt.x += event.relPos.dx;
            pt.y += event.relPos.dy;
            cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
            break;
        }
        case MouseEvent::ET_WHEEL:
        {
            cgEvent = CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 1, event.wheelDelta / 120);
            break;
        }
        case MouseEvent::ET_PRESS:  // Fallthrough
        case MouseEvent::ET_RELEASE:
        {
            CGPoint pt = getCurrentLocation();
            CGEventType cgEventType;
            CGMouseButton cgButton;

            ButtonState state = (event.type == MouseEvent::ET_PRESS ? BS_DOWN : BS_UP);
            if (!mouseButtonToCGMouseButton(event.button, state, cgEventType, cgButton))
                return false;
            cgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);

            break;
        }
        default:
            return false;
    }

    return cgEvent != nullptr;
}

inline bool mouseEventFromCGEvent(MouseEvent& event, CGEventType cgEventType, const CGEventRef& cgEvent)
{

}

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
