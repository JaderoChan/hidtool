#ifndef HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
#define HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP

#include <platforms/win/hooker_private.hpp>
#include <hidtool/mouse/mouse_hooker.hpp>

namespace hidtool
{

class MouseHookerPrivate : public HookerPrivate
{
public:
    static MouseHookerPrivate& getInstance();

    bool setEventHandler(const MouseEventHandler& eventHandler);

private:
    MouseHookerPrivate() = default;
    ~MouseHookerPrivate() = default;
    MouseHookerPrivate(const MouseHookerPrivate&) = delete;
    MouseHookerPrivate& operator=(const MouseHookerPrivate&) = delete;

    HHOOK setWindowHook() override;

    static LRESULT WINAPI LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
