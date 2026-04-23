#include "keyboard_simulator_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate()
{
    if (!isInitialized_.load())
        return;

    isInitialized_.store(false);
}

KeyboardSimulatorPrivate& KeyboardSimulatorPrivate::getInstance()
{
    static KeyboardSimulatorPrivate instance;
    return instance;
}

bool KeyboardSimulatorPrivate::initialize()
{
    bool expected = false;
    if (!isInitialized_.compare_exchange_strong(expected, true))
        return false;

    return true;
}

void KeyboardSimulatorPrivate::destroy()
{
    bool expected = true;
    if (!isInitialized_.compare_exchange_strong(expected, false))
        return;
}

bool KeyboardSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent& event)
{
    if (!isInitialized_.load())
        return false;
}

size_t KeyboardSimulatorPrivate::sendEvent(const std::vector<KeyboardEvent>& events)
{
    if (!isInitialized_.load())
        return 0;
}

bool KeyboardSimulatorPrivate::pressKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;
}

bool KeyboardSimulatorPrivate::releaseKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;
}

bool KeyboardSimulatorPrivate::clickKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;
}

} // namespace hidtool
