#ifndef HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP

#include <platforms/win/hooker_private.hpp>

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

    HHOOK setWindowHook() override;
    void handleSetEventHandlerEvent(WPARAM eventHandler) override;

    static LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyboardEventHandler eventHandler_;
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
