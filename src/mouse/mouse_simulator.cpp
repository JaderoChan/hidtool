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

namespace hidt
{

MouseSimulator::MouseSimulator(MouseSimulatorPrivate& pri)
    : pri_(pri) {}

MouseSimulator& MouseSimulator::getInstance()
{
    static MouseSimulator instance(MouseSimulatorPrivate::getInstance());
    return instance;
}

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

bool MouseSimulator::moveTo(const AbsolutePos& absPos)
{ return pri_.moveTo(absPos); }

bool MouseSimulator::moveBy(const RelativePos& relPos)
{ return pri_.moveBy(relPos); }

bool MouseSimulator::wheel(int32_t wheelDelta)
{ return pri_.wheel(wheelDelta); }

bool MouseSimulator::pressButton(MouseButton button)
{ return pri_.pressButton(button); }

bool MouseSimulator::releaseButton(MouseButton button)
{ return pri_.releaseButton(button); }

bool MouseSimulator::clickButton(MouseButton button)
{ return pri_.clickButton(button); }

bool MouseSimulator::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{ return pri_.wheel(absPos, wheelDelta); }

bool MouseSimulator::pressButton(const AbsolutePos& absPos, MouseButton button)
{ return pri_.pressButton(absPos, button); }

bool MouseSimulator::releaseButton(const AbsolutePos& absPos, MouseButton button)
{ return pri_.releaseButton(absPos, button); }

bool MouseSimulator::clickButton(const AbsolutePos& absPos, MouseButton button)
{ return pri_.clickButton(absPos, button); }

bool MouseSimulator::dragTo(const AbsolutePos& absPos, MouseButton button)
{ return pri_.dragTo(absPos, button); }

bool MouseSimulator::dragCombo(const AbsolutePos& endPos, MouseButton button)
{ return pri_.dragCombo(endPos, button); }

bool MouseSimulator::dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{ return pri_.dragCombo(startPos, endPos, button); }

} // namespace hidt
