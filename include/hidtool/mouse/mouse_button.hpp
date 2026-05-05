#ifndef HIDTOOL_MOUSE_BUTTON_HPP
#define HIDTOOL_MOUSE_BUTTON_HPP

#include <hidtool/defines.hpp>

namespace hidt
{

enum MouseButton : uint8_t
{
    MSBTN_NONE,
    MSBTN_LEFT,
    MSBTN_RIGHT,
    MSBTN_MIDDLE,
    MSBTN_BACK,
    MSBTN_FORWARD
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_BUTTON_HPP
