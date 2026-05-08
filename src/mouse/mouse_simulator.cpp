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

AbsolutePosRange MouseSimulator::getAbsoluteMoveRange()
{ return MouseSimulatorPrivate::getAbsoluteMoveRange(); }

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
{ return pri_.sendEvent(MouseEvent::createAbsMoveEvent(absPos)); }

bool MouseSimulator::moveBy(const RelativePos& relPos)
{ return pri_.sendEvent(MouseEvent::createRelMoveEvent(relPos)); }

bool MouseSimulator::wheel(int32_t wheelDelta)
{ return pri_.sendEvent(MouseEvent::createWheelEvent(wheelDelta)); }

bool MouseSimulator::pressButton(MouseButton button)
{ return pri_.sendEvent(MouseEvent::createPressButtonEvent(button)); }

bool MouseSimulator::releaseButton(MouseButton button)
{ return pri_.sendEvent(MouseEvent::createReleaseButtonEvent(button)); }

bool MouseSimulator::clickButton(MouseButton button)
{
    MouseEvent events[2] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 2) == 2;
}

bool MouseSimulator::doubleClickButton(MouseButton button)
{
    MouseEvent events[4] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button),
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 4) == 4;
}

bool MouseSimulator::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{
    MouseEvent events[2] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createWheelEvent(wheelDelta)
    };

    return pri_.sendEvent(events, 2) == 2;
}

bool MouseSimulator::pressButton(const AbsolutePos& absPos, MouseButton button)
{
    MouseEvent events[2] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createPressButtonEvent(button)
    };

    return pri_.sendEvent(events, 2) == 2;
}

bool MouseSimulator::releaseButton(const AbsolutePos& absPos, MouseButton button)
{
    MouseEvent events[2] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 2) == 2;
}

bool MouseSimulator::clickButton(const AbsolutePos& absPos, MouseButton button)
{
    MouseEvent events[3] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::doubleClickButton(const AbsolutePos& absPos, MouseButton button)
{
    MouseEvent events[5] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button),
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 5) == 5;
}

bool MouseSimulator::dragTo(const AbsolutePos& absPos, MouseButton button)
{ return pri_.sendEvent(MouseEvent::createDragEvent(absPos, button)); }

bool MouseSimulator::dragCombo(const AbsolutePos& endPos, MouseButton button)
{
    MouseEvent events[3] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createDragEvent(endPos, button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{
    MouseEvent events[4] = {
        MouseEvent::createAbsMoveEvent(startPos),
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createDragEvent(endPos, button),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 4) == 4;
}

} // namespace hidt
