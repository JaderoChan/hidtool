#ifndef HIDTOOL_KEYBOARD_SIMULATOR_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidtool
{

class KeyboardSimulatorPrivate;

/** @note 若未特别说明，此类的所有成员函数都是线程安全的。 */
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
     * @brief 等同于 `pressKey()` 与 `releaseKey()` 的组合。
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

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_SIMULATOR_HPP
