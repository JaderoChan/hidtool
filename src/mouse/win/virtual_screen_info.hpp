#ifndef HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
#define HIDTOOL_VIRTUAL_SCREEN_INFO_HPP

#include <cstdint>  // int32_t

namespace hidt
{

// 使用 int16_t 作为成员变量类型，能够使得结构体总大小为8字节，以便其能在最少依赖的情况下保证可原子化。
// （Windows 下超过8字节的平凡数据类型需要额外依赖/实现才能保证其可原子化）
// 此外，由于虚拟屏幕的坐标实际上均由16位有符号值表示，故不存在数值溢出等问题。
// （参见 https://learn.microsoft.com/en-us/windows/win32/gdi/the-virtual-screen）
struct VirtualScreenInfo
{
    static VirtualScreenInfo getLatest();

    void update();

    int16_t x = 0;
    int16_t y = 0;
    int16_t width = 0;
    int16_t height = 0;
};

} // namespace hidt

#endif // !HIDTOOL_VIRTUAL_SCREEN_INFO_HPP
