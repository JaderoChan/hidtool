#ifndef HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
#define HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP

#include <platforms/win/hooker_private.hpp>
#include <hidtool/mouse/mouse_hooker.hpp>

namespace hidt
{

class MouseHookerPrivate : public HookerPrivate
{
public:
    static MouseHookerPrivate& getInstance();

    static bool isButtonPressed(MouseButton button);
    static AbsolutePos getCursorPos();

    bool setEventHandler(MouseEventHandler eventHandler, void* userData = nullptr);
    bool setUserData(void* userData);

private:
    MouseHookerPrivate() = default;
    ~MouseHookerPrivate() = default;
    MouseHookerPrivate(const MouseHookerPrivate&) = delete;
    MouseHookerPrivate& operator=(const MouseHookerPrivate&) = delete;

    HHOOK setWindowHook() override;

    static LRESULT WINAPI LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
