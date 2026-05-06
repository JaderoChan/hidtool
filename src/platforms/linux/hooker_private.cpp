#include "hooker_private.hpp"

#include <errno.h>          // errno

#include <dirent.h>         // dirent
#include <fcntl.h>          // open()
#include <unistd.h>         // read(), write(), close()

#include <sys/eventfd.h>    // eventfd
#include <sys/inotify.h>    // inotify
#include <sys/stat.h>       // stat

namespace hidt
{

// 输入设备文件夹
constexpr const char* EVDEV_DIR = "/dev/input/";

HookerPrivate::~HookerPrivate()
{
    // 重置字段
    eventHandler_ = 0;
}

bool HookerPrivate::run()
{
    std::lock_guard<std::mutex> locker(operateMtx_);

    if (isRunning_.load())
        return false;

    // 配置所有文件描述符并将其加入至 poll 监控列表中。
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

    // 发送线程终止事件。
    WorkEvent event{WorkEvent::END};
    sendWorkEvent(event);

    // 等待线程退出。
    if (workerThread_.joinable())
        workerThread_.join();

    // 关闭所有文件描述符并重置 poll 监控列表。
    cleanupAllFds();

    {
        // 清空事件列表。
        std::lock_guard<std::mutex> locker(workEventsMtx_);
        workEvents_.clear();
    }

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
    ssize_t n = write(watchedPollfds_[0].fd /* Work event pollfd index is 0 */, &wakeup, 8);

    if (n != 8)
    {
        // 如果事件写入失败，回退事件列表至原本状态。
        workEvents_.pop_back();
        return false;
    }
    return true;
}

bool HookerPrivate::addEvdevFd(const std::string& evdevName)
{
    std::string filepath = EVDEV_DIR + evdevName;

    // 仅允许字符设备。
    if (!isCharacterDevice(filepath))
        return false;

    int evdevFd = open(filepath.c_str(), O_RDONLY | O_NONBLOCK);
    if (evdevFd == -1)
        return false;

    // 仅允许需要监听的设备类型。
    if (!isAccessDevice(evdevFd))
    {
        close(evdevFd);
        return false;
    }

    struct pollfd pollFd;
    pollFd.fd = evdevFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;

    // 将输入设备的文件描述符添加至 poll 监控列表中。
    watchedPollfds_.push_back(pollFd);
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
            // 关闭文件描述符。
            close(watchedPollfds_[idx].fd);
            // 删除 poll 监控列表和设备名称列表中的对应项。
            watchedPollfds_.erase(watchedPollfds_.begin() + idx);
            evdevNames_.erase(evdevNames_.begin() + i);
            return;
        }
    }
}

bool HookerPrivate::setupWorkEventFd()
{
    // 配置一个事件文件描述符。
    int workEventFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (workEventFd == -1)
        return false;

    struct pollfd pollFd;
    pollFd.fd = workEventFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;

    // 添加至 poll 监控列表中。
    watchedPollfds_[0] = pollFd;

    return true;
}

bool HookerPrivate::setupInotifyFd()
{
    int inotifyFd = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (inotifyFd == -1)
        return false;

    // 监听输入设备文件夹变化。
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

    // 添加至 poll 监控列表中。
    watchedPollfds_[1] = pollFd;

    return true;
}

bool HookerPrivate::setupEvdevFds()
{
    DIR* dir = opendir(EVDEV_DIR);
    if (!dir)
        return false;

    // 读取输入设备文件夹。
    struct dirent* ent = readdir(dir);
    while (ent)
    {
        // 尝试添加其中的输入设备。
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
    close(watchedPollfds_[0].fd);
    watchedPollfds_[0].fd = -1;
}

void HookerPrivate::cleanupInotifyFd()
{
    close(watchedPollfds_[1].fd);
    watchedPollfds_[1].fd = -1;
}

void HookerPrivate::cleanupEvdevFds()
{
    for (auto it = watchedPollfds_.begin() + 2; it != watchedPollfds_.end();)
    {
        close(it->fd);
        it = watchedPollfds_.erase(it);
    }

    evdevNames_.clear();
}

void HookerPrivate::cleanupAllFds()
{
    cleanupEvdevFds();
    cleanupInotifyFd();
    cleanupWorkEventFd();
}

void HookerPrivate::handleWorkEvent(int fd, bool& shouldClose)
{
    uint64_t eventCount;
    ssize_t n = read(fd, &eventCount, 8);

    if (n == 8)
    {
        // 读取工作事件队列。
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
    alignas(struct inotify_event) char buf[1024];

    while (true)
    {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n > 0)
        {
            size_t off = 0;
            // 读取所有文件变化。
            while (off + sizeof(struct inotify_event) <= static_cast<size_t>(n))
            {
                auto* ev = reinterpret_cast<struct inotify_event*>(buf + off);

                // 如果有新的输入设备，尝试添加。
                if ((ev->mask & IN_CREATE) && ev->len > 0)
                    addEvdevFd(ev->name);
                // 如果有输入设备被移除，尝试删除。
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
        // 监控文件描述符。
        int ret = poll(watchedPollfds_.data(), watchedPollfds_.size(), -1);
        if (ret == -1)
            continue;

        // 处理工作事件。
        struct pollfd workEventPollfd = watchedPollfds_[0];
        if (workEventPollfd.revents & POLLIN)
            handleWorkEvent(workEventPollfd.fd, shouldClose);

        // 处理输入设备发生变化。
        struct pollfd inotifyPollfd = watchedPollfds_[1];
        if (inotifyPollfd.revents & POLLIN)
            handleEvdevChanged(inotifyPollfd.fd);

        // 处理输入设备的输入事件。
        for (size_t i = 2; i < watchedPollfds_.size(); ++i)
        {
            struct pollfd evdevPollfd = watchedPollfds_[i];
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

} // namespace hidt
