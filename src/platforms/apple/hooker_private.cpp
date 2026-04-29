#include "hooker_private.hpp"

namespace hidtool
{

HookerPrivate::~HookerPrivate()
{
    // 重置字段
    eventHandler_.store(0);
}

bool HookerPrivate::run()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (isRunning_.load())
        return false;

    std::promise<bool> runningResult;
    auto fut = runningResult.get_future();

    workerThread_ = std::thread([this, &runningResult]() { work(runningResult); });

    bool runningSuccess = fut.get();
    isRunning_.store(runningSuccess);
    if (!runningSuccess)
    {
        if (workerThread_.joinable())
            workerThread_.join();
    }

    return runningSuccess;
}

void HookerPrivate::stop()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (!isRunning_.load())
        return;

    // TODO：是否线程安全？
    CFRunLoopStop(runLoop_.load());

    if (workerThread_.joinable())
        workerThread_.join();

    // 重置相关字段。
    runLoop_.store(nullptr);
    isRunning_.store(false);
}

bool HookerPrivate::isRunning() const
{
    return isRunning_.load();
}

void HookerPrivate::work(std::promise<bool>& runningResult)
{
    auto currentLoop = CFRunLoopGetCurrent();
    if (!currentLoop)
    {
        runningResult.set_value_at_thread_exit(false);
        return;
    }

    runLoop_.store(currentLoop);

    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        getCGEventMask(),
        getCGEventTapCallback(),
        nullptr);

    if (!eventTap)
    {
        runLoop_.store(nullptr);
        runningResult.set_value_at_thread_exit(false);
        return;
    }

    CFRunLoopSourceRef runLoopSource =
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    if (!runLoopSource)
    {
        CFRelease(eventTap);
        runLoop_.store(nullptr);
        runningResult.set_value_at_thread_exit(false);
        return;
    }

    CFRunLoopAddSource(runLoop_.load(), runLoopSource, kCFRunLoopDefaultMode);
    CGEventTapEnable(eventTap, true);

    runningResult.set_value(true);

    CFRunLoopRun();
}

} // namespace hidtool
