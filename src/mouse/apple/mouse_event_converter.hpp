#ifndef HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
#define HIDTOOL_MOUSE_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>           // CG*
#include <CoreFoundation/CoreFoundation.h>  // CF*

#include <hidtool/mouse/mouse_event.hpp>

namespace hidt
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

[[nodiscard]] inline bool mouseButtonToCGMouseButton(
    MouseButton button, ButtonState state,
    CGEventType& cgEventType, CGMouseButton& cgButton)
{
    switch (button)
    {
        case MSBTN_LEFT:
            switch (state)
            {
                case BS_DOWN: cgEventType = kCGEventLeftMouseDown; break;
                case BS_UP: cgEventType = kCGEventLeftMouseUp; break;
                case BS_DRAGGED: cgEventType = kCGEventLeftMouseDragged; break;
                default: return false;
            }
            cgButton = kCGMouseButtonLeft;
            break;
        case MSBTN_RIGHT:
            switch (state)
            {
                case BS_DOWN: cgEventType = kCGEventRightMouseDown; break;
                case BS_UP: cgEventType = kCGEventRightMouseUp; break;
                case BS_DRAGGED: cgEventType = kCGEventRightMouseDragged; break;
                default: return false;
            }
            cgButton = kCGMouseButtonRight;
            break;
        case MSBTN_MIDDLE:  // Fallthrough
        case MSBTN_BACK:    // Fallthrough
        case MSBTN_FORWARD:
            switch (state)
            {
                case BS_DOWN: cgEventType = kCGEventOtherMouseDown; break;
                case BS_UP: cgEventType = kCGEventOtherMouseUp; break;
                case BS_DRAGGED: cgEventType = kCGEventOtherMouseDragged; break;
                default: return false;
            }

            switch (button)
            {
                case MSBTN_MIDDLE: cgButton = kCGMouseButtonCenter; break;
                // TODO: MacOS下，未来可能弃用前进和后退侧键，因为他们在不同的设备厂商上可能具有不一致的值。
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

[[nodiscard]] inline bool mouseEventToCGEvent(const MouseEvent& event, CGEventRef& cgEvent)
{
    switch (event.type)
    {
        case MouseEvent::ET_ABS_MOVE:
        {
            CGPoint pt = CGPointMake(static_cast<CGFloat>(event.absPos.x), static_cast<CGFloat>(event.absPos.y));
            cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
            break;
        }
        case MouseEvent::ET_REL_MOVE:
        {
            CGPoint pt = getCurrentLocation();
            pt.x += static_cast<CGFloat>(event.relPos.dx);
            pt.y += static_cast<CGFloat>(event.relPos.dy);
            cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventMouseMoved, pt, kCGMouseButtonLeft);
            break;
        }
        case MouseEvent::ET_WHEEL:
        {
            cgEvent = CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 1, event.wheelDelta / 120);
            break;
        }
        case MouseEvent::DRAG:  // Fallthrough
        case MouseEvent::ET_PRESS:
        case MouseEvent::ET_RELEASE:
        {
            CGPoint pt = getCurrentLocation();
            CGEventType cgEventType;
            CGMouseButton cgButton;

            MouseButton button;
            ButtonState state;
            switch (event.type)
            {
                case MouseEvent::ET_DRAG: button = event.dragButton; state = BS_DRAGGED; break;
                case MouseEvent::ET_PRESS: button = event.button; state = BS_DOWN; break;
                case MouseEvent::ET_RELEASE: button = event.button; state = BS_UP; break;
                default: return false;
            }

            if (!mouseButtonToCGMouseButton(button, state, cgEventType, cgButton))
                return false;
            cgEvent = CGEventCreateMouseEvent(nullptr, cgEventType, pt, cgButton);

            break;
        }
        default:
            return false;
    }

    return cgEvent != nullptr;
}

[[nodiscard]] inline bool
mouseEventFromCGEvent(MouseEvent& event, CGEventType cgEventType, const CGEventRef& cgEvent)
{
    switch (cgEventType)
    {
        case kCGEventMouseMoved:
        {
            event.type = MouseEvent::ET_ABS_MOVE;
            CGPoint pt = CGEventGetLocation(cgEvent);
            event.absPos = {static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y)};
            break;
        }
        case kCGEventScrollWheel:
        {
            event.type = MouseEvent::ET_WHEEL;
            int64_t wheelDelta = CGEventGetIntegerValueField(cgEvent, kCGScrollWheelEventDeltaAxis1);
            // 滚轮变化单位量为 120。
            event.wheelDelta = static_cast<int32_t>(wheelDelta * 120);
            break;
        }
        case kCGEventLeftMouseDown:
        {
            event.type = MouseEvent::ET_PRESS;
            event.button = MSBTN_LEFT;
            break;
        }
        case kCGEventLeftMouseUp:
        {
            event.type = MouseEvent::ET_RELEASE;
            event.button = MSBTN_LEFT;
            break;
        }
        case kCGEventRightMouseDown:
        {
            event.type = MouseEvent::ET_PRESS;
            event.button = MSBTN_RIGHT;
            break;
        }
        case kCGEventRightMouseUp:
        {
            event.type = MouseEvent::ET_RELEASE;
            event.button = MSBTN_RIGHT;
            break;
        }
        case kCGEventOtherMouseDown:
        {
            event.type = MouseEvent::ET_PRESS;

            int64_t button = CGEventGetIntegerValueField(cgEvent, kCGMouseEventButtonNumber);
            switch (button)
            {
                case kCGMouseButtonCenter: event.button = MSBTN_MIDDLE; break;
                // TODO: MacOS下，未来可能弃用前进和后退侧键，因为他们在不同的设备厂商上可能具有不一致的值。
                case 3: event.button = MSBTN_BACK; break;       // 3 is back button.
                case 4: event.button = MSBTN_FORWARD; break;    // 4 is forward button.
                default: return false;
            }

            break;
        }
        case kCGEventOtherMouseUp:
        {
            event.type = MouseEvent::ET_RELEASE;

            int64_t button = CGEventGetIntegerValueField(cgEvent, kCGMouseEventButtonNumber);
            switch (button)
            {
                case kCGMouseButtonCenter: event.button = MSBTN_MIDDLE; break;
                // TODO: MacOS下，未来可能弃用前进和后退侧键，因为他们在不同的设备厂商上可能具有不一致的值。
                case 3: event.button = MSBTN_BACK; break;       // 3 is back button.
                case 4: event.button = MSBTN_FORWARD; break;    // 4 is forward button.
                default: return false;
            }

            break;
        }
        // TODO：可能在未来更改
        case kCGEventLeftMouseDragged:  // Fallthrough
        case kCGEventRightMouseDragged: // Fallthrough
        case kCGEventOtherMouseDragged:
        {
            event.type = MouseEvent::ET_DRAG;

            CGPoint pt = CGEventGetLocation(cgEvent);
            event.dragPos = {static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y)};

            switch (cgEventType)
            {
                case kCGEventLeftMouseDragged: event.dragButton = MSBTN_LEFT; break;
                case kCGEventRightMouseDragged: event.dragButton = MSBTN_RIGHT; break;
                case kCGEventOtherMouseDragged:
                {
                    int64_t button = CGEventGetIntegerValueField(cgEvent, kCGMouseEventButtonNumber);
                    switch (button)
                    {
                        case kCGMouseButtonCenter: event.button = MSBTN_MIDDLE; break;
                        // TODO: MacOS下，未来可能弃用前进和后退侧键，因为他们在不同的设备厂商上可能具有不一致的值。
                        case 3: event.button = MSBTN_BACK; break;       // 3 is back button.
                        case 4: event.button = MSBTN_FORWARD; break;    // 4 is forward button.
                        default: return false;
                    }
                }
                default: return false;
            }

            break;
        }
        default:
            return false;
    }

    return true;
}

} // namespace hidt

#endif // !HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
