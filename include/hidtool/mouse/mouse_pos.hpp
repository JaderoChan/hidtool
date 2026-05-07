#ifndef HIDTOOL_MOUSE_POS_HPP
#define HIDTOOL_MOUSE_POS_HPP

#include <hidtool/defines.hpp>

namespace hidt
{

struct AbsolutePos
{
    constexpr AbsolutePos() noexcept = default;
    constexpr AbsolutePos(int32_t x, int32_t y) noexcept
        : x(x), y(y) {}

    int32_t x = 0;
    int32_t y = 0;
};

struct RelativePos
{
    constexpr RelativePos() noexcept = default;
    constexpr RelativePos(int32_t dx, int32_t dy) noexcept
        : dx(dx), dy(dy) {}

    int32_t dx = 0;
    int32_t dy = 0;
};

/** @brief 绝对移动事件的坐标范围。 */
struct AbsolutePosRange
{
    int32_t minX = 0;
    int32_t maxX = 0;
    int32_t minY = 0;
    int32_t maxY = 0;
};

constexpr inline AbsolutePos operator+(const AbsolutePos& lhs, const AbsolutePos& rhs) noexcept
{ return AbsolutePos(lhs.x + rhs.x, lhs.y + rhs.y); }

constexpr inline AbsolutePos operator-(const AbsolutePos& lhs, const AbsolutePos& rhs) noexcept
{ return AbsolutePos(lhs.x - rhs.x, lhs.y - rhs.y); }

inline AbsolutePos& operator+=(AbsolutePos& lhs, const AbsolutePos& rhs) noexcept
{ lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }

inline AbsolutePos& operator-=(AbsolutePos& lhs, const AbsolutePos& rhs) noexcept
{ lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }

constexpr inline RelativePos operator+(const RelativePos& lhs, const RelativePos& rhs) noexcept
{ return RelativePos(lhs.dx + rhs.dx, lhs.dy + rhs.dy); }

inline RelativePos& operator+=(RelativePos& lhs, const RelativePos& rhs) noexcept
{ lhs.dx += rhs.dx; lhs.dy += rhs.dy; return lhs; }

constexpr inline AbsolutePos operator+(const AbsolutePos& lhs, const RelativePos& rhs) noexcept
{ return AbsolutePos(lhs.x + rhs.dx, lhs.y + rhs.dy); }

inline AbsolutePos& operator+=(AbsolutePos& lhs, const RelativePos& rhs) noexcept
{ lhs.x += rhs.dx; lhs.y += rhs.dy; return lhs; }

constexpr inline AbsolutePos operator+(const AbsolutePos& lhs, int32_t rhs) noexcept
{ return AbsolutePos(lhs.x + rhs, lhs.y + rhs); }

constexpr inline AbsolutePos operator-(const AbsolutePos& lhs, int32_t rhs) noexcept
{ return AbsolutePos(lhs.x - rhs, lhs.y - rhs); }

inline AbsolutePos& operator+=(AbsolutePos& lhs, int32_t rhs) noexcept
{ lhs.x += rhs; lhs.y += rhs; return lhs; }

inline AbsolutePos& operator-=(AbsolutePos& lhs, int32_t rhs) noexcept
{ lhs.x -= rhs; lhs.y -= rhs; return lhs; }

/** @brief 将给定的绝对坐标钳制在指定范围内。 */
inline AbsolutePos clampAbsolutePos(const AbsolutePos& absPos, const AbsolutePosRange& absPosRange) noexcept
{
    AbsolutePos result = absPos;

    if (result.x < absPosRange.minX)
        result.x = absPosRange.minX;
    else if (result.x > absPosRange.maxX)
        result.x = absPosRange.maxX;

    if (result.y < absPosRange.minY)
        result.y = absPosRange.minY;
    else if (result.y > absPosRange.maxY)
        result.y = absPosRange.maxY;

    return result;
}

/**
 * @brief 获得当前鼠标指针位置。
 * @todo **Linux** 平台实现。
 * @sa \ref `getAbsolutePosRange()`
 */
AbsolutePos getCursorPos();

/**
 * @brief 获得当前环境下的绝对移动事件的坐标范围。
 * @note 在 **Windows** 和 **MacOS** 平台下，当屏幕设备发生变动时，此前获得的值可能会过时。
 * @details
 * 在 **Windows** 和 **MacOS** 平台下，其等同于虚拟屏幕空间的范围。
 * 在 **Linux** 平台下，其始终等同于 {0, 65535, 0, 65535}。
 */
AbsolutePosRange getAbsolutePosRange();

} // namespace hidt

#endif // !HIDTOOL_MOUSE_POS_HPP
