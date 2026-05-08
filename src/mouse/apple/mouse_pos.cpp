#include <hidtool/mouse/mouse_pos.hpp>

#include <vector>

#include <CoreGraphics/CoreGraphics.h>  // kCG*, CG*

namespace hidt
{

AbsolutePos getCursorPos()
{
    CGEventRef cur = CGEventCreate(nullptr);
    CGPoint pt = CGEventGetLocation(cur);
    CFRelease(cur);

    return AbsolutePos(static_cast<int32_t>(pt.x), static_cast<int32_t>(pt.y));
}

} // namespace hidt
