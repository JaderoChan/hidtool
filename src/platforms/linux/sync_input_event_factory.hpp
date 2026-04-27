#ifndef HIDTOOL_INPUT_EVENT_FACTORY_HPP
#define HIDTOOL_INPUT_EVENT_FACTORY_HPP

#include <linux/input.h>

namespace hidtool
{

inline void setSyncReportEvent(struct input_event& ie)
{
    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
}

} // namespace hidtool

#endif // !HIDTOOL_INPUT_EVENT_FACTORY_HPP
