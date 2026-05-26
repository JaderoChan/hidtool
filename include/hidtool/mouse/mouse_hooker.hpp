#ifndef HIDTOOL_MOUSE_HOOKER_HPP
#define HIDTOOL_MOUSE_HOOKER_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidt
{

/**
 * @brief Mouse event handling call-back function signature.
 * @note When the return value is `true`, the received event is propagated normally;
 * when the return value is `false`, the event is prevented from propagating to other programs.
 */
using MouseEventHandler = bool (*)(const MouseEvent&, void* userData);

class MouseHookerPrivate;

/**
 * @brief Global Mouse Event Listener
 * @ingroup hid_hookers
 * @note Unless otherwise specified, all member functions of this class are thread-safe.
 * @attention Do not call member functions in worker threads (i.e., in event handling callbacks).
 */
class HIDTOOL_API MouseHooker
{
public:
    static MouseHooker& getInstance();

    static bool isButtonPressed(MouseButton button);
    /** @note Always returns {0, 0} on Linux. */
    static AbsolutePos getCursorPos();

    bool run();
    void stop();
    bool isRunning() const;

    /** @note Pass in nullptr to unset the event handler. */
    bool setEventHandler(MouseEventHandler eventHandler, void* userData = nullptr);
    bool setUserData(void* userData);

private:
    explicit MouseHooker(MouseHookerPrivate&);
    ~MouseHooker() = default;
    MouseHooker(const MouseHooker&) = delete;
    MouseHooker& operator=(const MouseHooker&) = delete;

    MouseHookerPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_HOOKER_HPP
