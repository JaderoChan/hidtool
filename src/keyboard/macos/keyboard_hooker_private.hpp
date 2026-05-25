#ifndef HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP

#include <platforms/macos/hooker_private.hpp>
#include <hidtool/keyboard/keyboard_hooker.hpp>

namespace hidt
{

class KeyboardHookerPrivate : public HookerPrivate
{
public:
    static KeyboardHookerPrivate& getInstance();

    static bool isKeyPressed(int32_t nativeKey);

    bool setEventHandler(KeyboardEventHandler eventHandler, void* userData = nullptr);
    bool setUserData(void* userData);

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

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
