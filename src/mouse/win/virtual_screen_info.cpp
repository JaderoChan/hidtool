#include "virtual_screen_info.hpp"

#include <windows.h>

namespace hidt
{

VirtualScreenInfo VirtualScreenInfo::getLatest()
{
    VirtualScreenInfo result;
    result.update();
    return result;
}

void VirtualScreenInfo::update()
{
#ifdef HIDTOOL_FORCE_IN_PIXEL
    DPI_AWARENESS_CONTEXT prevCtx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif // HIDTOOL_FORCE_IN_PIXEL

    x = static_cast<int16_t>(GetSystemMetrics(SM_XVIRTUALSCREEN));
    y = static_cast<int16_t>(GetSystemMetrics(SM_YVIRTUALSCREEN));
    width = static_cast<int16_t>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
    height = static_cast<int16_t>(GetSystemMetrics(SM_CYVIRTUALSCREEN));

#ifdef HIDTOOL_FORCE_IN_PIXEL
    SetThreadDpiAwarenessContext(prevCtx);
#endif // HIDTOOL_FORCE_IN_PIXEL
}

} // namespace hidt
