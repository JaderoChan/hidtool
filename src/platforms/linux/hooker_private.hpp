#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <cstdint>
#include <atomic>
#include <future>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <poll.h>   // pollfd

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

private:
    struct WorkEvent
    {
        enum Type : uint8_t
        {
            END,
            SET_EVENT_HANDLER
        };

        Type type;
        intptr_t eventHandler;
    };

    void addWorkEvent(const WorkEvent& event);

    bool addEvdevFd(const std::string& evdevName);
    void removeEvdevFd(const std::string& evdevName);

    bool setupWorkEventFd();
    bool setupInotifyFd();
    bool setupEvdevFds();
    bool setupAllFds();

    void cleanupWorkEventFd();
    void cleanupInotifyFd();
    void cleanupEvdevFds();
    void cleanupAllFds();

    void handleEvdevChanged();
    void handleInputEvent(int evdevFd);

    void work();

    std::list<WorkEvent> workEvents_;
    mutable std::mutex workEventsMtx_;

    // watchedFds layout:
    // [0] work event fd: see also work event.
    // [1] inotify fd: Detect evdev device changes.
    // [2..] opened evdev keyboard fds.
    // Reserve 2 pollfd for 'work event fd' and 'inotify fd'.
    std::vector<struct pollfd> watchedFds{2, pollfd{-1, 0, 0}};
    std::vector<std::string> evdevNames;

    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;
};

} // namespace hidtool

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
