#ifndef HIDTOOL_INPUT_EVENT_FACTORY_HPP
#define HIDTOOL_INPUT_EVENT_FACTORY_HPP

#include <linux/input.h>

namespace hidtool
{

constexpr struct input_event createSyncEvent()
{
    struct input_event ie = {0};

    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    return ie;
}

} // namespace hidism

#endif // !HIDTOOL_INPUT_EVENT_FACTORY_HPP
