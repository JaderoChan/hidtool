#include "keyboard_hooker_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidt
{

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::isKeyPressed(int32_t nativeKey)
{
    return (GetAsyncKeyState(nativeKey) & 0x80000) != 0;
}

bool KeyboardHookerPrivate::setEventHandler(KeyboardEventHandler eventHandler, void* userData)
{
    return HookerPrivate::setEventHandler<KeyboardEventHandler>(eventHandler, userData);
}

bool KeyboardHookerPrivate::setUserData(void* userData)
{
    return HookerPrivate::setUserData(userData);
}

HHOOK KeyboardHookerPrivate::setWindowHook()
{
    // 设置低级键盘钩子。
    return SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, nullptr, 0);
}

LRESULT WINAPI KeyboardHookerPrivate::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static KeyboardHookerPrivate& hooker = getInstance();

    if (nCode == HC_ACTION)
    {
        KeyboardEvent event;
        if (keyboardEventFromParam(event, wParam, lParam))
        {
            // 取得事件处理程序。
            auto eventHandler = hooker.getEventHandler<KeyboardEventHandler>();
            // 如果事件处理程序返回 false，则返回 1 以阻断事件传播。
            if (eventHandler && !eventHandler(event, hooker.getUserData()))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidt
