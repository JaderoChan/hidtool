#ifndef HIDTOOL_KEYBOARD_HOOKER_HPP
#define HIDTOOL_KEYBOARD_HOOKER_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidt
{

/**
 * @brief Keyboard event handling call-back function signature.
 * @note When the return value is `true`, the received event is propagated normally;
 * when the return value is `false`, the event is prevented from propagating to other programs.
 */
using KeyboardEventHandler = bool (*)(const KeyboardEvent&);

class KeyboardHookerPrivate;

/**
 * @brief Global Keyboard Event Listener
 * @ingroup hid_hookers
 * @note Unless otherwise specified, all member functions of this class are thread-safe.
 * @attention Do not call member functions in worker threads (i.e., in event handling callbacks).
 */
class HIDTOOL_API KeyboardHooker
{
public:
    static KeyboardHooker& getInstance();

    bool run();
    void stop();
    bool isRunning() const;

    /** @brief Pass in nullptr to unset the event handler. */
    bool setEventHandler(const KeyboardEventHandler& eventHandler);

private:
    explicit KeyboardHooker(KeyboardHookerPrivate&);
    ~KeyboardHooker() = default;
    KeyboardHooker(const KeyboardHooker&) = delete;
    KeyboardHooker& operator=(const KeyboardHooker&) = delete;

    KeyboardHookerPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_HOOKER_HPP
