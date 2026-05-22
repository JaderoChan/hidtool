#ifndef HIDTOOL_INPUT_EVENT_FACTORY_HPP
#define HIDTOOL_INPUT_EVENT_FACTORY_HPP

#include <cstdint>  // int32_t

#include <linux/input.h>

namespace hidt
{

inline void setPressKeyInputEvent(struct input_event& ie, int32_t nativeKey)
{
    ie.type = EV_KEY;
    ie.value = 1;   // 1 is press.
    ie.code = static_cast<__s32>(nativeKey);
}

inline void setReleaseKeyInputEvent(struct input_event& ie, int32_t nativeKey)
{
    ie.type = EV_KEY;
    ie.value = 0;   // 0 is release.
    ie.code = static_cast<__s32>(nativeKey);
}

} // namespace hidt

#endif // !HIDTOOL_INPUT_EVENT_FACTORY_HPP
