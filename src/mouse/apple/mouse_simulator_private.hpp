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

    static AbsolutePosRange getAbsoluteMoveRange();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

private:
    MouseSimulatorPrivate() = default;
    ~MouseSimulatorPrivate();
    MouseSimulatorPrivate(const MouseSimulatorPrivate&) = delete;
    MouseSimulatorPrivate& operator=(const MouseSimulatorPrivate&) = delete;

    std::atomic<bool> isInitialized_{false};
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
