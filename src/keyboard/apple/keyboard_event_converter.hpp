#ifndef HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
#define HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>

#include <hidtool/keyboard/keyboard_event.hpp>

namespace hidtool
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
        case KeyboardEvent::ET_PRESSED:
            cgEvent = CGEventCreateKeyboardEvent(nullptr, keyCode, true);
            break;
        case KeyboardEvent::ET_RELEASED:
            cgEvent = CGEventCreateKeyboardEvent(nullptr, keyCode, false);
            break;
        default:
            cgEvent = nullptr;
            return false;
    }

    return cgEvent != nullptr;
}

[[nodiscard]] inline bool keyboardEventFromCGEvent(KeyboardEvent& event, CGEventType cgEventType, const CGEventRef& cgEvent)
{
    CGKeyCode keycode = CGEventGetIntegerValueField(cgEvent, kCGKeyboardEventKeycode);

    switch (cgEventType)
    {
        case kCGEventKeyDown:
            event.type = KeyboardEvent::ET_PRESSED;
            break;
        case kCGEventKeyUp:
            event.type = KeyboardEvent::ET_RELEASED;
            break;
        case kCGEventFlagsChanged:
        {
            CGEventFlags mask = keycodeToModifierMask(keycode);
            if (mask != 0)
            {
                CGEventFlags flags = CGEventGetFlags(cgEvent);
                event.type = (flags & mask) ? KeyboardEvent::ET_PRESSED : KeyboardEvent::ET_RELEASED;
            }
            break;
        }
        default:
            return false;
    }

    event.nativeKey = static_cast<uint32_t>(keycode);

    return true;
}

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
