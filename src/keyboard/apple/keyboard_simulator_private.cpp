#include "keyboard_simulator_private.hpp"

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate()
{
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
    return isInitialized_.compare_exchange_strong(expected, true);
}

void KeyboardSimulatorPrivate::destroy()
{
    bool expected = true;
    isInitialized_.compare_exchange_strong(expected, false);
}

bool KeyboardSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent& event)
{
    if (!isInitialized_.load())
        return false;

    CGEventRef cgEvent = nullptr;
    if (keyboardEventToCGEvent(event, cgEvent))
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

size_t KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;
}

bool KeyboardSimulatorPrivate::pressKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    CGEventRef cgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, true);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool KeyboardSimulatorPrivate::releaseKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    CGEventRef cgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, false);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        return true;
    }

    return false;
}

bool KeyboardSimulatorPrivate::clickKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    int success = 0;

    CGEventRef cgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, true);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        success++;
    }

    cgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, false);
    if (cgEvent)
    {
        CGEventPost(kCGHIDEventTap, cgEvent);
        CFRelease(cgEvent);
        success++;
    }

    return success == 2;
}

} // namespace hidtool
