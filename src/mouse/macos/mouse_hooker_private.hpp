#ifndef HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
#define HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP

#include <platforms/macos/hooker_private.hpp>
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
    ~MouseHookerPrivate();
    MouseHookerPrivate(const MouseHookerPrivate&) = delete;
    MouseHookerPrivate& operator=(const MouseHookerPrivate&) = delete;

    CGEventMask getCGEventMask() const override;
    CGEventTapCallBack getCGEventTapCallback() const override;

    static CGEventRef mouseTapCallback(
        CGEventTapProxy proxy, CGEventType cgEventType, CGEventRef cgEvent, void* data);
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
