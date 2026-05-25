#include "key_state_getter.hpp"

#include <string>

#include <errno.h>          // errno
#include <dirent.h>         // dirent ...
#include <fcntl.h>          // open()
#include <unistd.h>         // read(), write(), close()
#include <sys/ioctl.h>      // ioctl
#include <sys/inotify.h>    // inotify
#include <sys/stat.h>       // stat

#include <linux/input.h>    // EVIOCGBIT

namespace hidt
{

constexpr const char* EVDEV_DIR = "/dev/input/";

static bool isCharacterDevice(const std::string& filepath)
{
    struct stat st;
    if (stat(filepath.c_str(), &st) == -1)
        return false;
    return S_ISCHR(st.st_mode);
}

static bool isKeyDevice(int fd)
{
    // 获取输入设备具备的事件。
    unsigned long evBits = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), &evBits) == -1)
        return false;

    return ((evBits & (1u << EV_KEY)) != 0);
}

KeyStateGetter::KeyStateGetter()
{
    inotifyFd_ = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (inotifyFd_ != -1)
        inotify_add_watch(inotifyFd_, EVDEV_DIR, IN_CREATE | IN_DELETE);
    refresh();
}

KeyStateGetter::~KeyStateGetter()
{
    for (int fd : fds_)
        close(fd);
    if (inotifyFd_ != -1)
        close(inotifyFd_);
}

bool KeyStateGetter::isKeyPressed(int32_t nativeKey)
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (needsRefresh_ || drainInotify())
        refresh();

    for (int fd : fds_)
    {
        int rc = ioctl(fd, EVIOCGKEY(sizeof(keyStateBits_)), keyStateBits_);
        if (rc == -1)
        {
            if (errno == ENODEV || errno == ENXIO)
                needsRefresh_ = true;
            continue;
        }

        if (keyStateBits_[nativeKey / 8] & (1 << (nativeKey % 8)))
            return true;
    }

    return false;
}

bool KeyStateGetter::drainInotify()
{
    if (inotifyFd_ == -1)
        return false;

    alignas(struct inotify_event) char buf[sizeof(struct inotify_event) + NAME_MAX + 1];
    bool changed = false;
    while (read(inotifyFd_, buf, sizeof(buf)) > 0)
        changed = true;
    return changed;
}

void KeyStateGetter::refresh()
{
    for (int fd : fds_)
        close(fd);

    fds_.clear();
    needsRefresh_ = false;

    DIR* dir = opendir(EVDEV_DIR);
    if (!dir)
        return;

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        std::string path = std::string(EVDEV_DIR) + ent->d_name;
        if (!isCharacterDevice(path))
            continue;

        int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
        if (fd == -1)
            continue;

        if (!isKeyDevice(fd))
        {
            close(fd);
            continue;
        }

        fds_.push_back(fd);
    }

    closedir(dir);
}

} // namespace hidt
