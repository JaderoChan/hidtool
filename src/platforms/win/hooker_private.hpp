#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <atomic>
#include <future>
#include <mutex>
#include <thread>

#include <windows.h>

namespace hidtool
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
    bool setEventHandler(intptr_t eventHandler);

    template <typename T>
    static T getEventHandler() { return reinterpret_cast<T>(eventHandler_); }

    virtual HHOOK setWindowHook() = 0;

private:
    void work(std::promise<bool>& runningResult);

    static intptr_t eventHandler_;

    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;
    DWORD workerThreadId_ = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
