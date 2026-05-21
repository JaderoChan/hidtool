#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <atomic>   // atomic
#include <future>   // promise, future
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

namespace hidt
{

class HookerPrivate
{
public:
    HookerPrivate() = default;
    virtual ~HookerPrivate();

    bool run();
    void stop();
    bool isRunning() const;

protected:
    template <typename T>
    bool setEventHandler(T eventHandler)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        eventHandler_.store(reinterpret_cast<intptr_t>(eventHandler));
        return true;
    }

    template <typename T>
    T getEventHandler() { return reinterpret_cast<T>(eventHandler_.load()); }

    virtual CGEventMask getCGEventMask() const = 0;
    virtual CGEventTapCallBack getCGEventTapCallback() const = 0;

private:
    void work(std::promise<bool>& runningResult);

    std::atomic<intptr_t> eventHandler_{0};

    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;

    CFRunLoopRef runLoop_ = nullptr;

};

} // namespace hidt

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
