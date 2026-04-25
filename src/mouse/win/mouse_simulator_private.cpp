#include "mouse_simulator_private.hpp"

#include <algorithm>
#include <future>
#include <vector>

#include "mouse_event_converter.hpp"

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate()
{
    if (!isInitialized_.load())
        return;

    PostThreadMessageA(displayChangedHandlerThreadId_, WM_QUIT, 0, 0);
    if (displayChangedHandlerThread_.joinable())
        displayChangedHandlerThread_.join();

    displayChangedHandlerThreadId_ = 0;
    isInitialized_.store(false);
}

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

MouseSimulator::AbsMoveRange MouseSimulatorPrivate::getAbsMoveRange()
{
    MouseSimulator::AbsMoveRange result;
    VirtualScreenInfo vsi = VirtualScreenInfo::getLatest();

    result.minX = vsi.x;
    result.minY = vsi.y;
    result.maxX = vsi.x + vsi.width;
    result.maxY = vsi.y + vsi.height;

    return result;
}

bool MouseSimulatorPrivate::initialize()
{
    std::lock_guard<std::mutex> locker(initDestrMtx_);

    if (isInitialized_.load())
        return false;

    virtualScreenInfo_.store(VirtualScreenInfo::getLatest());

    std::promise<void> threadIdIsReady;
    auto fut = threadIdIsReady.get_future();

    displayChangedHandlerThread_ = std::thread([=, &threadIdIsReady]()
    {
        displayChangedHandlerThreadId_ = GetCurrentThreadId();
        threadIdIsReady.set_value();

        handleDisplayChanged();

        displayChangedHandlerThreadId_ = 0;
    });

    isInitialized_.store(true);

    fut.wait();
    return true;
}

void MouseSimulatorPrivate::destroy()
{
    std::lock_guard<std::mutex> locker(initDestrMtx_);

    if (!isInitialized_.load())
        return;

    PostThreadMessageA(displayChangedHandlerThreadId_, WM_QUIT, 0, 0);
    if (displayChangedHandlerThread_.joinable())
        displayChangedHandlerThread_.join();

    isInitialized_.store(false);
}

bool MouseSimulatorPrivate::isInitialized() const
{
    return isInitialized_.load();
}

bool MouseSimulatorPrivate::sendEvent(const MouseEvent& event)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    if (mouseEventToInput(event, input))
        return SendInput(1, &input, sizeof(INPUT)) == 1;
    return false;
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    std::vector<INPUT> inputs;
    inputs.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        INPUT input = {0};
        if (mouseEventToInput(events[i], input))
            inputs.emplace_back(std::move(input));
    }

    return SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
}

bool MouseSimulatorPrivate::moveBy(int32_t dx, int32_t dy)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = static_cast<LONG>(dx);
    input.mi.dy = static_cast<LONG>(dy);

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::moveTo(int32_t x, int32_t y)
{
    if (!isInitialized_.load())
        return false;

    VirtualScreenInfo vsi = virtualScreenInfo_.load();

    // Normalize position.
    LONG nx = static_cast<LONG>(static_cast<double>(x - vsi.x) / (vsi.width - 1) * 65535.0);
    LONG ny = static_cast<LONG>(static_cast<double>(y - vsi.y) / (vsi.height - 1) * 65535.0);

    // Clamp.
    nx = std::max(0L, std::min(65535L, nx));
    ny = std::max(0L, std::min(65535L, ny));

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABS_MOVE;
    input.mi.dx = nx;
    input.mi.dy = ny;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = static_cast<DWORD>(wheelDelta);

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    if (!setPressMouseButtonInput(input, button))
        return false;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    if (!setReleaseMouseButtonInput(input, button))
        return false;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};
    inputs[0].type = INPUT_MOUSE;
    inputs[1].type = INPUT_MOUSE;
    if (!setPressMouseButtonInput(inputs[0], button))
        return false;
    if (!setReleaseMouseButtonInput(inputs[1], button))
        return false;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

void MouseSimulatorPrivate::handleDisplayChanged()
{
    MSG msg = {0};
    // Force the system to create the message queue.
    PeekMessageA(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    while (GetMessageA(&msg, nullptr, 0, 0) != 0)
    {
        switch (msg.message)
        {
            case WM_DISPLAYCHANGE:
                virtualScreenInfo_.store(VirtualScreenInfo::getLatest());
                break;
            default:
                break;
        }
    }
}

} // namespace hidtool
