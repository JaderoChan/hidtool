#ifndef HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP
#define HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP

#include <atomic>   // atomic

#include <hidtool/keyboard/keyboard_simulator.hpp>

namespace hidt
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

private:
    KeyboardSimulatorPrivate() = default;
    ~KeyboardSimulatorPrivate();
    KeyboardSimulatorPrivate(const KeyboardSimulatorPrivate&) = delete;
    KeyboardSimulatorPrivate& operator=(const KeyboardSimulatorPrivate&) = delete;

    std::atomic<bool> isInitialized_{false};
};

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_SIMULATOR_PRIVATE_HPP
