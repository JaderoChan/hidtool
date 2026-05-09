#ifndef HIDTOOL_HID_SIMULATOR_HPP
#define HIDTOOL_HID_SIMULATOR_HPP

#include "defines.hpp"
#include "hidevent.hpp"

#ifdef HIDTOOL_HAS_KEYBOARD
    #include <hidtool/keyboard/keyboard_simulator.hpp>
#endif // HIDTOOL_HAS_KEYBOARD
#ifdef HIDTOOL_HAS_MOUSE
    #include <hidtool/mouse/mouse_simulator.hpp>
#endif // HIDTOOL_HAS_MOUSE

/**
 * @defgroup Simulators HID 输入模拟器
 * @attention 在 **MacOS** 平台上，由于其 API 设计原因，实际上所有模拟函数都无法知晓其成功与否，
 * 所以，在 **MacOS** 平台上，即使你执行模拟函数并得到 `true` 值，其仍然可能不产生任何效果。通常来讲，
 * 你需要给你的应用程序获得 **辅助功能** 相关权限才能正常执行。
 */

namespace hidt
{

/**
 * @ingroup Simulators
 * @brief 集成所有 Simulator 模块。
 * @note 相较于直接使用特定 Simulator 模块，使用此接口，性能会有所下降。
 */
class HIDTOOL_API HidSimulator
{
public:
    static HidSimulator& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const HidEvent& event);
    size_t sendEvent(const HidEvent* events, size_t count);

private:
    HidSimulator();
    ~HidSimulator() = default;
    HidSimulator(const HidSimulator&) = delete;
    HidSimulator& operator=(const HidSimulator&) = delete;

#ifdef HIDTOOL_HAS_KEYBOARD
    KeyboardSimulator& kbdSimulator_;
#endif // HIDTOOL_HAS_KEYBOARD
#ifdef HIDTOOL_HAS_MOUSE
    MouseSimulator& msSimulator_;
#endif // HIDTOOL_HAS_MOUSE
};

} // namespace hidt

#endif // !HIDTOOL_HID_SIMULATOR_HPP
