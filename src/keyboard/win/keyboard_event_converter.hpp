#ifndef HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
#define HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP

#include <windows.h>

#include <hidtool/keyboard/keyboard_event.hpp>

namespace hidtool
{

[[nodiscard]] inline bool keyboardEventToInput(const KeyboardEvent& event, INPUT& input)
{
    switch (event.eventType)
    {
        case KeyboardEvent::ET_PRESS:
            input.ki.dwFlags = 0;
            break;
        case KeyboardEvent::ET_RELEASE:
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        default:
            return false;
    }

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(event.nativeKey);

    return true;
}

[[nodiscard]] inline bool keyboardEventFromParam(KeyboardEvent& event, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* kbdllhs = reinterpret_cast<KBDLLHOOKSTRUCT*>(static_cast<intptr_t>(lParam));
    if (!kbdllhs)
        return false;

    switch (wParam)
    {
        case WM_KEYDOWN:    // Fallthrough
        case WM_SYSKEYDOWN:
            event.eventType = KeyboardEvent::ET_PRESS;
            break;
        case WM_KEYUP:      // Fallthrough
        case WM_SYSKEYUP:
            event.eventType = KeyboardEvent::ET_RELEASE;
            break;
        default:
            return false;
    }

    event.nativeKey = kbdllhs->vkCode;

    return true;
}

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_EVENT_CONVERTER_HPP
