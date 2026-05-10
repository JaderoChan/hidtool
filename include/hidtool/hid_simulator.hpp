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
 * @defgroup hid_simulators HID Input Simulator
 * @attention On the **MacOS** platform, due to the design of its API.
 * In fact, it is impossible to know the success or failure of any simulated function execution.
 * So, on the **MacOS** platform, even if you execute the simulation function and get a `true` value,
 * it still might not have any effect. Generally speaking,
 * you need to grant your application **Accessibility** permissions to function properly.
 */

namespace hidt
{

/**
 * @brief Integrates all Simulator modules
 * @ingroup hid_simulators
 * @note Compared to directly using a specific Hooker module,
 * using this interface will result in some performance degradation.
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
