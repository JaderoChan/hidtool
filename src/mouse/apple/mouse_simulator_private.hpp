#ifndef HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP

#include <atomic>   // atomic

#include <hidtool/mouse/mouse_simulator.hpp>

namespace hidt
{

class MouseSimulatorPrivate
{
public:
    static MouseSimulatorPrivate& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    bool moveTo(const AbsolutePos& absPos);
    bool moveBy(const RelativePos& relPos);

    bool wheel(int32_t wheelDelta);

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    bool clickButton(MouseButton button);

    bool wheel(const AbsolutePos& absPos, int32_t wheelDelta);
    bool pressButton(const AbsolutePos& absPos, MouseButton button);
    bool releaseButton(const AbsolutePos& absPos, MouseButton button);
    bool clickButton(const AbsolutePos& absPos, MouseButton button);

    bool drag(const AbsolutePos& endPos, MouseButton button);
    bool drag(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button);

private:
    MouseSimulatorPrivate() = default;
    ~MouseSimulatorPrivate();
    MouseSimulatorPrivate(const MouseSimulatorPrivate&) = delete;
    MouseSimulatorPrivate& operator=(const MouseSimulatorPrivate&) = delete;

    std::atomic<bool> isInitialized_{false};
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
