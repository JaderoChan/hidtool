#ifndef HIDTOOL_MOUSE_UINPUT_HPP
#define HIDTOOL_MOUSE_UINPUT_HPP

#include <platforms/linux/uinput.hpp>

namespace hidtool
{

constexpr int32_t ABS_MIN_X = 0;
constexpr int32_t ABS_MAX_X = 65535;
constexpr int32_t ABS_MIN_Y = 0;
constexpr int32_t ABS_MAX_Y = 65535;

class MouseUInput : public UInput
{
protected:
    bool setupUInputFd(int fd) override;
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_UINPUT_HPP
