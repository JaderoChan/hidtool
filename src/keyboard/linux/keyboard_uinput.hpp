#ifndef HIDTOOL_KEYBOARD_UINPUT_HPP
#define HIDTOOL_KEYBOARD_UINPUT_HPP

#include <platforms/linux/uinput.hpp>

namespace hidt
{

class KeyboardUInput : public UInput
{
protected:
    bool setupUInputFd(int fd) override;
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_UINPUT_HPP
