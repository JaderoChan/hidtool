#include <hidtool/keyboard/keyboard_simulator.hpp>

#if defined(HIDTOOL_WIN)
    #include "win/keyboard_simulator_private.hpp"
#elif defined(HIDTOOL_APPLE)
    #include "apple/keyboard_simulator_private.hpp"
#elif defined(HIDTOOL_LINUX)
    #include "linux/keyboard_simulator_private.hpp"
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN

namespace hidtool
{

KeyboardSimulator::KeyboardSimulator(KeyboardSimulatorPrivate& pri)
    : pri_(pri) {}

KeyboardSimulator& KeyboardSimulator::getInstance()
{
    static KeyboardSimulator instance(KeyboardSimulatorPrivate::getInstance());
    return instance;
}

bool KeyboardSimulator::initialize()
{ return pri_.initialize(); }

void KeyboardSimulator::destroy()
{ pri_.destroy(); }

bool KeyboardSimulator::isInitialized() const
{ return pri_.isInitialized(); }

bool KeyboardSimulator::sendEvent(const KeyboardEvent& event)
{ return pri_.sendEvent(event); }

size_t KeyboardSimulator::sendEvent(const KeyboardEvent* events, size_t count)
{ return pri_.sendEvent(events, count); }

bool KeyboardSimulator::pressKey(uint32_t nativeKey)
{ return pri_.pressKey(nativeKey); }

bool KeyboardSimulator::pressKey(KeyboardKey key)
{ return pri_.pressKey(keyboardKeyToNativeKey(key)); }

bool KeyboardSimulator::releaseKey(uint32_t nativeKey)
{ return pri_.releaseKey(nativeKey); }

bool KeyboardSimulator::releaseKey(KeyboardKey key)
{ return pri_.releaseKey(keyboardKeyToNativeKey(key)); }

bool KeyboardSimulator::clickKey(uint32_t nativeKey)
{ return pri_.clickKey(nativeKey); }

bool KeyboardSimulator::clickKey(KeyboardKey key)
{ return pri_.clickKey(keyboardKeyToNativeKey(key)); }

} // namespace hidtool
