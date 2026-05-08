#ifndef HIDTOOL_KEYBOARD_SIMULATOR_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidt
{

class KeyboardSimulatorPrivate;

/**
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

    bool pressKey(uint32_t nativeKey);
    bool pressKey(KeyboardKey key);

    bool releaseKey(uint32_t nativeKey);
    bool releaseKey(KeyboardKey key);

    /**
     * @name clickKey 函数
     * @brief 等同于 \ref `pressKey()` 与 \ref `releaseKey()` 的组合。
     *
     * @{
     */
    bool clickKey(uint32_t nativeKey);
    bool clickKey(KeyboardKey key);
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
