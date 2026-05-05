#include "mouse_hooker_private.hpp"

#include "mouse_event_factory.hpp"

namespace hidt
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
    // 设置低级鼠标钩子。
    return SetWindowsHookExA(WH_MOUSE_LL, &LowLevelMouseProc, nullptr, 0);
}

LRESULT WINAPI MouseHookerPrivate::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static MouseHookerPrivate& hooker = getInstance();

    if (nCode == HC_ACTION)
    {
        MouseEvent event;
        if (mouseEventFromParam(event, wParam, lParam))
        {
            // 调用事件处理程序。
            auto eventHandler = hooker.getEventHandler<MouseEventHandler>();
            // 如果事件处理程序返回 `false`，返回 `1` 以阻断事件传播。
            if (eventHandler && !eventHandler(event))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidt
