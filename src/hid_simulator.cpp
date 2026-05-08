#include <hidtool/hid_simulator.hpp>

#include <chrono>   // chrono
#include <thread>   // this_thread

namespace hidt
{

HidSimulator::HidSimulator()
    :
#ifdef HIDTOOL_HAS_KEYBOARD
    kbdSimulator_(KeyboardSimulator::getInstance())
#endif
#ifdef HIDTOOL_HAS_MOUSE
    #ifdef HIDTOOL_HAS_KEYBOARD
    ,
    #endif
    msSimulator_(MouseSimulator::getInstance())
#endif
{}

HidSimulator& HidSimulator::getInstance()
{
    static HidSimulator instance;
    return instance;
}

bool HidSimulator::initialize()
{
#ifdef HIDTOOL_HAS_KEYBOARD
    if (!kbdSimulator_.initialize())
        return false;
#endif

#ifdef HIDTOOL_HAS_MOUSE
    if (!msSimulator_.initialize())
    {
    #ifdef HIDTOOL_HAS_KEYBOARD
        kbdSimulator_.destroy();
    #endif
        return false;
    }
#endif

    return true;
}

void HidSimulator::destroy()
{
#ifdef HIDTOOL_HAS_KEYBOARD
    kbdSimulator_.destroy();
#endif
#ifdef HIDTOOL_HAS_MOUSE
    msSimulator_.destroy();
#endif
}

bool HidSimulator::isInitialized() const
{
    bool result = true;

    #ifdef HIDTOOL_HAS_KEYBOARD
        result &= kbdSimulator_.isInitialized();
    #endif
    #ifdef HIDTOOL_HAS_MOUSE
        result &= msSimulator_.isInitialized();
    #endif

    return result;
}

bool HidSimulator::sendEvent(const HidEvent& event)
{
    switch (event.hidType)
    {
        case HIDTYPE_KEYBOARD:
        #ifdef HIDTOOL_HAS_KEYBOARD
            return kbdSimulator_.sendEvent(event.keyboardEvent);
        #else
            return false;
        #endif
        case HIDTYPE_MOUSE:
        #ifdef HIDTOOL_HAS_MOUSE
            return msSimulator_.sendEvent(event.mouseEvent);
        #else
            return false;
        #endif
        case HIDTYPE_SLEEP:
            std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
            return true;
        default:
            return false;
    }
}

size_t HidSimulator::sendEvent(const HidEvent* events, size_t count)
{
    size_t sent = 0;

    for (size_t i = 0; i < count; ++i)
    {
        const auto& event = events[i];

        switch (event.hidType)
        {
            case HIDTYPE_KEYBOARD:
            #ifdef HIDTOOL_HAS_KEYBOARD
                sent += kbdSimulator_.sendEvent(event.keyboardEvent);
            #endif
                break;
            case HIDTYPE_MOUSE:
            #ifdef HIDTOOL_HAS_MOUSE
                sent += msSimulator_.sendEvent(event.mouseEvent);
            #endif
                break;
            case HIDTYPE_SLEEP:
                std::this_thread::sleep_for(std::chrono::milliseconds(event.sleepMs));
                sent++;
                break;
            default:
                break;
        }
    }

    return sent;
}

} // namespace hidt
