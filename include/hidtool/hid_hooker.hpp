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
 * @defgroup Hookers HID 事件监听器
 */

namespace hidtool
{

using HidEventHandler = bool (*)(const HidEvent&);

/**
 * @ingroup Hookers
 * @brief 集成所有 Hooker 模块。
 * @note 相较于直接使用特定 Hooker 模块，使用此接口，性能会有所下降。
 */
class HIDTOOL_API HidHooker
{
public:
    static HidHooker& getInstance();

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

} // namespace hidtool

#endif // !HIDTOOL_HID_HOOKER_HPP
