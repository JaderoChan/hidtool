#ifndef HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
#define HIDTOOL_MOUSE_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>

#include <hidtool/mouse/mouse_event.hpp>

namespace hidtool
{

inline bool mouseEventToCGEvent(const MouseEvent& event, CGEventRef& cgEvent)
{

}

inline bool mouseEventFromCGEvent(MouseEvent& event, CGEventType cgEventType, const CGEventRef& cgEvent)
{

}

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_EVENT_CONVERTER_HPP
