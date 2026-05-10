#ifndef HIDTOOL_KEYBOARD_SIMULATOR_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "keyboard_event.hpp"

namespace hidt
{

class KeyboardSimulatorPrivate;

/**
 * @brief Keyboard Input Simulator
 * @ingroup hid_simulators
 * @note Unless otherwise specified, all member functions of this class are reentrant.
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
     * @defgroup keyboard_simulator_convenient Keyboard simulation utility functions
     * @brief Equivalent to send event or send a group events by \ref sendEvent().
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
