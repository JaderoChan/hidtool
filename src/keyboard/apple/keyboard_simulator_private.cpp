#include "keyboard_simulator_private.hpp"

#include <CoreGraphics/CGEvent.h>           // kCG*, CG*
#include <CoreFoundation/CoreFoundation.h>  // CF*

#include "keyboard_event_converter.hpp"

namespace hidt
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate() { destroy(); }

KeyboardSimulatorPrivate& KeyboardSimulatorPrivate::getInstance()
{
    static KeyboardSimulatorPrivate instance;
    return instance;
}

bool KeyboardSimulatorPrivate::initialize()
{
    // 无实际作用，仅与其他子模块保持一致性。`destroy()` 同理。
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

    size_t sent = 0;

    CGEventRef cgEvent = nullptr;
    for (size_t i = 0; i < count; ++i)
    {
        if (keyboardEventToCGEvent(events[i], cgEvent))
        {
            CGEventPost(kCGHIDEventTap, cgEvent);
            CFRelease(cgEvent);
            sent++;
        }
    }

    return sent;
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

    size_t sent = 0;

    CGEventRef pressKeyCgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, true);
    CGEventRef releaseKeyCgEvent = CGEventCreateKeyboardEvent(nullptr, nativeKey, false);

    if (pressKeyCgEvent)
    {
        CGEventPost(kCGHIDEventTap, pressKeyCgEvent);
        CFRelease(pressKeyCgEvent);
        sent++;
    }

    if (releaseKeyCgEvent)
    {
        CGEventPost(kCGHIDEventTap, releaseKeyCgEvent);
        CFRelease(releaseKeyCgEvent);
        sent++;
    }

    return sent == 2;
}

} // namespace hidt
