#ifndef HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP

#include <platforms/linux/hooker_private.hpp>
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

    bool isAccessDevice(int fd) override;
    void handleInputEvent(int fd) override;
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_HOOKER_PRIVATE_HPP
