#include "keyboard_simulator_private.hpp"

#include <platforms/linux/input_event_factory.hpp>

namespace hidtool
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate()
{
    if (!isInitialized_.load())
        return;

    isInitialized_.store(false);
}

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

    kbdUInput_.cleanup();
}

bool KeyboardSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent& event)
{
    switch (event.eventType())
    {
        case KeyboardEvent::ET_PRESS:
            return pressKey(event.nativeKey());
        case KeyboardEvent::ET_RELEASE:
            return releaseKey(event.nativeKey());
        case KeyboardEvent::ET_NONE:
        default:
            return false;
    }
}

size_t KeyboardSimulatorPrivate::sendEvent(const std::vector<KeyboardEvent>& events)
{
    if (!isInitialized_.load())
        return 0;

    size_t sent = 0;
    for (const auto& event : events)
    {
        switch (event.eventType())
        {
            case KeyboardEvent::ET_PRESS:
                sent += pressKey(event.nativeKey());
                break;
            case KeyboardEvent::ET_RELEASE:
                sent += releaseKey(event.nativeKey());
                break;
            case KeyboardEvent::ET_NONE:
            default:
                continue;
        }
    }

    return sent;
}

bool KeyboardSimulatorPrivate::pressKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event iePair[2] = {0};

    iePair[0].type = EV_KEY;
    iePair[0].value = 1;    // 1 is press.
    iePair[0].code = static_cast<__s32>(nativeKey);

    iePair[1] = createSyncEvent();

    return kbdUInput_.sendEvent(iePair, 2);
}

bool KeyboardSimulatorPrivate::releaseKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event iePair[2] = {0};

    iePair[0].type = EV_KEY;
    iePair[0].value = 0;    // 0 is release.
    iePair[0].code = static_cast<__s32>(nativeKey);

    iePair[1] = createSyncEvent();

    return kbdUInput_.sendEvent(iePair, 2);
}

bool KeyboardSimulatorPrivate::clickKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    struct input_event ies[4] = {0};

    ies[0].type = EV_KEY;
    ies[0].value = 1;   // 1 is press.
    ies[0].code = static_cast<__s32>(nativeKey);

    ies[1] = createSyncEvent();

    ies[2].type = EV_KEY;
    ies[2].value = 0;   // 0 is release.
    ies[2].code = static_cast<__s32>(nativeKey);

    ies[3] = createSyncEvent();

    return kbdUInput_.sendEvent(ies, 4);
}

} // namespace hidtool
