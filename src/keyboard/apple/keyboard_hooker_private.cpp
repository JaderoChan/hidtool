#include "keyboard_hooker_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardHookerPrivate::~KeyboardHookerPrivate() { stop(); }

KeyboardHookerPrivate& KeyboardHookerPrivate::getInstance()
{
    static KeyboardHookerPrivate instance;
    return instance;
}

bool KeyboardHookerPrivate::setEventHandler(const KeyboardEventHandler& eventHandler)
{
    return HookerPrivate::setEventHandler<KeyboardEventHandler>(eventHandler);
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
        // 如果事件处理程序返回 `false`，返回 `nullptr` 以阻断事件传播。
        if (eventHandler && !eventHandler(event))
            return nullptr;
    }

    return cgEvent;
}

} // namespace hidtool
