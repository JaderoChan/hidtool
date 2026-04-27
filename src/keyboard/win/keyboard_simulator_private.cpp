#include "keyboard_simulator_private.hpp"

#include <vector>       // vector

#include <windows.h>    // INPUT, SendInput()

#include "keyboard_event_converter.hpp"

namespace hidtool
{

KeyboardSimulatorPrivate::~KeyboardSimulatorPrivate()
{
    isInitialized_.store(false);
}

KeyboardSimulatorPrivate& KeyboardSimulatorPrivate::getInstance()
{
    static KeyboardSimulatorPrivate instance;
    return instance;
}

bool KeyboardSimulatorPrivate::initialize()
{
    // 无实际作用，仅与其他子模块保持一致性。`destroy()` 同理。
    bool expected = false;
    return isInitialized_.compare_exchange_strong(expected, true);
}

void KeyboardSimulatorPrivate::destroy()
{
    bool expected = true;
    isInitialized_.compare_exchange_strong(expected, false);
}

bool KeyboardSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent& event)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    if (keyboardEventToInput(event, input))
        return SendInput(1, &input, sizeof(INPUT)) == 1;
    return false;
}

size_t KeyboardSimulatorPrivate::sendEvent(const KeyboardEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    std::vector<INPUT> inputs;
    inputs.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        INPUT input = {0};
        if (keyboardEventToInput(events[i], input))
            inputs.emplace_back(std::move(input));
    }

    return SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
}

bool KeyboardSimulatorPrivate::pressKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(nativeKey);
    input.ki.dwFlags = 0;   // 0 is key down.

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool KeyboardSimulatorPrivate::releaseKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(nativeKey);
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool KeyboardSimulatorPrivate::clickKey(uint32_t nativeKey)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = static_cast<WORD>(nativeKey);
    inputs[0].ki.dwFlags = 0;   // 0 is key down.

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = static_cast<WORD>(nativeKey);
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

} // namespace hidtool
