#include "keyboard_simulator_private.hpp"

#include <platforms/linux/sync_input_event_factory.hpp>
#include "input_event_factory.hpp"

namespace hidtool
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate() { destroy(); }

KeyboardSimulatorPrivate& KeyboardSimulatorPrivate::getInstance()
{
    static KeyboardSimulatorPrivate instance;
    return instance;
}

bool KeyboardSimulatorPrivate::initialize()
{
    bool expected = false;
    if (!isInitialized_.compare_exchange_strong(expected, true))
        return false;

    // 配置键盘 UInput。
    if (!kbdUInput_.setup("JaderoChan HIDTOOL Keyboard UInput", 0x0310, 0x0001, 0x0001))
    {
        isInitialized_.store(false);
        return false;
    }

    return true;
}

void KeyboardSimulatorPrivate::destroy()
{
    bool expected = true;
    if (!isInitialized_.compare_exchange_strong(expected, false))
        return;

    // 清理键盘 UInput。
    kbdUInput_.cleanup();
}

bool KeyboardSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent& event)
{
    if (!isInitialized_.load)
        return false;

    struct input_event iePair[2] = {0};
    iePair[0].type = EV_KEY;
    iePair[0].code = static_cast<__s32>(nativeKey);
    setSyncReportEvent(iePair[1]);

    switch (event.eventType)
    {
        case KeyboardEvent::ET_PRESS:
            iePair[0].value = 1;    // 1 is press.
            break;
        case KeyboardEvent::ET_RELEASE:
            iePair[0].value = 0;    // 0 is release.
            break;
        default:
            return false;
    }

    return kbdUInput_.sendEvent(iePair, 2);
}

size_t KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    struct input_event iePair[2] = {0};
    iePair[0].type = EV_KEY;
    iePair[0].code = static_cast<__s32>(nativeKey);
    setSyncReportEvent(iePair[1]);

    size_t sent = 0;
    for (size_t i = 0; i < count; ++i)
    {
        const auto& event = events[i];
        switch (event.eventType)
        {
            case KeyboardEvent::ET_PRESS:
                iePair[0].value = 1;    // 1 is press.
                break;
            case KeyboardEvent::ET_RELEASE:
                iePair[0].value = 0;    // 0 is release.
                break;
            default:
                continue;
        }

        sent += kbdUInput_.sendEvent(iePair, 2);
    }

    return sent;
}

bool KeyboardSimulatorPrivate::pressKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event iePair[2] = {0};
    setPressKeyInputEvent(iePair[0], nativeKey);
    setSyncReportEvent(iePair[1]);

    return kbdUInput_.sendEvent(iePair, 2);
}

bool KeyboardSimulatorPrivate::releaseKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event iePair[2] = {0};
    setReleaseKeyInputEvent(iePair[0], nativeKey);
    setSyncReportEvent(iePair[1]);

    return kbdUInput_.sendEvent(iePair, 2);
}

bool KeyboardSimulatorPrivate::clickKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[4] = {0};
    setPressKeyInputEvent(ies[0], nativeKey);
    setSyncReportEvent(iePair[1]);
    setReleaseKeyInputEvent(ies[2], nativeKey);
    setSyncReportEvent(iePair[3]);

    return kbdUInput_.sendEvent(ies, 4);
}

} // namespace hidtool
