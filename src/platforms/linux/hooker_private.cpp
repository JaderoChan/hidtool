#include "hooker_private.hpp"

#include <errno.h>

#include <dirent.h>         // dirent
#include <fcntl.h>          // open
#include <unistd.h>         // read, write, close

#include <sys/eventfd.h>    // eventfd
#include <sys/inotify.h>    // inotify
#include <sys/stat.h>       // stat

namespace hidtool
{

constexpr const char* EVDEV_DIR = "/dev/input/";

HookerPrivate::~HookerPrivate()
{
    WorkEvent event{WorkEvent::END};
    sendWorkEvent(event);

    if (workerThread_.joinable())
        workerThread_.join();

    close(watchedFds_[0].fd);
    watchedFds_[0].fd = -1;

    close(watchedFds_[1].fd);
    watchedFds_[1].fd = -1;

    for (auto it = watchedFds_.begin() + 2; it != watchedFds_.end();)
    {
        close(it->fd);
        it = watchedFds_.erase(it);
    }

    evdevNames_.clear();

    eventHandler_ = 0;
    workEvents_.clear();
}

bool HookerPrivate::run()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (isRunning_.load())
        return false;

    if (!setupAllFds())
        return false;

    workerThread_ = std::thread([this]() { work(); });
    isRunning_.store(true);

    return true;
}

void HookerPrivate::stop()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (!isRunning_.load())
        return;

    WorkEvent event{WorkEvent::END};
    sendWorkEvent(event);

    if (workerThread_.joinable())
        workerThread_.join();

    cleanupAllFds();

    isRunning_.store(false);
}

bool HookerPrivate::isRunning() const
{
    return isRunning_.load();
}

bool HookerPrivate::sendWorkEvent(const HookerPrivate::WorkEvent& event)
{
    std::lock_guard<std::mutex> locker(workEventsMtx_);

    workEvents_.push_back(event);

    int64_t wakeup = 1;
    ssize_t n = write(watchedFds_[0].fd /* Work event fd index is 0 */, &wakeup, 8);

    if (n != 8)
    {
        workEvents_.pop_back();
        return false;
    }
    return true;
}

bool HookerPrivate::addEvdevFd(const std::string& evdevName)
{
    std::string filepath = EVDEV_DIR + evdevName;

    // Only accept character devices.
    if (!isCharacterDevice(filepath))
        return false;

    int evdevFd = open(filepath.c_str(), O_RDONLY | O_NONBLOCK);
    if (evdevFd == -1)
        return false;

    // Only accept specified devices.
    if (!isAccessDevice(evdevFd))
    {
        close(evdevFd);
        return false;
    }

    struct pollfd pollFd;
    pollFd.fd = evdevFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;

    watchedFds_.push_back(pollFd);
    evdevNames_.push_back(evdevName);

    return true;
}

void HookerPrivate::removeEvdevFd(const std::string& evdevName)
{
    for (size_t i = 0; i < evdevNames_.size(); ++i)
    {
        if (evdevNames_[i] == evdevName)
        {
            int idx = i + 2;
            close(watchedFds_[idx].fd);
            watchedFds_.erase(watchedFds_.begin() + idx);
            evdevNames_.erase(evdevNames_.begin() + i);
            return;
        }
    }
}

bool HookerPrivate::setupWorkEventFd()
{
    int workEventFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (workEventFd == -1)
        return false;

    struct pollfd pollFd;
    pollFd.fd = workEventFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;
    watchedFds_[0] = pollFd;

    return true;
}

bool HookerPrivate::setupInotifyFd()
{
    int inotifyFd = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (inotifyFd == -1)
        return false;

    int wd = inotify_add_watch(inotifyFd, EVDEV_DIR, IN_CREATE | IN_DELETE);
    if (wd == -1)
    {
        close(inotifyFd);
        return false;
    }

    struct pollfd pollFd;
    pollFd.fd = inotifyFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;
    watchedFds_[1] = pollFd;

    return true;
}

