#ifndef HIDTOOL_MOUSE_SIM_HPP
#define HIDTOOL_MOUSE_SIM_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidtool
{

class MouseSimulatorPrivate;

/**
 * @note 若未特别说明，此类的所有成员函数都是线程安全的。
 * 此外，除 `initialize()` 和 `destroy()` 外，其他成员函数都是可重入的。
 */
class HIDTOOL_API MouseSimulator
{
public:
    struct AbsMoveRange
    {
        int32_t minX = 0;
        int32_t maxX = 0;
        int32_t minY = 0;
        int32_t maxY = 0;
    };

    static MouseSimulator& getInstance();

    static AbsMoveRange getAbsMoveRange();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    /// @brief 相对移动。
    bool moveBy(int32_t dx, int32_t dy);

    /// @brief 绝对移动。
    /// @sa MouseEvent::pos
    bool moveTo(int32_t x, int32_t y);

    /// @sa MouseEvent::wheelDelta
    bool wheel(int32_t wheelDelta);

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    /// @note 等同于 `pressButton()` 和 `releaseButton()` 的组合。
    bool clickButton(MouseButton button);

private:
    explicit MouseSimulator(MouseSimulatorPrivate&);
    ~MouseSimulator() = default;
    MouseSimulator(const MouseSimulator&) = delete;
    MouseSimulator& operator=(const MouseSimulator&) = delete;

    MouseSimulatorPrivate& pri_;
};

} // namespace hidtool

#endif // !HIDTOOL_MOUSE_SIM_HPP
