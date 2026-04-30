#ifndef HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP

#include <platforms/apple/hooker_private.hpp>
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
    ~MouseHookerPrivate();
    MouseHookerPrivate(const MouseHookerPrivate&) = delete;
    MouseHookerPrivate& operator=(const MouseHookerPrivate&) = delete;

    CGEventMask getCGEventMask() const override;
    CGEventTapCallBack getCGEventTapCallback() const override;

    static CGEventRef mouseTapCallback(
        CGEventTapProxy proxy, CGEventType cgEventType, CGEventRef cgEvent, void* data);
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
