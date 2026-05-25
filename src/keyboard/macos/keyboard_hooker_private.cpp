#include "keyboard_hooker_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidt
{

KeyboardHookerPrivate::~KeyboardHookerPrivate() { stop(); }

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::isKeyPressed(int32_t nativeKey)
{
    return CGEventSourceKeyState(
        kCGEventSourceStateCombinedSessionState,
        static_cast<CGKeyCode>(nativeKey));
}

bool KeyboardHookerPrivate::setEventHandler(KeyboardEventHandler eventHandler, void* userData)
{
    return HookerPrivate::setEventHandler<KeyboardEventHandler>(eventHandler, userData);
}

bool KeyboardHookerPrivate::setUserData(void* userData)
{
    return HookerPrivate::setUserData(userData);
}

CGEventMask KeyboardHookerPrivate::getCGEventMask() const
{
    return CGEventMaskBit(kCGEventKeyDown) |
        CGEventMaskBit(kCGEventKeyUp) |
        CGEventMaskBit(kCGEventFlagsChanged);
}

CGEventTapCallBack KeyboardHookerPrivate::getCGEventTapCallback() const
{
    return &keyboardTapCallback;
}

CGEventRef KeyboardHookerPrivate::keyboardTapCallback(
    CGEventTapProxy proxy, CGEventType cgEventType, CGEventRef cgEvent, void* data)
{
    static KeyboardHookerPrivate& hooker = getInstance();

    KeyboardEvent event;
    if (keyboardEventFromCGEvent(event, cgEventType, cgEvent))
    {
        // 取得事件处理程序。
        auto eventHandler = hooker.getEventHandler<KeyboardEventHandler>();
        // 如果事件处理程序返回 false，则返回 nullptr 以阻断事件传播。
        if (eventHandler && !eventHandler(event, hooker.getUserData()))
            return nullptr;
    }

    return cgEvent;
}

} // namespace hidt
