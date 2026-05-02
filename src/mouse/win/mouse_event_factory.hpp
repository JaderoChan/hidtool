#ifndef HIDTOOL_MOUSE_EVENT_FACTORY_HPP
#define HIDTOOL_MOUSE_EVENT_FACTORY_HPP

#include <windows.h>

#include <hidtool/mouse/mouse_event.hpp>

namespace hidtool
{

[[nodiscard]] inline bool mouseEventFromParam(MouseEvent& event, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* msllh = reinterpret_cast<MSLLHOOKSTRUCT*>(static_cast<intptr_t>(lParam));
    if (!msllh)
        return false;

    switch (wParam)
    {
        case WM_MOUSEMOVE:
            event.type = MouseEvent::ET_ABS_MOVE;
            event.absPos = {msllh->pt.x, msllh->pt.y};
            break;
        case WM_MOUSEWHEEL:
            event.type = MouseEvent::ET_WHEEL;
            event.wheelDelta = static_cast<int32_t>(static_cast<int16_t>(HIWORD(msllh->mouseData)));
            break;
        case WM_LBUTTONDOWN:
            event.type = MouseEvent::ET_PRESS;
            event.button = MSBTN_LEFT;
            break;
        case WM_LBUTTONUP:
            event.type = MouseEvent::ET_RELEASE;
            event.button = MSBTN_LEFT;
            break;
        case WM_RBUTTONDOWN:
            event.type = MouseEvent::ET_PRESS;
            event.button = MSBTN_RIGHT;
            break;
        case WM_RBUTTONUP:
            event.type = MouseEvent::ET_RELEASE;
            event.button = MSBTN_RIGHT;
            break;
        case WM_MBUTTONDOWN:
            event.type = MouseEvent::ET_PRESS;
            event.button = MSBTN_MIDDLE;
            break;
        case WM_MBUTTONUP:
            event.type = MouseEvent::ET_RELEASE;
            event.button = MSBTN_MIDDLE;
            break;
        case WM_XBUTTONDOWN:    // Fallthrough
        case WM_XBUTTONUP:
            event.type = (wParam == WM_XBUTTONDOWN ? MouseEvent::ET_PRESS : MouseEvent::ET_RELEASE);
            switch (HIWORD(msllh->mouseData))
            {
                case XBUTTON1:
                    event.button = MSBTN_BACK;
                    break;
                case XBUTTON2:
                    event.button = MSBTN_FORWARD;
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

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_EVENT_FACTORY_HPP
