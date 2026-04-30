#include "keyboard_hooker_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::setEventHandler(const KeyboardEventHandler& eventHandler)
{
    return HookerPrivate::setEventHandler<KeyboardEventHandler>(eventHandler);
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
            // 如果事件处理程序返回 `false`，返回 `1` 以阻断事件传播。
            if (eventHandler && !eventHandler(event))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidtool
