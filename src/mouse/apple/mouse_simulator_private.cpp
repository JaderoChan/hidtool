#include "mouse_simulator_private.hpp"

#include "mouse_event_converter.hpp"

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate()
{
    if (!isInitialized_.load())
        return;

    isInitialized_.store(false);
}

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

bool MouseSimulatorPrivate::initialize()
{
    bool expected = false;
    if (!isInitialized_.compare_exchange_strong(expected, true))
        return false;

    return true;
}

void MouseSimulatorPrivate::destroy()
{
    bool expected = true;
    if (!isInitialized_.compare_exchange_strong(expected, false))
        return;
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

size_t MouseSimulatorPrivate::sendEvent(const std::vector<MouseEvent>& events)
{
    if (!isInitialized_.load())
        return 0;
}

bool MouseSimulatorPrivate::moveBy(int32_t dx, int32_t dy)
{
    if (!isInitialized_.load())
        return false;
}

bool MouseSimulatorPrivate::moveTo(int32_t x, int32_t y)
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

} // namespace hidtool
