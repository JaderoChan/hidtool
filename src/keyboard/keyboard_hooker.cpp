#include <hidtool/keyboard/keyboard_hooker.hpp>

#if defined(HIDTOOL_WIN)
    #include "win/keyboard_hooker_private.hpp"
#elif defined(HIDTOOL_MACOS)
    #include "macos/keyboard_hooker_private.hpp"
#elif defined(HIDTOOL_LINUX)
    #include "linux/keyboard_hooker_private.hpp"
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN

namespace hidt
{

KeyboardHooker::KeyboardHooker(KeyboardHookerPrivate& pri)
    : pri_(pri) {}

KeyboardHooker& KeyboardHooker::getInstance()
{
    static KeyboardHooker instance(KeyboardHookerPrivate::getInstance());
    return instance;
}

bool KeyboardHooker::run()
{ return pri_.run(); }

void KeyboardHooker::stop()
{ return pri_.stop(); }

bool KeyboardHooker::isRunning() const
{ return pri_.isRunning(); }

bool KeyboardHooker::setEventHandler(KeyboardEventHandler eventHandler, void* userData)
{ return pri_.setEventHandler(eventHandler, userData); }

bool KeyboardHooker::setUserData(void* userData)
{ return pri_.setUserData(userData); }

} // namespace hidt
