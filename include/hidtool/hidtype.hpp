#ifndef HIDTOOL_HIDTYPE_HPP
#define HIDTOOL_HIDTYPE_HPP

#include "defines.hpp"

namespace hidt
{

/** @brief HID Device types */
enum HidType : uint8_t
{
    HIDTYPE_KEYBOARD,
    HIDTYPE_MOUSE
};

/** @brief Check if the current environment supports the specified HID type submodule. */
HIDTOOL_API bool isHidTypeSupported(HidType hidType) noexcept;

} // namespace hidt

#endif // !HIDTOOL_HIDTYPE_HPP
