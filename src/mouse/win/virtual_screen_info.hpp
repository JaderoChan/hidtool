#ifndef HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
#define HIDTOOL_VIRTUAL_SCREEN_INFO_HPP

#include <cstdint>  // int32_t

namespace hidtool
{

struct VirtualScreenInfo
{
    static VirtualScreenInfo getLatest();

    void update();

    int16_t x = 0;
    int16_t y = 0;
    int16_t width = 0;
    int16_t height = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
