#ifndef HIDTOOL_MOUSE_BUTTON_HPP
#define HIDTOOL_MOUSE_BUTTON_HPP

#include <hidtool/defines.hpp>

namespace hidtool
{

enum MouseButton : uint8_t
{
    MSBTN_LEFT,
    MSBTN_RIGHT,
    MSBTN_MIDDLE,
    MSBTN_BACK,
    MSBTN_FORWARD
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_BUTTON_HPP
