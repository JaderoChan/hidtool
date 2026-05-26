#include <hidtool/mouse/mouse_simulator.hpp>

#if defined(HIDTOOL_WIN)
    #include "win/mouse_simulator_private.hpp"
#elif defined(HIDTOOL_APPLE)
    #include "macos/mouse_simulator_private.hpp"
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

bool MouseSimulator::clickButton(MouseButton button, uint64_t interval)
{
    MouseEvent events[3] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::doubleClickButton(MouseButton button, uint64_t interval1, uint64_t interval2)
{
    MouseEvent events[7] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval1),
        MouseEvent::createReleaseButtonEvent(button),

        MouseEvent::createSleepEvent(interval2),

        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval1),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 7) == 7;
}

bool MouseSimulator::wheel(const AbsolutePos& absPos, int32_t wheelDelta, uint64_t interval)
{
    MouseEvent events[3] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createWheelEvent(wheelDelta)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::pressButton(const AbsolutePos& absPos, MouseButton button, uint64_t interval)
{
    MouseEvent events[3] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createPressButtonEvent(button)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::releaseButton(const AbsolutePos& absPos, MouseButton button, uint64_t interval)
{
    MouseEvent events[3] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 3) == 3;
}

bool MouseSimulator::clickButton(const AbsolutePos& absPos, MouseButton button,
    uint64_t interval1, uint64_t interval2)
{
    MouseEvent events[5] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createSleepEvent(interval1),

        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval2),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 5) == 5;
}

bool MouseSimulator::doubleClickButton(const AbsolutePos& absPos, MouseButton button,
    uint64_t interval1, uint64_t interval2, uint64_t interval3)
{
    MouseEvent events[9] = {
        MouseEvent::createAbsMoveEvent(absPos),
        MouseEvent::createSleepEvent(interval1),

        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval2),
        MouseEvent::createReleaseButtonEvent(button),

        MouseEvent::createSleepEvent(interval3),

        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval2),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 9) == 9;
}

bool MouseSimulator::dragTo(const AbsolutePos& absPos, MouseButton button)
{ return pri_.sendEvent(MouseEvent::createDragEvent(absPos, button)); }

bool MouseSimulator::dragCombo(const AbsolutePos& endPos,
    MouseButton button, uint64_t interval)
{
    MouseEvent events[5] = {
        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createDragEvent(endPos, button),
        MouseEvent::createSleepEvent(interval),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 5) == 5;
}

bool MouseSimulator::dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos,
    MouseButton button, uint64_t interval1, uint64_t interval2)
{
    MouseEvent events[7] = {
        MouseEvent::createAbsMoveEvent(startPos),
        MouseEvent::createSleepEvent(interval1),

        MouseEvent::createPressButtonEvent(button),
        MouseEvent::createSleepEvent(interval2),

        MouseEvent::createDragEvent(endPos, button),
        MouseEvent::createSleepEvent(interval2),
        MouseEvent::createReleaseButtonEvent(button)
    };

    return pri_.sendEvent(events, 7) == 7;
}

} // namespace hidt
