#ifndef HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP

#include <atomic>   // atomic
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread

#include <windows.h>    // DWORD

#include <hidtool/mouse/mouse_simulator.hpp>
#include "virtual_screen_info.hpp"

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

    void handleDisplayChanged();

    // 用于互斥 `initialize()` 和 `destroy()` 操作。
    mutable std::mutex initDestrMtx_;

    std::atomic<bool> isInitialized_{false};
    std::atomic<VirtualScreenInfo> virtualScreenInfo_;
    std::thread displayChangedHandlerThread_;
    DWORD displayChangedHandlerThreadId_ = 0;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_SIMULATOR_PRIVATE_HPP
