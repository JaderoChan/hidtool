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

AbsolutePosRange getAbsolutePosRange()
{
    uint32_t displayCount = 0;
    CGError err = CGGetActiveDisplayList(0, nullptr, &displayCount);
    if (err != kCGErrorSuccess || displayCount == 0)
        return AbsolutePosRange();

    std::vector<CGDirectDisplayID> displays(displayCount);
    err = CGGetActiveDisplayList(displayCount, displays.data(), &displayCount);
    if (err != kCGErrorSuccess)
        return AbsolutePosRange();

    CGRect unionRect = CGDisplayBounds(displays[0]);
    for (const auto& id : displays)
        unionRect = CGRectUnion(unionRect, CGDisplayBounds(id));

    AbsolutePosRange result;

    result.minX = unionRect.origin.x;
    result.minY = unionRect.origin.y;
    result.maxX = unionRect.origin.x + unionRect.size.width;
    result.maxY = unionRect.origin.y + unionRect.size.height;

    return result;
}

} // namespace hidt
