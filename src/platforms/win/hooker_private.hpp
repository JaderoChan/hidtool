#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <atomic>   // atomic
#include <future>   // promise, future
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread

#include <windows.h>    // WM_USER, DWORD, HHOOK, GetCurrentThreadId(), Message... Hook...

namespace hidtool
{

constexpr UINT WM_SET_EVENT_HANDLER = (WM_USER + 1);

class HookerPrivate
{
public:
    HookerPrivate() = default;
    virtual ~HookerPrivate();

    bool run();
    void stop();
    bool isRunning() const;

protected:
    // 提供一个抽象的事件处理程序设置接口，供派生类调用。
    template <typename T>
    bool setEventHandler(T eventHandler)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        // 如果当前 Hooker 未启动，直接设置事件处理程序。
        if (!isRunning_.load())
        {
            eventHandler_ = reinterpret_cast<intptr_t>(eventHandler);
            return true;
        }

        // 否则，发送事件处理程序设置事件。
        return (PostThreadMessageA(
            workerThreadId_,
            WM_SET_EVENT_HANDLER,
            reinterpret_cast<intptr_t>(eventHandler),
            0) != 0);
    }

    // 提供事件处理程序获取接口，供派生类调用。
    template <typename T>
    T getEventHandler() { return reinterpret_cast<T>(eventHandler_); }

    virtual HHOOK setWindowHook() = 0;

private:
    void work(std::promise<bool>& runningResult);

    intptr_t eventHandler_ = 0;

    // 用于互斥 `run()`，`stop()` 和 `setEventHandler()` 操作。
    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;

    DWORD workerThreadId_ = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
