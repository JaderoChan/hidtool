#include "keyboard_hooker_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardEventHandler KeyboardHookerPrivate::eventHandler_ = nullptr;

KeyboardHookerPrivate::~KeyboardHookerPrivate()
{
    eventHandler_ = nullptr;
}

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::setEventHandler(const KeyboardEventHandler& eventHandler)
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (!isRunning())
    {
        eventHandler_ = eventHandler;
        return true;
    }

    return HookerPrivate::sendSetEventHandlerEvent(reinterpret_cast<WPARAM>(eventHandler));
}

HHOOK KeyboardHookerPrivate::setWindowHook()
{
    return SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, nullptr, 0);
}

void KeyboardHookerPrivate::handleSetEventHandlerEvent(WPARAM eventHandler)
{
    eventHandler_ = reinterpret_cast<KeyboardEventHandler>(static_cast<intptr_t>(eventHandler));
}

LRESULT WINAPI KeyboardHookerPrivate::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KeyboardEvent event(KeyboardEvent::ET_PRESS);
        if (keyboardEventFromParam(event, wParam, lParam))
        {
            auto eventHandler = KeyboardHookerPrivate::eventHandler_;
            if (eventHandler && !eventHandler(event))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidtool
