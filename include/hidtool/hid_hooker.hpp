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

namespace hidt
{

using HidEventHandler = bool (*)(const HidEvent&);

/**
 * @brief 集成所有 Hooker 模块
 * @ingroup Hookers
 * @note 相较于直接使用特定 Hooker 模块，使用此接口，性能会有所下降。
 * @attention 不要在工作线程中（即事件处理回调中）调用成员函数。
 */
class HIDTOOL_API HidHooker
{
public:
    static HidHooker& getInstance();

    /** @brief 检查当前环境是否支持阻断事件的传播。 */
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
