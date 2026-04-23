#ifndef HIDTOOL_UINPUT_HPP
#define HIDTOOL_UINPUT_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <mutex>

#include <linux/input.h>

namespace hidtool
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
    mutable std::mutex uinputFdMtx_;
};

} // namespace hidism

#endif // !HIDTOOL_UINPUT_HPP
