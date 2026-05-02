#include "virtual_screen_info.hpp"

#include <windows.h>

namespace hidtool
{

VirtualScreenInfo VirtualScreenInfo::getLatest()
{
    VirtualScreenInfo result;
    VirtualScreenInfo::update(result);
    return result;
}

void VirtualScreenInfo::update(VirtualScreenInfo& vsi)
{
    DPI_AWARENESS_CONTEXT prevCtx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);

    vsi.x = static_cast<int16_t>(GetSystemMetrics(SM_XVIRTUALSCREEN));
    vsi.y = static_cast<int16_t>(GetSystemMetrics(SM_YVIRTUALSCREEN));
    vsi.width = static_cast<int16_t>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
    vsi.height = static_cast<int16_t>(GetSystemMetrics(SM_CYVIRTUALSCREEN));

    SetThreadDpiAwarenessContext(prevCtx);
}

} // namespace hidtool
