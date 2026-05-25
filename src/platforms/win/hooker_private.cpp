#include "hooker_private.hpp"

namespace hidt
{

HookerPrivate::~HookerPrivate()
{
    // 由于工作线程循环中未调用纯虚函数，可以在基类析构函数中安全调用 stop() 以退出工作线程。
    stop();
    // 重置字段
    eventHandler_ = 0;
    userData_ = 0;
}

bool HookerPrivate::run()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (isRunning_.load())
        return false;

    std::promise<bool> runningResult;
    auto fut = runningResult.get_future();

    workerThread_ = std::thread([this, &runningResult]() { work(runningResult); });

    // 检查线程启动成功与否。
    bool runningSuccess = fut.get();
    isRunning_.store(runningSuccess);
    if (!runningSuccess)
    {
        // 如果线程启动失败则等待线程退出。
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

    // 向线程发送退出事件。
    PostThreadMessageA(workerThreadId_, WM_QUIT, 0, 0);
    // 等待线程退出。
    if (workerThread_.joinable())
        workerThread_.join();

    // 重置相关字段。
    workerThreadId_ = 0;
    isRunning_.store(false);
}

bool HookerPrivate::isRunning() const
{
    return isRunning_.load();
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
    // 强制系统创建消息循环。
    PeekMessageA(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    runningResult.set_value(true);

    // 只接收由 PostThreadMessage() 发送的消息。
    while (GetMessageA(&msg, reinterpret_cast<HWND>(static_cast<intptr_t>(-1)), 0, 0) != 0)
    {
        switch (msg.message)
        {
            case WM_SET_EVENT_HANDLER:
                eventHandler_ = static_cast<intptr_t>(msg.wParam);
                userData_ = static_cast<intptr_t>(msg.lParam);
                break;
            case WM_SET_USER_DATA:
                userData_ = static_cast<intptr_t>(msg.lParam);
                break;
            default:
                break;
        }
    }

    UnhookWindowsHookEx(hook);
}

} // namespace hidt
