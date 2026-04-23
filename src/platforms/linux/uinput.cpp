#include "uinput.hpp"

#include <cerrno>
#include <cstdio>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/uinput.h>
#include <linux/input-event-codes.h>

namespace hidtool
{

UInput::~UInput()
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);

    if (uinputFd_ != -1)
    {
        ioctl(uinputFd_, UI_DEV_DESTROY);
        close(uinputFd_);
        uinputFd_ = -1;
    }
}

bool UInput::setup(const std::string& name, uint16_t vendor, uint16_t product, uint16_t version)
{
    std::lock_guard<std::mutex> locker(uinputFdMtx_);

    if (uinputFd_ != -1)
        return false;

    constexpr const char* uinputFilepath = "/dev/uinput";
    uinputFd_ = open(uinputFilepath, O_WRONLY | O_NONBLOCK);
    if (uinputFd_ == -1)
        return false;

    struct uinput_setup usetup = {0};
    snprintf(usetup.name, sizeof(usetup.name), "%s", name.c_str());
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = vendor;
    usetup.id.product = product;
    usetup.id.version = version;

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

        if (n == -1 && errno == EINTR)
            continue;   // Interrupted by signal, retry write.

        if (n == -1 && errno == EAGAIN)
            continue;

        break;
    }

    return written == total;
}

} // namespace hidtool
