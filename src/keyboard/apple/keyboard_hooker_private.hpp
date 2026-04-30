#ifndef HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP

#include <platforms/apple/hooker_private.hpp>
#include <hidtool/keyboard/keyboard_hooker.hpp>

namespace hidtool
{

class KeyboardHookerPrivate : public HookerPrivate
{
public:
    static KeyboardHookerPrivate& getInstance();

    bool setEventHandler(const KeyboardEventHandler& eventHandler);

private:
    KeyboardHookerPrivate() = default;
    ~KeyboardHookerPrivate();
    KeyboardHookerPrivate(const KeyboardHookerPrivate&) = delete;
    KeyboardHookerPrivate& operator=(const KeyboardHookerPrivate&) = delete;

    CGEventMask getCGEventMask() const override;
    CGEventTapCallBack getCGEventTapCallback() const override;

    static CGEventRef keyboardTapCallback(
        CGEventTapProxy proxy, CGEventType cgEventType, CGEventRef cgEvent, void* data);
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
