#ifndef HIDTOOL_HID_HOOKER_HPP
#define HIDTOOL_HID_HOOKER_HPP

#include <atomic>

#include "defines.hpp"
#include "hidevent.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_hooker.hpp>
#endif // HIDTOOL_HAS_KEYBOARD
#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_hooker.hpp>
#endif // HIDTOOL_HAS_MOUSE

/**
 * @defgroup hid_hookers Global HID Event Listener
 */

namespace hidt
{

using HidEventHandler = bool (*)(const HidEvent&);

/**
 * @brief Integrates all Hooker modules
 * @ingroup hid_hookers
 * @note Compared to directly using a specific Hooker module,
 * using this interface will result in some performance degradation.
 * @attention Do not call member functions in worker threads (i.e., in event handling callbacks).
 */
class HIDTOOL_API HidHooker
{
public:
    static HidHooker& getInstance();

    /** @brief Check if the current environment supports blocking event propagation. */
    static bool isSupportBlockEventPropagation() noexcept;

    bool run();
    void stop();
    bool isRunning() const;

    bool setEventHandler(const HidEventHandler& eventHandler);

private:
    HidHooker();
    ~HidHooker() = default;
    HidHooker(const HidHooker&) = delete;
    HidHooker& operator=(const HidHooker&) = delete;

    static std::atomic<HidEventHandler> hidEventHandler_;
#ifdef HIDTOOL_HAS_KEYBOARD
    static KeyboardEventHandler kbdEventHandler_;
#endif
#ifdef HIDTOOL_HAS_MOUSE
    static MouseEventHandler msEventHandler_;
#endif

#ifdef HIDTOOL_HAS_KEYBOARD
    KeyboardHooker& kbdHooker_;
#endif // HIDTOOL_HAS_KEYBOARD
#ifdef HIDTOOL_HAS_MOUSE
    MouseHooker& msHooker_;
#endif // HIDTOOL_HAS_MOUSE
};

} // namespace hidt

#endif // !HIDTOOL_HID_HOOKER_HPP
