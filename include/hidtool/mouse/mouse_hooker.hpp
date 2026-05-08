#ifndef HIDTOOL_MOUSE_HOOKER_HPP
#define HIDTOOL_MOUSE_HOOKER_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidt
{

/** @note 返回值为 `true` 时正常传播接收到的事件，返回值为 `false` 时将阻止事件向其他程序传播。 */
using MouseEventHandler = bool (*)(const MouseEvent&);

class MouseHookerPrivate;

/**
 * @ingroup Hookers
 * @note 若未特别说明，此类的所有成员函数都是线程安全的。
 * @attention 不要在工作线程中（即事件处理回调中）调用成员函数。
 */
class HIDTOOL_API MouseHooker
{
public:
    static MouseHooker& getInstance();

    bool run();
    void stop();
    bool isRunning() const;

    bool setEventHandler(const MouseEventHandler& eventHandler);

private:
    explicit MouseHooker(MouseHookerPrivate&);
    ~MouseHooker() = default;
    MouseHooker(const MouseHooker&) = delete;
    MouseHooker& operator=(const MouseHooker&) = delete;

    MouseHookerPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_HOOKER_HPP
