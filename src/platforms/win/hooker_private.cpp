#include "hooker_private.hpp"

namespace hidtool
{

#define WM_SET_EVENT_HANDLER (WM_USER + 1)

HookerPrivate::~HookerPrivate()
{
    if (!isRunning_.load())
        return;

    PostThreadMessageA(workerThreadId_, WM_QUIT, 0, 0);
    if (workerThread_.joinable())
        workerThread_.join();

    workerThreadId_ = 0;
    isRunning_.store(false);
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

    PostThreadMessageA(workerThreadId_, WM_QUIT, 0, 0);
    if (workerThread_.joinable())
        workerThread_.join();

    isRunning_.store(false);
}

bool HookerPrivate::isRunning() const
{
    return isRunning_.load();
}

bool HookerPrivate::sendSetEventHandlerEvent(WPARAM eventHandler)
{
    return (PostThreadMessageA(
        workerThreadId_,
        WM_SET_EVENT_HANDLER,
        eventHandler,
        0) != 0);
}

void HookerPrivate::work(std::promise<bool>& runningResult)
{
    HHOOK hook = setWindowHook();
    if (!hook)
    {
        runningResult.set_value_at_thread_exit(false);
        return;
    }

    workerThreadId_ = GetCurrentThreadId();
    MSG msg = {0};
    // Force the system to create the message queue.
    PeekMessageA(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    runningResult.set_value(true);

    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by `PostThreadMessage()`.
    while (GetMessageA(&msg, reinterpret_cast<HWND>(static_cast<intptr_t>(-1)), 0, 0) != 0)
    {
        switch (msg.message)
        {
            case WM_SET_EVENT_HANDLER:
                handleSetEventHandlerEvent(msg.wParam);
                break;
            default:
                break;
        }
    }

    UnhookWindowsHookEx(hook);

    workerThreadId_ = 0;
}

} // namespace hidtool
