#ifndef HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
#define HIDTOOL_VIRTUAL_SCREEN_INFO_HPP

#include <cstdint>  // int32_t

namespace hidtool
{

struct VirtualScreenInfo
{
    static VirtualScreenInfo getLatest();
    static void update(VirtualScreenInfo& vsi);

    int32_t x = 0;
    int32_t y = 0;
    int32_t width = 0;
    int32_t height = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
