#ifndef HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP

#include <atomic>

#include <hidtool/mouse/mouse_simulator.hpp>

namespace hidtool
{

class MouseSimulatorPrivate
{
public:
    static MouseSimulatorPrivate& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const std::vector<MouseEvent>& events);

    bool moveTo(int32_t x, int32_t y);
    bool moveBy(int32_t dx, int32_t dy);

    bool wheel(int32_t wheelDelta);

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    bool clickButton(MouseButton button);

private:
    MouseSimulatorPrivate() = default;
    ~MouseSimulatorPrivate();
    MouseSimulatorPrivate(const MouseSimulatorPrivate&) = delete;
    MouseSimulatorPrivate& operator=(const MouseSimulatorPrivate&) = delete;

    std::atomic<bool> isInitialized_{false};
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
