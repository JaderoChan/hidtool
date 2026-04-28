#include "mouse_simulator_private.hpp"

#include <future>       // promise, future
#include <vector>       // vector

#include <windows.h>    // INPUT, SendInput()

#include "input_factory.hpp"

namespace hidtool
{

MouseSimulatorPrivate::~MouseSimulatorPrivate() { destroy(); }

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

bool MouseSimulatorPrivate::initialize()
{
    std::lock_guard<std::mutex> locker(initDestrMtx_);

    if (isInitialized_.load())
        return false;

    // 初始化虚拟屏幕空间信息。
    virtualScreenInfo_.store(VirtualScreenInfo::getLatest());

    std::promise<void> threadIdIsReady;
    auto fut = threadIdIsReady.get_future();

    // 创建屏幕显示变更检测线程。
    displayChangedHandlerThread_ = std::thread([=, &threadIdIsReady]()
    {
        displayChangedHandlerThreadId_ = GetCurrentThreadId();
        threadIdIsReady.set_value();

        // 屏幕显示变更处理。
        handleDisplayChanged();
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

    // 向屏幕显示变更检测线程发送退出事件。
    PostThreadMessageA(displayChangedHandlerThreadId_, WM_QUIT, 0, 0);
    // 等待线程退出。
    if (displayChangedHandlerThread_.joinable())
        displayChangedHandlerThread_.join();

    // 重置相关字段。
    displayChangedHandlerThreadId_ = 0;
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
    switch (event.type)
    {
        case MouseEvent::ET_ABS_MOVE:
            setAbsoluteMoveInput(input, event.absPos, virtualScreenInfo_.load());
            break;
        case MouseEvent::ET_REL_MOVE:
            setRelativeMoveInput(input, event.relPos);
            break;
        case MouseEvent::ET_WHEEL:
            setWheelInput(input, event.wheelDelta);
            break;
        case MouseEvent::ET_PRESS:
            if (!setPressButtonInput(input, event.button))
                return false;
            break;
        case MouseEvent::ET_RELEASE:
            if (!setReleaseButtonInput(input, event.button))
                return false;
            break;
        default:
            return false;
    }

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

size_t MouseSimulatorPrivate::sendEvent(const MouseEvent* events, size_t count)
{
    if (!isInitialized_.load())
        return 0;

    std::vector<INPUT> inputs;
    inputs.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        const auto& event = events[i];
        INPUT input = {0};

        switch (event.type)
        {
            case MouseEvent::ET_ABS_MOVE:
                setAbsoluteMoveInput(input, event.absPos, virtualScreenInfo_.load());
                break;
            case MouseEvent::ET_REL_MOVE:
                setRelativeMoveInput(input, event.relPos);
                break;
            case MouseEvent::ET_WHEEL:
                setWheelInput(input, event.wheelDelta);
                break;
            case MouseEvent::ET_PRESS:
                if (!setPressButtonInput(input, event.button))
                    continue;
                break;
            case MouseEvent::ET_RELEASE:
                if (!setReleaseButtonInput(input, event.button))
                    continue;
                break;
            default:
                continue;
        }

        inputs.emplace_back(std::move(input));
    }

    return SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
}

bool MouseSimulatorPrivate::moveTo(const AbsolutePos& absPos)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    setAbsoluteMoveInput(input, absPos, virtualScreenInfo_.load());

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::moveBy(const RelativePos& relPos)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    setRelativeMoveInput(input, relPos);

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::wheel(int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    setWheelInput(input, wheelDelta);

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::pressButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    if (!setPressButtonInput(input, button))
        return false;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::releaseButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT input = {0};
    if (!setReleaseButtonInput(input, button))
        return false;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool MouseSimulatorPrivate::clickButton(MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};
    if (!setPressButtonInput(inputs[0], button))
        return false;
    if (!setReleaseButtonInput(inputs[1], button))
        return false;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool MouseSimulatorPrivate::wheel(const AbsolutePos& absPos, int32_t wheelDelta)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};
    setAbsoluteMoveInput(inputs[0], absPos, virtualScreenInfo_.load());
    setWheelInput(inputs[1], wheelDelta);

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool MouseSimulatorPrivate::pressButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};
    setAbsoluteMoveInput(inputs[0], absPos, virtualScreenInfo_.load());
    if (!setPressButtonInput(inputs[1], button))
        return false;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool MouseSimulatorPrivate::releaseButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[2] = {0};
    setAbsoluteMoveInput(inputs[0], absPos, virtualScreenInfo_.load());
    if (!setReleaseButtonInput(inputs[1], button))
        return false;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool MouseSimulatorPrivate::clickButton(const AbsolutePos& absPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[3] = {0};
    setAbsoluteMoveInput(inputs[0], absPos, virtualScreenInfo_.load());
    if (!setPressButtonInput(inputs[1], button))
        return false;
    if (!setReleaseButtonInput(inputs[2], button))
        return false;

    return SendInput(3, inputs, sizeof(INPUT)) == 3;
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    INPUT inputs[3] = {0};
    if (!setPressButtonInput(inputs[0], button))
        return false;
    setAbsoluteMoveInput(inputs[1], endPos, virtualScreenInfo_.load());
    if (!setReleaseButtonInput(inputs[2], button))
        return false;

    return SendInput(3, inputs, sizeof(INPUT)) == 3;
}

bool MouseSimulatorPrivate::drag(const AbsolutePos& startPos, const AbsolutePos& endPos, MouseButton button)
{
    if (!isInitialized_.load())
        return false;

    auto vsi = virtualScreenInfo_.load();

    INPUT inputs[4] = {0};
    setAbsoluteMoveInput(inputs[0], startPos, vsi);
    if (!setPressButtonInput(inputs[1], button))
        return false;
    setAbsoluteMoveInput(inputs[2], endPos, vsi);
    if (!setReleaseButtonInput(inputs[3], button))
        return false;

    return SendInput(4, inputs, sizeof(INPUT)) == 4;
}

void MouseSimulatorPrivate::handleDisplayChanged()
{
    MSG msg = {0};
    // 强制系统创建消息循环。
    PeekMessageA(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

    while (GetMessageA(&msg, nullptr, 0, 0) != 0)
    {
        switch (msg.message)
        {
            // TODO：测试有效性。
            case WM_DISPLAYCHANGE:
                virtualScreenInfo_.store(VirtualScreenInfo::getLatest());
                break;
            default:
                break;
        }
    }
}

} // namespace hidtool
