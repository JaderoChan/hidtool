#include <hidtool/mouse/mouse_pos.hpp>

#include "mouse_uinput.hpp"

namespace hidtool
{

AbsolutePosRange getAbsolutePosRange()
{
    AbsolutePosRange result;

    result.minX = ABS_MIN_X;
    result.maxX = ABS_MAX_X;
    result.minY = ABS_MIN_Y;
    result.maxY = ABS_MAX_Y;

    return result;
}

} // namespace hidtool
