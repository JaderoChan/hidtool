#ifndef HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
#define HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP

#include <platforms/linux/hooker_private.hpp>
#include <hidtool/mouse/mouse_hooker.hpp>

namespace hidt
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

    bool isAccessDevice(int fd) override;
    void handleInputEvent(int fd) override;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_HOOKER_PRIVATE_HPP
