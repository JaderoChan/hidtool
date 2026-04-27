#include <hidtool/mouse/mouse_pos.hpp>

#include "virtual_screen_info.hpp"

namespace hidtool
{

AbsolutePosRange getAbsolutePosRange()
{
    AbsolutePosRange result;
    VirtualScreenInfo vsi = VirtualScreenInfo::getLatest();

    result.minX = vsi.x;
    result.minY = vsi.y;
    result.maxX = vsi.x + vsi.width;
    result.maxY = vsi.y + vsi.height;

    return result;
}

} // namespace hidtool
