#include <hidtool/mouse/mouse_simulator.hpp>

#if defined(HIDTOOL_WIN)
    #include "win/mouse_simulator_private.hpp"
#elif defined(HIDTOOL_APPLE)
    #include "apple/mouse_simulator_private.hpp"
#elif defined(HIDTOOL_LINUX)
    #include "linux/mouse_simulator_private.hpp"
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN

namespace hidtool
{

MouseSimulator::MouseSimulator(MouseSimulatorPrivate& pri)
    : pri_(pri) {}

MouseSimulator& MouseSimulator::getInstance()
{
    static MouseSimulator instance(MouseSimulatorPrivate::getInstance());
    return instance;
}

MouseSimulator::AbsMoveRange MouseSimulator::getAbsMoveRange()
{ return MouseSimulatorPrivate::getAbsMoveRange(); }

bool MouseSimulator::initialize()
{ return pri_.initialize(); }

void MouseSimulator::destroy()
{ pri_.destroy(); }

bool MouseSimulator::isInitialized() const
{ return pri_.isInitialized(); }

bool MouseSimulator::sendEvent(const MouseEvent& event)
{ return pri_.sendEvent(event); }

size_t MouseSimulator::sendEvent(const MouseEvent* events, size_t count)
{ return pri_.sendEvent(events, count); }

bool MouseSimulator::moveBy(int32_t dx, int32_t dy)
{ return pri_.moveBy(dx, dy); }

bool MouseSimulator::moveTo(int32_t x, int32_t y)
{ return pri_.moveTo(x, y); }

bool MouseSimulator::wheel(int32_t wheelDelta)
{ return pri_.wheel(wheelDelta); }

bool MouseSimulator::pressButton(MouseButton button)
{ return pri_.pressButton(button); }

bool MouseSimulator::releaseButton(MouseButton button)
{ return pri_.releaseButton(button); }

bool MouseSimulator::clickButton(MouseButton button)
{ return pri_.clickButton(button); }

} // namespace hidtool
