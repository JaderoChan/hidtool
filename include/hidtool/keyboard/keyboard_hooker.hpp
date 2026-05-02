#ifndef HIDTOOL_KEYBOARD_HOOKER_HPP
#define HIDTOOL_KEYBOARD_HOOKER_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidtool
{

/** @note 返回值为 `true` 时正常传播接收到的事件，返回值为 `false` 时将阻止事件向其他程序传播。 */
using KeyboardEventHandler = bool (*)(const KeyboardEvent&);

class KeyboardHookerPrivate;

/**
 * @ingroup Hooker
 * @note 若未特别说明，此类的所有成员函数都是线程安全的。
 * @attention 不要在工作线程中（即事件处理回调中）调用成员函数。
 */
class HIDTOOL_API KeyboardHooker
{
public:
    static KeyboardHooker& getInstance();

    /** @brief 检查当前环境是否支持阻断事件的传播。 */
    static bool isSupportBlockEventPropagation() noexcept;

    bool run();
    void stop();
    bool isRunning() const;

    bool setEventHandler(const KeyboardEventHandler& eventHandler);

private:
    explicit KeyboardHooker(KeyboardHookerPrivate&);
    ~KeyboardHooker() = default;
    KeyboardHooker(const KeyboardHooker&) = delete;
    KeyboardHooker& operator=(const KeyboardHooker&) = delete;

    KeyboardHookerPrivate& pri_;
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_HOOKER_HPP
