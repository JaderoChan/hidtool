#include <hidtool/mouse/mouse_pos.hpp>

#include <windows.h>

#include "virtual_screen_info.hpp"

namespace hidt
{

AbsolutePos getCursorPos()
{
    AbsolutePos result;

    HDESK curHDesk = GetThreadDesktop(GetCurrentThreadId());
    HDESK inpHDesk = OpenInputDesktop(0, true, 0);
    if (inpHDesk)
    {
        SetThreadDesktop(inpHDesk);
        DPI_AWARENESS_CONTEXT prevCtx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        POINT pt = {0};
        if (GetCursorPos(&pt))
        {
            result.x = pt.x;
            result.y = pt.y;
        }

        SetThreadDpiAwarenessContext(prevCtx);
        SetThreadDesktop(curHDesk);
        CloseDesktop(inpHDesk);
    }

    return result;
}

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
