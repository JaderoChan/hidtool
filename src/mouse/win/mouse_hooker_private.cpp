#include "mouse_hooker_private.hpp"

#include "mouse_event_factory.hpp"

namespace hidt
{

MouseHookerPrivate& MouseHookerPrivate::getInstance()
{
    static MouseHookerPrivate instance;
    return instance;
}

bool MouseHookerPrivate::isButtonPressed(MouseButton button)
{
    int vk;
    switch (button)
    {
        case MSBTN_LEFT:    vk = VK_LBUTTON;  break;
        case MSBTN_RIGHT:   vk = VK_RBUTTON;  break;
        case MSBTN_MIDDLE:  vk = VK_MBUTTON;  break;
        case MSBTN_BACK:    vk = VK_XBUTTON1; break;
        case MSBTN_FORWARD: vk = VK_XBUTTON2; break;
        default: return false;
    }
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

AbsolutePos MouseHookerPrivate::getCursorPos()
{
    POINT pt;
    if (GetCursorPos(&pt))
        return AbsolutePos(static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y));
    return AbsolutePos(0, 0);
}

bool MouseHookerPrivate::setEventHandler(MouseEventHandler eventHandler, void* userData)
{
    return HookerPrivate::setEventHandler<MouseEventHandler>(eventHandler, userData);
}

bool MouseHookerPrivate::setUserData(void* userData)
{
    return HookerPrivate::setUserData(userData);
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
            // 如果事件处理程序返回 false，返则回 1 以阻断事件传播。
            if (eventHandler && !eventHandler(event, hooker.getUserData()))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidt
