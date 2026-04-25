#ifndef HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP

#include <atomic>
#include <mutex>
#include <thread>

#include <windows.h>

#include <hidtool/mouse/mouse_simulator.hpp>
#include "virtual_screen_info.hpp"

namespace hidtool
{

class MouseSimulatorPrivate
{
public:
    static MouseSimulatorPrivate& getInstance();

    static MouseSimulator::AbsMoveRange getAbsMoveRange();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    bool moveBy(int32_t dx, int32_t dy);
    bool moveTo(int32_t x, int32_t y);

    bool wheel(int32_t wheelDelta);

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    bool clickButton(MouseButton button);

private:
    MouseSimulatorPrivate() = default;
    ~MouseSimulatorPrivate();
    MouseSimulatorPrivate(const MouseSimulatorPrivate&) = delete;
    MouseSimulatorPrivate& operator=(const MouseSimulatorPrivate&) = delete;

    void handleDisplayChanged();

    mutable std::mutex initDestrMtx_;

    std::atomic<bool> isInitialized_{false};
    std::atomic<VirtualScreenInfo> virtualScreenInfo_;
    std::thread displayChangedHandlerThread_;
    DWORD displayChangedHandlerThreadId_ = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
