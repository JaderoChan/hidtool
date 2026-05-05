#ifndef HIDTOOL_MOUSE_SIMULATOR_HPP
#define HIDTOOL_MOUSE_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidt
{

class MouseSimulatorPrivate;

/**
 * @ingroup Simulators
 * @note 若未特别说明，此类的所有成员函数都是线程安全的。
 * 此外，除 \ref `MouseSimulator::initialize()` 和 \ref `MouseSimulator::destroy()` 外，其他成员函数都是可重入的。
 */
class HIDTOOL_API MouseSimulator
{
public:
    static MouseSimulator& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    /**
     * @brief 绝对移动。
     * @note 当发送超过坐标范围的绝对移动事件时，会将其钳制在合法范围内。
     * @sa \ref `MouseEvent::absPos`
     */
    bool moveTo(const AbsolutePos& absPos);

    /**
     * @brief 相对移动。
     * @sa \ref `MouseEvent::relPos`
     */
    bool moveBy(const RelativePos& relPos);

    /**
     * @param wheelDelta 单位量为 `120`。值为正时，滚轮朝远离用户的方向滚动；值为负时，滚轮朝靠近用户的方向滚动。
     * @note 在各平台下，其行为均为低分辨率纵向滚轮。
     * @attention 若指定滚动量绝对值小于 `120`，可能不会有效果。
     */
    bool wheel(int32_t wheelDelta);

    /**
     * @name 鼠标按键函数
     * @brief 在鼠标当前位置执行按键函数。
     * @details 在 **MacOS** 平台下需要额外进行获取鼠标当前位置的操作。
     * @{
     */
    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    /** @brief 等同于 \ref `pressButton()` 和 \ref `releaseButton()` 的组合。 */
    bool clickButton(MouseButton button);
    /** @} */

    /**
     * @name 带坐标的鼠标函数
     * @brief 在指定位置执行函数。
     * @details
     * 在 **Windows** 和 **Linux** 平台下，等同于 \ref `moveTo()` + 对应函数的组合。
     * 在 **MacOS** 下，其原生支持。
     * @{
     */
    bool wheel(const AbsolutePos& absPos, int32_t wheelDelta);
    bool pressButton(const AbsolutePos& absPos, MouseButton button);
    bool releaseButton(const AbsolutePos& absPos, MouseButton button);
    bool clickButton(const AbsolutePos& absPos, MouseButton button);
    /** @} */

    /**
     * @anchor mouse_drag_fns
     * @name 鼠标拖拽函数。
     * @brief 从起始位置拖拽鼠标至终点位置。
     * @attention 在 **MacOS** 平台下，无法通过 \ref `pressButton()` + \ref `moveTo()` + \ref `releaseButton()`
     * 等函数的组合来实现拖拽。只能调用此函数。（后续版本可能通过增加事件类型进行修改）
     * @details
     * 在 **Windows** 和 **Linux** 平台下，等同于 \ref `pressButton()` + \ref `moveTo()` + \ref `releaseButton()` 的组合。
     * 在 **MacOS** 平台下，其原生支持。
     * @sa \ref `MouseEvent::ET_ABS_MOVE`
     * @{
     */

    /** @brief 从鼠标当前位置拖拽鼠标至指定位置。 */
    bool drag(const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT);
    bool drag(const AbsolutePos& startPos, const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT);

    /** @} */

private:
    explicit MouseSimulator(MouseSimulatorPrivate&);
    ~MouseSimulator() = default;
    MouseSimulator(const MouseSimulator&) = delete;
    MouseSimulator& operator=(const MouseSimulator&) = delete;

    MouseSimulatorPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_SIMULATOR_HPP
