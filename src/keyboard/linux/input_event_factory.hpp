#ifndef HIDTOOL_INPUT_EVENT_FACTORY_HPP
#define HIDTOOL_INPUT_EVENT_FACTORY_HPP

#include <cstdint>  // uint32_t

#include <linux/input.h>

namespace hidtool
{

inline void setPressKeyInputEvent(struct input_event& ie, uint32_t nativeKey)
{
    ie.type = EV_KEY;
    ie.value = 1;   // 1 is press.
    ie.code = static_cast<__s32>(nativeKey);
}

inline void setReleaseKeyInputEvent(struct input_event& ie, uint32_t nativeKey)
{
    ie.type = EV_KEY;
    ie.value = 0;   // 1 is release.
    ie.code = static_cast<__s32>(nativeKey);
}

} // namespace hidtool

#endif // !HIDTOOL_INPUT_EVENT_FACTORY_HPP
