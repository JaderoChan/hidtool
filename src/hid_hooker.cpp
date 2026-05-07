#include <hidtool/hid_hooker.hpp>

namespace hidt
{

std::atomic<HidEventHandler> HidHooker::hidEventHandler_{nullptr};

#ifdef HIDTOOL_HAS_KEYBOARD
KeyboardEventHandler HidHooker::kbdEventHandler_ = [](const KeyboardEvent& event) -> bool
{
    HidEvent hidEvent{HIDTYPE_KEYBOARD};
    hidEvent.keyboardEvent = event;

    auto hidEventHandler = hidEventHandler_.load();
    if (hidEventHandler)
        return hidEventHandler(hidEvent);
    return true;
};
#endif

#ifdef HIDTOOL_HAS_MOUSE
MouseEventHandler HidHooker::msEventHandler_ = [](const MouseEvent& event) -> bool
{
    HidEvent hidEvent{HIDTYPE_MOUSE};
    hidEvent.mouseEvent = event;

    auto hidEventHandler = hidEventHandler_.load();
    if (hidEventHandler)
        return hidEventHandler(hidEvent);
    return true;
};
#endif

HidHooker::HidHooker()
    :
#ifdef HIDTOOL_HAS_KEYBOARD
    kbdHooker_(KeyboardHooker::getInstance())
#endif
#ifdef HIDTOOL_HAS_MOUSE
    #ifdef HIDTOOL_HAS_KEYBOARD
    ,
    #endif
    msHooker_(MouseHooker::getInstance())
#endif
{
#ifdef HIDTOOL_HAS_KEYBOARD
    kbdHooker_.setEventHandler(kbdEventHandler_);
#endif
#ifdef HIDTOOL_HAS_MOUSE
    msHooker_.setEventHandler(msEventHandler_);
#endif
}

HidHooker& HidHooker::getInstance()
{
    static HidHooker instance;
    return instance;
}

bool HidHooker::isSupportBlockEventPropagation() noexcept
{
    bool result = true;

    #ifdef HIDTOOL_HAS_KEYBOARD
        result &= KeyboardHooker::isSupportBlockEventPropagation();
    #endif
    #ifdef HIDTOOL_HAS_MOUSE
        result &= MouseHooker::isSupportBlockEventPropagation();
    #endif

    return result;
}

bool HidHooker::run()
{
#ifdef HIDTOOL_HAS_KEYBOARD
    if (!kbdHooker_.run())
        return false;
#endif

#ifdef HIDTOOL_HAS_MOUSE
    if (!msHooker_.run())
    {
    #ifdef HIDTOOL_HAS_KEYBOARD
        kbdHooker_.stop();
    #endif
        return false;
    }
#endif

    return true;
}

void HidHooker::stop()
{
#ifdef HIDTOOL_HAS_KEYBOARD
    kbdHooker_.stop();
#endif
#ifdef HIDTOOL_HAS_MOUSE
    msHooker_.stop();
#endif
}

bool HidHooker::isRunning() const
{
    bool result = true;

    #ifdef HIDTOOL_HAS_KEYBOARD
        result &= kbdHooker_.isRunning();
    #endif
    #ifdef HIDTOOL_HAS_MOUSE
        result &= msHooker_.isRunning();
    #endif

    return result;
}

bool HidHooker::setEventHandler(const HidEventHandler& eventHandler)
{
    hidEventHandler_.store(eventHandler);
    return true;
}

} // namespace hidt
