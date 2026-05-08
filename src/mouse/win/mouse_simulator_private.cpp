#include "mouse_simulator_private.hpp"

#include <future>       // promise, future
#include <vector>       // vector

#include <windows.h>    // INPUT, SendInput()

#include "input_factory.hpp"

namespace hidt
{

MouseSimulatorPrivate::~MouseSimulatorPrivate() { destroy(); }

MouseSimulatorPrivate& MouseSimulatorPrivate::getInstance()
{
    static MouseSimulatorPrivate instance;
    return instance;
}

AbsolutePosRange MouseSimulatorPrivate::getAbsoluteMoveRange()
{
    AbsolutePosRange result;
    VirtualScreenInfo vsi = VirtualScreenInfo::getLatest();

    result.minX = static_cast<int32_t>(vsi.x);
    result.minY = static_cast<int32_t>(vsi.y);
    result.maxX = static_cast<int32_t>(vsi.x) + static_cast<int32_t>(vsi.width);
    result.maxY = static_cast<int32_t>(vsi.y) + static_cast<int32_t>(vsi.height);

    return result;
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
        case MouseEvent::ET_ABS_MOVE:   // Fallthrough
        case MouseEvent::ET_DRAG:
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
            case MouseEvent::ET_ABS_MOVE:   // Fallthrough
            case MouseEvent::ET_DRAG:
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

} // namespace hidt
