#include "virtual_screen_info.hpp"

#include <windows.h>

namespace hidtool
{

VirtualScreenInfo VirtualScreenInfo::getLatest()
{
    VirtualScreenInfo result;
    result.update();
    return result;
}

void VirtualScreenInfo::update()
{
    DPI_AWARENESS_CONTEXT prevCtx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);

    x = static_cast<int16_t>(GetSystemMetrics(SM_XVIRTUALSCREEN));
    y = static_cast<int16_t>(GetSystemMetrics(SM_YVIRTUALSCREEN));
    width = static_cast<int16_t>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
    height = static_cast<int16_t>(GetSystemMetrics(SM_CYVIRTUALSCREEN));

    SetThreadDpiAwarenessContext(prevCtx);
}

} // namespace hidtool
