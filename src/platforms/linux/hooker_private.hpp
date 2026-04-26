#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <cstdint>
#include <atomic>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <poll.h>           // pollfd
#include <linux/input.h>    // input_event

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
    template <typename T>
    bool setEventHandler(T eventHandler)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        if (!isRunning_.load())
        {
            eventHandler_ = reinterpret_cast<intptr_t>(eventHandler);
            return true;
        }

        WorkEvent event{WorkEvent::SET_EVENT_HANDLER, reinterpret_cast<intptr_t>(eventHandler)};
        return sendWorkEvent(event);
    }

    template <typename T>
    T getEventHandler() { return reinterpret_cast<T>(eventHandler_); }

    virtual bool isAccessDevice(int fd) = 0;
    virtual void handleInputEvent(int fd) = 0;

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

    bool sendWorkEvent(const WorkEvent& event);

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

    void handleWorkEvent(int fd, bool& shouldClose);
    void handleEvdevChanged(int fd);

    void work();

    static bool isCharacterDevice(const std::string& filepath);

    std::list<WorkEvent> workEvents_;
    mutable std::mutex workEventsMtx_;

    // watchedFds layout:
    // [0] work event fd: see also work event.
    // [1] inotify fd: Detect evdev device changes.
    // [2..] opened evdev keyboard fds.
    // Reserve 2 pollfd for 'work event fd' and 'inotify fd'.
    std::vector<struct pollfd> watchedFds_{2, pollfd{-1, 0, 0}};
    std::vector<std::string> evdevNames_;

    intptr_t eventHandler_ = 0;

    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;
};

} // namespace hidtool

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
