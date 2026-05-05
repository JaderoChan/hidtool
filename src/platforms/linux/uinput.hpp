#ifndef HIDTOOL_UINPUT_HPP
#define HIDTOOL_UINPUT_HPP

#include <cstddef>  // size_t
#include <cstdint>  // uint16_t
#include <string>   // string
#include <mutex>    // mutex, lock_guard

#include <linux/input.h>    // input_event

namespace hidt
{

class UInput
{
public:
    UInput() = default;
    virtual ~UInput();

    bool setup(const std::string& name, uint16_t vendor, uint16_t product, uint16_t version);
    void cleanup();

    bool isSetup() const;
    int uinputFd() const;

    bool sendEvent(const struct input_event* ies, size_t count);

protected:
    virtual bool setupUInputFd(int fd) = 0;

private:
    int uinputFd_ = -1;
    // 用于互斥涉及到 `uinputFd_` 读写的操作。
    mutable std::mutex uinputFdMtx_;
};

} // namespace hidt

#endif // !HIDTOOL_UINPUT_HPP
