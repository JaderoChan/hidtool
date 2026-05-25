#ifndef HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
#define HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP

#include <Carbon/Carbon.h>          // kVK_*
#include <CoreGraphics/CGEvent.h>   // kCG*, CG*

#include <hidtool/keyboard/keyboard_event.hpp>

namespace hidt
{

inline CGEventFlags keycodeToModifierMask(CGKeyCode keyCode)
{
    switch (keyCode)
    {
        case kVK_Shift:
        case kVK_RightShift:
            return kCGEventFlagMaskShift;
        case kVK_Control:
        case kVK_RightControl:
            return kCGEventFlagMaskControl;
        case kVK_Option:
        case kVK_RightOption:
            return kCGEventFlagMaskAlternate;
        case kVK_Command:
        case kVK_RightCommand:
            return kCGEventFlagMaskCommand;
        case kVK_CapsLock:
            return kCGEventFlagMaskAlphaShift;
        case kVK_Function:
            return kCGEventFlagMaskSecondaryFn;
        default:
            return 0;
    }
}

[[nodiscard]] inline bool keyboardEventToCGEvent(const KeyboardEvent& event, CGEventRef& cgEvent)
{
    CGKeyCode keyCode = static_cast<CGKeyCode>(event.nativeKey);
    switch (event.type)
    {
        case KeyboardEvent::ET_PRESS:
            cgEvent = CGEventCreateKeyboardEvent(nullptr, keyCode, true);
            break;
        case KeyboardEvent::ET_RELEASE:
            cgEvent = CGEventCreateKeyboardEvent(nullptr, keyCode, false);
            break;
        default:
            cgEvent = nullptr;
            return false;
    }

    return cgEvent != nullptr;
}

[[nodiscard]] inline bool
keyboardEventFromCGEvent(KeyboardEvent& event, CGEventType cgEventType, const CGEventRef& cgEvent)
{
    int64_t keycode = CGEventGetIntegerValueField(cgEvent, kCGKeyboardEventKeycode);

    switch (cgEventType)
    {
        case kCGEventKeyDown:
            event.type = KeyboardEvent::ET_PRESS;
            break;
        case kCGEventKeyUp:
            event.type = KeyboardEvent::ET_RELEASE;
            break;
        case kCGEventFlagsChanged:
        {
            CGEventFlags mask = keycodeToModifierMask(static_cast<CGKeyCode>(keycode));
            if (mask != 0)
            {
                CGEventFlags flags = CGEventGetFlags(cgEvent);
                event.type = (flags & mask) ? KeyboardEvent::ET_PRESS : KeyboardEvent::ET_RELEASE;
            }
            break;
        }
        default:
            return false;
    }

    event.nativeKey = static_cast<int32_t>(keycode);
    event.timestamp = CGEventGetTimestamp(cgEvent);

    return true;
}

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
