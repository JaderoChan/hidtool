#ifndef HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP

#include <atomic>   // atomic

#include <hidtool/keyboard/keyboard_simulator.hpp>

namespace hidtool
{

class KeyboardSimulatorPrivate
{
public:
    static KeyboardSimulatorPrivate& getInstance();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const KeyboardEvent& event);
    size_t sendEvent(const KeyboardEvent* events, size_t count);

    bool pressKey(uint32_t nativeKey);
    bool releaseKey(uint32_t nativeKey);
    bool clickKey(uint32_t nativeKey);

private:
    KeyboardSimulatorPrivate() = default;
    ~KeyboardSimulatorPrivate();
    KeyboardSimulatorPrivate(const KeyboardSimulatorPrivate&) = delete;
    KeyboardSimulatorPrivate& operator=(const KeyboardSimulatorPrivate&) = delete;

    std::atomic<bool> isInitialized_{false};
};

} // namespace hidtool

#endif // !HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP
