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
    bool sendSetEventHandlerEvent(WPARAM eventHandler);

    virtual HHOOK setWindowHook() = 0;
    virtual void handleSetEventHandlerEvent(WPARAM eventHandler) = 0;

    mutable std::mutex operateMtx_;

private:
    void work(std::promise<bool>& runningResult);

    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;
    DWORD workerThreadId_ = 0;
};

} // namespace hidtool

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
