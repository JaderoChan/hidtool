#include "mouse_hooker_private.hpp"

#include "mouse_event_converter.hpp"

namespace hidtool
{

MouseEventHandler MouseHookerPrivate::eventHandler_ = nullptr;

MouseHookerPrivate::~MouseHookerPrivate()
{
    eventHandler_ = nullptr;
}

MouseHookerPrivate& MouseHookerPrivate::getInstance()
{
    static MouseHookerPrivate instance;
    return instance;
}

bool MouseHookerPrivate::setEventHandler(const MouseEventHandler& eventHandler)
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (!isRunning())
    {
        eventHandler_ = eventHandler;
        return true;
    }

    return HookerPrivate::sendSetEventHandlerEvent(reinterpret_cast<WPARAM>(eventHandler));
}

HHOOK MouseHookerPrivate::setWindowHook()
{
    return SetWindowsHookExA(WH_MOUSE_LL, &LowLevelMouseProc, nullptr, 0);
}

void MouseHookerPrivate::handleSetEventHandlerEvent(WPARAM eventHandler)
{
    eventHandler_ = reinterpret_cast<MouseEventHandler>(static_cast<intptr_t>(eventHandler));
}

LRESULT WINAPI MouseHookerPrivate::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        MouseEvent event(MouseEvent::ET_ABS_MOVE);
        if (mouseEventFromParam(event, wParam, lParam))
        {
            auto eventHandler = MouseHookerPrivate::eventHandler_;
            if (eventHandler && !eventHandler(event))
                return 1;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace hidtool
