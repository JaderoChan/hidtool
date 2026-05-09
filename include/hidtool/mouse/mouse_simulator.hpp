#ifndef HIDTOOL_MOUSE_SIMULATOR_HPP
#define HIDTOOL_MOUSE_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidt
{

class MouseSimulatorPrivate;

/**
 * @brief 鼠标输入模拟
 * @ingroup Simulators
 * @note 若未特别说明，此类的所有成员函数都是线程安全的。
 * 此外，除 \ref MouseSimulator::initialize() 和 \ref MouseSimulator::destroy() 外，其他成员函数都是可重入的。
 */
class HIDTOOL_API MouseSimulator
{
public:
    static MouseSimulator& getInstance();

    /**
     * @brief 获得当前环境下发送的绝对移动事件的坐标范围。
     * @note 当发送的绝对移动事件的坐标超出此范围时将会被钳制。
     * @note 此 API 会受环境 DPI 策略的影响。
     * 在 **Windows** 平台下可通过启用 `HIDTOOL_FORCE_IN_PIXEL` 编译选项强制以物理像素为单位。
     * @details 在 **Windows** 和 **MacOS** 平台下，其等同于虚拟屏幕空间的范围。
     * 在 **Linux** 平台下，其始终等同于 {0, 65535, 0, 65535}。
     */
    static AbsolutePosRange getAbsoluteMoveRange();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    /**
     * @defgroup MouseSimulatorConvenient 鼠标模拟便利函数
     * @brief 等效于 \ref sendEvent() 相应的事件或事件组。
     */

    /**
     * @ingroup MouseSimulatorConvenient
     * @{
     */

    /**
     * @brief 鼠标绝对移动
     * @note 当发送超过坐标范围的绝对移动事件时，会将其钳制在合法范围内。
     * @sa \ref MouseEvent::absPos
     */
    bool moveTo(const AbsolutePos& absPos);

    /**
     * @brief 鼠标相对移动
     * @sa \ref MouseEvent::relPos
     */
    bool moveBy(const RelativePos& relPos);

    /**
     * @brief 鼠标滚轮滚动
     * @param wheelDelta 单位量为 `120`。值为正时，滚轮朝远离用户的方向滚动；值为负时，滚轮朝靠近用户的方向滚动。
     * @sa \ref MouseEvent::wheelDelta
     */
    bool wheel(int32_t wheelDelta);

    /**
     * @name 鼠标按键函数
     * @brief 在当前鼠标指针位置执行按键函数。
     * @{
     */

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    bool clickButton(MouseButton button, size_t interval = 0);

    /**
     * @brief 鼠标双击
     * @param interval1 点击事件中按键按下与释放的时间间隔。
     * @param interval2 两次点击事件的时间间隔。
     */
    bool doubleClickButton(MouseButton button, size_t interval1 = 0, size_t interval2 = 10);

    /** @} */

    /**
     * @name 带坐标的鼠标函数
     * @brief 在指定位置执行函数。
     * @note 等效于绝对移动事件与其他事件的组合。
     * @{
     */

    bool wheel(const AbsolutePos& absPos, int32_t wheelDelta, size_t interval = 10);
    bool pressButton(const AbsolutePos& absPos, MouseButton button, size_t interval = 10);
    bool releaseButton(const AbsolutePos& absPos, MouseButton button, size_t interval = 10);

    /**
     * @param interval1 移动事件与点击事件的时间间隔。
     * @param interval2 点击事件中按键按下与释放的时间间隔。
     */
    bool clickButton(const AbsolutePos& absPos, MouseButton button,
        size_t interval1 = 10, size_t interval2 = 0);

    /**
     * @param interval1 移动事件与点击事件的时间间隔。
     * @param interval2 点击事件中按键按下与释放的时间间隔。
     * @param interval3 两次点击事件的时间间隔。
     */
    bool doubleClickButton(const AbsolutePos& absPos, MouseButton button,
        size_t interval1 = 10, size_t interval2 = 0, size_t interval3 = 10);

    /** @} */

    /**
     * @brief 鼠标拖拽移动
     * @note 在 **Windows** 和 **Linux** 平台下，等同于绝对移动事件，参数 `button` 将被丢弃。
     * 在 **MacOS** 平台下，其原生支持。
     * @sa \ref dragComboAnchor
     */
    bool dragTo(const AbsolutePos& absPos, MouseButton button = MSBTN_LEFT);

    /**
     * @anchor dragComboAnchor
     * @name 鼠标拖拽组合
     * @brief 从指定起始位置拖拽鼠标至指定终点位置。
     * @details 等同于鼠标按键按压事件、拖拽事件与鼠标按键释放事件的组合。
     * @{
     */

    /** @param 拖拽事件中按键按下、拖拽至与按键释放的事件间隔。 */
    bool dragCombo(const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT, size_t interval = 0);

    /**
     * @param interval1 移动到 `startPos` 事件与拖拽事件的时间间隔。
     * @param interval2 拖拽事件中按键按下、拖拽至与按键释放的事件间隔。
     */
    bool dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT, size_t interval1 = 10, size_t interval2 = 0);

    /** @} */

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