bool HookerPrivate::setupEvdevFds()
{
    DIR* dir = opendir(EVDEV_DIR);
    if (!dir)
        return false;

    struct dirent* ent = readdir(dir);
    while (ent)
    {
        addEvdevFd(ent->d_name);
        ent = readdir(dir);
    }
    closedir(dir);

    return true;
}

bool HookerPrivate::setupAllFds()
{
    if (!setupWorkEventFd())
        return false;

    if (!setupInotifyFd())
    {
        cleanupWorkEventFd();
        return false;
    }

    if (!setupEvdevFds())
    {
        cleanupWorkEventFd();
        cleanupInotifyFd();
        return false;
    }

    return true;
}

void HookerPrivate::cleanupWorkEventFd()
{
    close(watchedFds_[0].fd);
    watchedFds_[0].fd = -1;
}

void HookerPrivate::cleanupInotifyFd()
{
    close(watchedFds_[1].fd);
    watchedFds_[1].fd = -1;
}

void HookerPrivate::cleanupEvdevFds()
{
    for (auto it = watchedFds_.begin() + 2; it != watchedFds_.end();)
    {
        close(it->fd);
        it = watchedFds_.erase(it);
    }

    evdevNames_.clear();
}

void HookerPrivate::cleanupAllFds()
{
    cleanupEvdevFds();
    cleanupInotifyFd();
    cleanupWorkEventFd();

    workEvents_.clear();
}

void HookerPrivate::handleWorkEvent(int fd, bool& shouldClose)
{
    uint64_t eventCount;
    ssize_t n = read(fd, &eventCount, 8);
    if (n == 8)
    {
        for (uint64_t i = 0; i < eventCount; ++i)
        {
            WorkEvent workEvent;
            bool hasEvent = false;
            {
                std::lock_guard<std::mutex> locker(workEventsMtx_);
                if (!workEvents_.empty())
                {
                    workEvent = workEvents_.front();
                    workEvents_.pop_front();
                    hasEvent = true;
                }
            }

            if (!hasEvent)
                break;

            switch (workEvent.type)
            {
                case WorkEvent::END:
                    shouldClose = true;
                    break;
                case WorkEvent::SET_EVENT_HANDLER:
                    eventHandler_ = workEvent.eventHandler;
                    break;
                default:
                    break;
            }
        }
    }
}

void HookerPrivate::handleEvdevChanged(int fd)
{
    alignas(struct inotify_event) char buf[4096];

    while (true)
    {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n > 0)
        {
            size_t off = 0;
            while (off + sizeof(struct inotify_event) <= static_cast<size_t>(n))
            {
                auto* ev = reinterpret_cast<struct inotify_event*>(buf + off);

                if ((ev->mask & IN_CREATE) && ev->len > 0)
                    addEvdevFd(ev->name);
                else if ((ev->mask & IN_DELETE) && ev->len > 0)
                    removeEvdevFd(ev->name);

                off += sizeof(struct inotify_event) + ev->len;
            }

            continue;
        }

        if (n == -1 && errno == EINTR)
            continue;

        break;
    }
}

void HookerPrivate::work()
{
    bool shouldClose = false;
    while (!shouldClose)
    {
        int ret = poll(watchedFds_.data(), watchedFds_.size(), -1);
        if (ret == -1)
            continue;

        // Handle work events.
        struct pollfd workEventPollfd = watchedFds_[0];
        if (workEventPollfd.revents & POLLIN)
            handleWorkEvent(workEventPollfd.fd, shouldClose);

        // Handle input devices changed event.
        struct pollfd inotifyPollfd = watchedFds_[1];
        if (inotifyPollfd.revents & POLLIN)
            handleEvdevChanged(inotifyPollfd.fd);

        // Handle input events.
        for (size_t i = 2; i < watchedFds_.size(); ++i)
        {
            struct pollfd evdevPollfd = watchedFds_[i];
            if (evdevPollfd.revents & POLLIN)
                handleInputEvent(evdevPollfd.fd);
        }
    }
}

bool HookerPrivate::isCharacterDevice(const std::string& filepath)
{
    struct stat st;
    if (stat(filepath.c_str(), &st) == -1)
        return false;
    return S_ISCHR(st.st_mode);
}

} // namespace hidtool
