#include <hidtool/mouse/mouse_pos.hpp>

#include "virtual_screen_info.hpp"

namespace hidt
{

AbsolutePosRange getAbsolutePosRange()
{
    AbsolutePosRange result;
    VirtualScreenInfo vsi = VirtualScreenInfo::getLatest();

    result.minX = static_cast<int32_t>(vsi.x);
    result.minY = static_cast<int32_t>(vsi.y);
    result.maxX = static_cast<int32_t>(vsi.x) + static_cast<int32_t>(vsi.width);
    result.maxY = static_cast<int32_t>(vsi.y) + static_cast<int32_t>(vsi.height);

    return result;
}

} // namespace hidt
