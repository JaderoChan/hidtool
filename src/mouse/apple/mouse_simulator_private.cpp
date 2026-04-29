#include "mouse_simulator_private.hpp"

#include <CoreFoundation/CoreFoundation.h>  // CF*

#include "mouse_event_converter.hpp"

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate() { destroy(); }

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

bool MouseSimulatorPrivate::initialize()
{
    // 无实际作用，仅与其他子模块保持一致性。`destroy()` 同理。
    bool expected = false;
    return isInitialized_.compare_exchange_strong(expected, true);
}

void MouseSimulatorPrivate::destroy()
{
    bool expected = true;
    isInitialized_.compare_exchange_strong(expected, false);
}

bool MouseSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool MouseSimulatorPrivate::sendEvent(const MouseEvent& event)
{
    if (!isInitialized_.load())
        return false;
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;
}

bool MouseSimulatorPrivate::moveTo(const AbsolutePos& absPos)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::moveBy(const RelativePos& relPos)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::pressButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::releaseButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::clickButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;
}

} // namespace hidtool
