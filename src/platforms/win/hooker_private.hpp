#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <atomic>   // atomic
#include <future>   // promise, future
#include <mutex>    // mutex, lock_guard
#include <thread>   // thread

#include <windows.h>    // WM_USER, DWORD, HHOOK, GetCurrentThreadId(), Message ... Hook ...

namespace hidt
{

constexpr UINT WM_SET_EVENT_HANDLER = (WM_USER + 1);
constexpr UINT WM_SET_USER_DATA     = (WM_USER + 2);

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
    bool setEventHandler(T eventHandler, void* userData = nullptr)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        // 如果当前 Hooker 未启动，直接设置事件处理程序。
        if (!isRunning_.load())
        {
            eventHandler_ = reinterpret_cast<intptr_t>(eventHandler);
            userData_ = reinterpret_cast<intptr_t>(userData);
            return true;
        }

        // 否则，发送事件处理程序设置事件。
        return (PostThreadMessageA(
            workerThreadId_,
            WM_SET_EVENT_HANDLER,
            static_cast<WPARAM>(reinterpret_cast<intptr_t>(eventHandler)),
            static_cast<LPARAM>(reinterpret_cast<intptr_t>(userData))) != 0);
    }

    bool setUserData(void* userData)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        if (!isRunning_.load())
        {
            userData_ = reinterpret_cast<intptr_t>(userData);
            return true;
        }

        return (PostThreadMessageA(
            workerThreadId_,
            WM_SET_USER_DATA,
            0,
            static_cast<LPARAM>(reinterpret_cast<intptr_t>(userData))) != 0);
    }

    // 提供事件处理程序获取接口，供派生类调用。
    template <typename T>
    T getEventHandler() { return reinterpret_cast<T>(eventHandler_); }

    void* getUserData() const { return reinterpret_cast<void*>(userData_); }

    virtual HHOOK setWindowHook() = 0;

private:
    void work(std::promise<bool>& runningResult);

    intptr_t eventHandler_ = 0;
    intptr_t userData_ = 0;

    // 用于互斥 run()，stop() 和 setEventHandler() 的执行。
    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;

    DWORD workerThreadId_ = 0;
};

} // namespace hidt

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
