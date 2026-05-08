#include <hidtool/hidtype.hpp>

namespace hidt
{

bool isHidTypeSupported(HidType hidType) noexcept
{
    switch (hidType)
    {
        case HIDTYPE_KEYBOARD:
        #ifdef HIDTOOL_HAS_KEYBOARD
            return true;
        #else
            return false;
        #endif // HIDTOOL_HAS_KEYBOARD

        case HIDTYPE_MOUSE:
        #ifdef HIDTOOL_HAS_MOUSE
            return true;
        #else
            return false;
        #endif // HIDTOOL_HAS_MOUSE

        case HIDTYPE_SLEEP:
            return true;

        default:
            return false;
    }
}

} // namespace hidt
