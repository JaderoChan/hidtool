#ifndef HIDTOOL_HIDTYPE_HPP
#define HIDTOOL_HIDTYPE_HPP

#include "defines.hpp"

namespace hidtool
{

enum HidType : uint8_t
{
    HIDTYPE_KEYBOARD,
    HIDTYPE_MOUSE
};

HIDTOOL_API bool isHidTypeSupported(HidType hidType) noexcept;

} // namespace hidtool

#endif // !HIDTOOL_HIDTYPE_HPP
