#ifndef HIDTOOL_KEYBOARD_SIMULATOR_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidt
{

class KeyboardSimulatorPrivate;

/**
 * @brief 键盘输入模拟器
 * @ingroup Simulators
 * @note 若未特别说明，此类的所有成员函数都是可重入的。
 */
class HIDTOOL_API KeyboardSimulator
{
public:
    static KeyboardSimulator& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const KeyboardEvent& event);
    size_t sendEvent(const KeyboardEvent* events, size_t count);

    /**
     * @defgroup keyboard_simulator_convenient 键盘模拟便利函数
     * @brief 等效于 \ref sendEvent() 相应的事件或事件组。
     */

    /**
     * @ingroup keyboard_simulator_convenient
     * @{
     */

    bool pressKey(uint32_t nativeKey);
    bool pressKey(KeyboardKey key);

    bool releaseKey(uint32_t nativeKey);
    bool releaseKey(KeyboardKey key);

    bool clickKey(uint32_t nativeKey, size_t interval = 0);
    bool clickKey(KeyboardKey key, size_t interval = 0);

    /** @} */

private:
    explicit KeyboardSimulator(KeyboardSimulatorPrivate&);
    ~KeyboardSimulator() = default;
    KeyboardSimulator(const KeyboardSimulator&) = delete;
    KeyboardSimulator& operator=(const KeyboardSimulator&) = delete;

    KeyboardSimulatorPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_SIMULATOR_HPP
