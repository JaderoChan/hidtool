#ifndef HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
#define HIDTOOL_MOUSE_EVENT_CONVERTER_HPP

#include <algorithm>    // min(), max()

#include <windows.h>

#ifdef max
    #undef max
#endif
#ifdef min
    #undef min
#endif

#include <hidtool/mouse/mouse_button.hpp>
#include <hidtool/mouse/mouse_pos.hpp>
#include "virtual_screen_info.hpp"

namespace hidt
{

constexpr DWORD MOUSEEVENTF_ABS_MOVE = (MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE);

inline void setAbsoluteMoveInput(INPUT& input, const AbsolutePos& absPos, const VirtualScreenInfo& vsi)
{
    // Normalize position.
    LONG nx = static_cast<LONG>(static_cast<double>(absPos.x - vsi.x) / (vsi.width - 1) * 65535.0);
    LONG ny = static_cast<LONG>(static_cast<double>(absPos.y - vsi.y) / (vsi.height - 1) * 65535.0);

    // Clamp.
    nx = std::max(0L, std::min(65535L, nx));
    ny = std::max(0L, std::min(65535L, ny));

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABS_MOVE;
    input.mi.dx = nx;
    input.mi.dy = ny;
}

inline void setRelativeMoveInput(INPUT& input, const RelativePos& relPos)
{
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = static_cast<LONG>(relPos.dx);
    input.mi.dy = static_cast<LONG>(relPos.dy);
}

inline void setWheelInput(INPUT& input, int32_t wheelDelta)
{
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = static_cast<DWORD>(wheelDelta);
}

[[nodiscard]] inline bool setPressButtonInput(INPUT& input, MouseButton button)
{
    switch (button)
    {
        case MSBTN_LEFT:
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            break;
        case MSBTN_RIGHT:
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            break;
        case MSBTN_MIDDLE:
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            break;
        case MSBTN_BACK:
            input.mi.dwFlags = MOUSEEVENTF_XDOWN;
            input.mi.mouseData = XBUTTON1;
            break;
        case MSBTN_FORWARD:
            input.mi.dwFlags = MOUSEEVENTF_XDOWN;
            input.mi.mouseData = XBUTTON2;
            break;
        default:
            return false;
    }

    input.type = INPUT_MOUSE;

    return true;
}

[[nodiscard]] inline bool setReleaseButtonInput(INPUT& input, MouseButton button)
{
    switch (button)
    {
        case MSBTN_LEFT:
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        case MSBTN_RIGHT:
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case MSBTN_MIDDLE:
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        case MSBTN_BACK:
            input.mi.dwFlags = MOUSEEVENTF_XUP;
            input.mi.mouseData = XBUTTON1;
            break;
        case MSBTN_FORWARD:
            input.mi.dwFlags = MOUSEEVENTF_XUP;
            input.mi.mouseData = XBUTTON2;
            break;
        default:
            return false;
    }

    input.type = INPUT_MOUSE;

    return true;
}

} // namespace hidt

#endif // !HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
