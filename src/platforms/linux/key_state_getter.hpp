#ifndef HIDTOOL_KEY_STATE_GETTER_HPP
#define HIDTOOL_KEY_STATE_GETTER_HPP

#include <mutex>
#include <vector>

#include <linux/input-event-codes.h>    // KEY_*

namespace hidt
{

class KeyStateGetter
{
public:
    KeyStateGetter();
    ~KeyStateGetter();

    bool isKeyPressed(int32_t nativeKey);

private:
    // Drain inotify events, returns true if any device change was detected.
    bool drainInotify();
    void refresh();

    std::vector<int> fds_;
    uint8_t keyStateBits_[KEY_MAX / 8 + 1] = {0};
    int inotifyFd_      = -1;
    bool needsRefresh_  = true;
    mutable std::mutex mtx_;
};

} // namespace hidt

#endif // !HIDTOOL_KEY_STATE_GETTER_HPP
