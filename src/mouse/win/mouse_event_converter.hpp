#ifndef HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
#define HIDTOOL_MOUSE_EVENT_CONVERTER_HPP

#include <windows.h>

#include <hidtool/mouse/mouse_event.hpp>

namespace hidtool
{

constexpr DWORD MOUSEEVENTF_ABS_MOVE = (MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE);

inline bool setPressMouseButtonInput(INPUT& input, MouseButton button)
{
    switch (button)
    {
        case MSBTN_LEFT:
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            break;
        case MSBTN_RIGHT:
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            break;
        case MSBTN_MIDDLE:
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            break;
        case MSBTN_BACK:
            input.mi.dwFlags = MOUSEEVENTF_XDOWN;
            input.mi.mouseData = XBUTTON1;
            break;
        case MSBTN_FORWARD:
            input.mi.dwFlags = MOUSEEVENTF_XDOWN;
            input.mi.mouseData = XBUTTON2;
            break;
        default:
            return false;
    }

    return true;
}

inline bool setReleaseMouseButtonInput(INPUT& input, MouseButton button)
{
    switch (button)
    {
        case MSBTN_LEFT:
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        case MSBTN_RIGHT:
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case MSBTN_MIDDLE:
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        case MSBTN_BACK:
            input.mi.dwFlags = MOUSEEVENTF_XUP;
            input.mi.mouseData = XBUTTON1;
            break;
        case MSBTN_FORWARD:
            input.mi.dwFlags = MOUSEEVENTF_XUP;
            input.mi.mouseData = XBUTTON2;
            break;
        default:
            return false;
    }

    return true;
}

inline bool mouseEventToInput(const MouseEvent& event, INPUT& input)
{
    input.type = INPUT_MOUSE;

    switch (event.eventType)
    {
        case MouseEvent::ET_ABS_MOVE:
            input.mi.dwFlags = MOUSEEVENTF_ABS_MOVE;
            input.mi.dx = event.pos.x;
            input.mi.dy = event.pos.y;
            break;
        case MouseEvent::ET_REL_MOVE:
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            input.mi.dx = event.pos.x;
            input.mi.dy = event.pos.y;
            break;
        case MouseEvent::ET_WHEEL:
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = event.wheelDelta;
            break;
        case MouseEvent::ET_PRESS:
            if (!setPressMouseButtonInput(input, event.button))
                return false;
            break;
        case MouseEvent::ET_RELEASE:
            if (!setReleaseMouseButtonInput(input, event.button))
                return false;
            break;
        default:
            return false;
    }

    return true;
}

inline bool mouseEventFromParam(MouseEvent& event, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* msllh = reinterpret_cast<MSLLHOOKSTRUCT*>(static_cast<intptr_t>(lParam));
    if (!msllh)
        return false;

    switch (wParam)
    {
        case WM_MOUSEMOVE:
            event.eventType = MouseEvent::ET_ABS_MOVE;
            event.pos = {msllh->pt.x, msllh->pt.y};
            break;
        case WM_MOUSEWHEEL:
            event.eventType = MouseEvent::ET_WHEEL;
            event.wheelDelta = static_cast<int32_t>(HIWORD(msllh->mouseData));
            break;
        case WM_LBUTTONDOWN:
            event.eventType = MouseEvent::ET_PRESS;
            event.button = MouseButton::MSBTN_LEFT;
            break;
        case WM_LBUTTONUP:
            event.eventType = MouseEvent::ET_RELEASE;
            event.button = MouseButton::MSBTN_LEFT;
            break;
        case WM_RBUTTONDOWN:
            event.eventType = MouseEvent::ET_PRESS;
            event.button = MouseButton::MSBTN_RIGHT;
            break;
        case WM_RBUTTONUP:
            event.eventType = MouseEvent::ET_RELEASE;
            event.button = MouseButton::MSBTN_RIGHT;
            break;
        case WM_MBUTTONDOWN:
            event.eventType = MouseEvent::ET_PRESS;
            event.button = MouseButton::MSBTN_MIDDLE;
            break;
        case WM_MBUTTONUP:
            event.eventType = MouseEvent::ET_RELEASE;
            event.button = MouseButton::MSBTN_MIDDLE;
            break;
        case WM_XBUTTONDOWN:    // Fallthrough
        case WM_XBUTTONUP:
            event.eventType = (wParam == WM_XBUTTONDOWN ? MouseEvent::ET_PRESS : MouseEvent::ET_RELEASE);
            switch (HIWORD(msllh->mouseData))
            {
                case XBUTTON1:
                    event.button = MouseButton::MSBTN_BACK;
                    break;
                case XBUTTON2:
                    event.button = MouseButton::MSBTN_FORWARD;
                    break;
                default:
                    return false;
            }
            break;
        default:
            return false;
    }

    return true;
}

} // namespace hidism

#endif // !HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
