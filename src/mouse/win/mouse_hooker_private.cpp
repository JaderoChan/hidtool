#include "mouse_hooker_private.hpp"

#include "mouse_event_converter.hpp"

namespace hidtool
{

MouseHookerPrivate& MouseHookerPrivate::getInstance()
{
    static MouseHookerPrivate instance;
    return instance;
}

bool MouseHookerPrivate::setEventHandler(const MouseEventHandler& eventHandler)
{
    return HookerPrivate::setEventHandler<MouseEventHandler>(eventHandler);
}

HHOOK MouseHookerPrivate::setWindowHook()
{
    return SetWindowsHookExA(WH_MOUSE_LL, &LowLevelMouseProc, nullptr, 0);
}

LRESULT WINAPI MouseHookerPrivate::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static MouseHookerPrivate& hooker = getInstance();

    if (nCode == HC_ACTION)
    {
        MouseEvent event(MouseEvent::ET_ABS_MOVE);
        if (mouseEventFromParam(event, wParam, lParam))
        {
            auto eventHandler = hooker.getEventHandler<MouseEventHandler>();
            if (eventHandler && !eventHandler(event))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidtool
