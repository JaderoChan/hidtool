#include "uinput.hpp"

#include <cerrno>   // errno
#include <cstdio>   // sprintf()

#include <fcntl.h>          // open()
#include <unistd.h>         // write(), close()
#include <sys/ioctl.h>      // ioctl()

#include <linux/uinput.h>   // uinput...

namespace hidtool
{

UInput::~UInput() { cleanup(); }

bool UInput::setup(const std::string& name, uint16_t vendor, uint16_t product, uint16_t version)
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);

    // 如果 `uinputFd_` 不为 -1，意味着已经是配置完成状态，直接返回 `false`。
    if (uinputFd_ != -1)
        return false;

    // UInput文件位置。
    constexpr const char* uinputFilepath = "/dev/uinput";
    uinputFd_ = open(uinputFilepath, O_WRONLY | O_NONBLOCK);
    if (uinputFd_ == -1)
        return false;

    // 配置 UInput 属性。
    struct uinput_setup usetup = {0};
    snprintf(usetup.name, sizeof(usetup.name), "%s", name.c_str());
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = vendor;
    usetup.id.product = product;
    usetup.id.version = version;

    // 配置 UInput -> 派生类配置 UInput 功能 -> 创建 UInput。
    if ((ioctl(uinputFd_, UI_DEV_SETUP, &usetup) == -1) ||
        !setupUInputFd(uinputFd_) ||
        ioctl(uinputFd_, UI_DEV_CREATE) == -1)
    {
        ioctl(uinputFd_, UI_DEV_DESTROY);
        close(uinputFd_);
        uinputFd_ = -1;
        return false;
    }

    return true;
}

void UInput::cleanup()
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);

    if (uinputFd_ != -1)
    {
        ioctl(uinputFd_, UI_DEV_DESTROY);
        close(uinputFd_);
        uinputFd_ = -1;
    }
}

bool UInput::isSetup() const
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);
    return uinputFd_ != -1;
}

int UInput::uinputFd() const
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);
    return uinputFd_;
}

bool UInput::sendEvent(const struct input_event* ies, size_t count)
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);

    if (uinputFd_ == -1 || !ies || count == 0)
        return 0;

    const uint8_t *p = reinterpret_cast<const uint8_t*>(ies);
    size_t total = count * sizeof(struct input_event);
    size_t written = 0;

    while (written < total)
    {
        ssize_t n = write(uinputFd_, p + written, total - written);

        if (n > 0)
        {
            written += static_cast<size_t>(n);
            continue;
        }

        if (n == -1 && (errno == EINTR || errno == EAGAIN))
            continue;

        break;
    }

    return written == total;
}

} // namespace hidtool
