#include <hidtool/mouse/mouse_hooker.hpp>

#if defined(HIDTOOL_WIN)
    #include "win/mouse_hooker_private.hpp"
#elif defined(HIDTOOL_APPLE)
    #include "apple/mouse_hooker_private.hpp"
#elif defined(HIDTOOL_LINUX)
    #include "linux/mouse_hooker_private.hpp"
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN

namespace hidt
{

MouseHooker::MouseHooker(MouseHookerPrivate& pri)
    : pri_(pri) {}

MouseHooker& MouseHooker::getInstance()
{
    static MouseHooker instance(MouseHookerPrivate::getInstance());
    return instance;
}

bool MouseHooker::isSupportBlockEventPropagation() noexcept
{
#if defined(HIDTOOL_WIN)
    return true;
#elif defined(HIDTOOL_APPLE)
    return true;
#elif defined(HIDTOOL_LINUX)
    return false;
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN
}

bool MouseHooker::run()
{ return pri_.run(); }

void MouseHooker::stop()
{ return pri_.stop(); }

bool MouseHooker::isRunning() const
{ return pri_.isRunning(); }

bool MouseHooker::setEventHandler(const MouseEventHandler& eventHandler)
{ return pri_.setEventHandler(eventHandler); }

} // namespace hidt
