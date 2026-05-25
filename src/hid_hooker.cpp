#include <hidtool/hid_hooker.hpp>

namespace hidt
{

std::atomic<HidEventHandler> HidHooker::hidEventHandler_{nullptr};
std::atomic<intptr_t> HidHooker::hidUserData_{0};

#ifdef HIDTOOL_HAS_KEYBOARD
KeyboardEventHandler HidHooker::kbdEventHandler_ = [](const KeyboardEvent& event, void*) -> bool
{
    HidEvent hidEvent(HidEvent::ET_KEYBOARD);
    hidEvent.keyboardEvent = event;

    auto hidEventHandler = hidEventHandler_.load();
    if (hidEventHandler)
        return hidEventHandler(hidEvent, reinterpret_cast<void*>(hidUserData_.load()));
    return true;
};
#endif

#ifdef HIDTOOL_HAS_MOUSE
MouseEventHandler HidHooker::msEventHandler_ = [](const MouseEvent& event, void*) -> bool
{
    HidEvent hidEvent(HidEvent::ET_MOUSE);
    hidEvent.mouseEvent = event;

    auto hidEventHandler = hidEventHandler_.load();
    if (hidEventHandler)
        return hidEventHandler(hidEvent, reinterpret_cast<void*>(hidUserData_.load()));
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
#if defined(HIDTOOL_WIN)
    return true;
#elif defined(HIDTOOL_MACOS)
    return true;
#elif defined(HIDTOOL_LINUX)
    return false;
#else
    #error "Unspported platform"
#endif // HIDTOOL_WIN
}

#ifdef HIDTOOL_HAS_KEYBOARD
bool HidHooker::isKeyPressed(int32_t nativeKey)
{
    return KeyboardHooker::isKeyPressed(nativeKey);
}
#endif

#ifdef HIDTOOL_HAS_MOUSE
bool HidHooker::isButtonPressed(MouseButton button)
{
    return MouseHooker::isButtonPressed(button);
}

AbsolutePos HidHooker::getCursorPos()
{
    return MouseHooker::getCursorPos();
}
#endif

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

bool HidHooker::setEventHandler(HidEventHandler eventHandler, void* userData)
{
    hidEventHandler_.store(eventHandler);
    hidUserData_.store(reinterpret_cast<intptr_t>(userData));
    return true;
}

bool HidHooker::setUserData(void* userData)
{
    hidUserData_.store(reinterpret_cast<intptr_t>(userData));
    return true;
}

} // namespace hidt
