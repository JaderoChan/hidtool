#ifndef HIDTOOL_HOOKER_PRIVATE_HPP
#define HIDTOOL_HOOKER_PRIVATE_HPP

#include <cstdint>  // uint8_t
#include <atomic>   // atomic
#include <list>     // list
#include <mutex>    // mutex, lock_guard
#include <string>   // string
#include <thread>   // thread
#include <vector>   // vector

#include <poll.h>           // pollfd
#include <linux/input.h>    // input_event

namespace hidt
{

class HookerPrivate
{
public:
    HookerPrivate() = default;
    virtual ~HookerPrivate();

    bool run();
    void stop();
    bool isRunning() const;

protected:
    // 提供一个抽象的事件处理程序设置接口，供派生类调用。
    template <typename T>
    bool setEventHandler(T eventHandler)
    {
        std::lock_guard<std::mutex> locker(operateMtx_);

        // 如果当前 Hooker 未启动，直接设置事件处理程序。
        if (!isRunning_.load())
        {
            eventHandler_ = reinterpret_cast<intptr_t>(eventHandler);
            return true;
        }

        // 否则，发送事件处理程序设置事件。
        WorkEvent event{WorkEvent::SET_EVENT_HANDLER, reinterpret_cast<intptr_t>(eventHandler)};
        return sendWorkEvent(event);
    }

    // 提供事件处理程序获取接口，供派生类调用。
    template <typename T>
    T getEventHandler() { return reinterpret_cast<T>(eventHandler_); }

    // 派生类实现：判断指定文件描述符对应的输入设备是否是需要监听的设备。
    virtual bool isAccessDevice(int fd) = 0;

    // 派生类实现：处理指定描述符对应的输入设备的输入事件。
    // 当输入设备有事件到来时会调用此函数。
    virtual void handleInputEvent(int fd) = 0;

private:
    struct WorkEvent
    {
        enum Type : uint8_t
        {
            END,
            SET_EVENT_HANDLER
        };

        Type type;
        intptr_t eventHandler;
    };

    // 发送一个工作事件。
    bool sendWorkEvent(const WorkEvent& event);

    // 尝试添加指定名称的输入设备。
    bool addEvdevFd(const std::string& evdevName);
    // 尝试移除指定名称的输入设备。
    void removeEvdevFd(const std::string& evdevName);

    bool setupWorkEventFd();
    bool setupInotifyFd();
    bool setupEvdevFds();
    bool setupAllFds();

    void cleanupWorkEventFd();
    void cleanupInotifyFd();
    void cleanupEvdevFds();
    void cleanupAllFds();

    // 当工作事件队列发生改变时被调用。
    void handleWorkEvent(int fd, bool& shouldClose);
    // 输入设备发生变化时（输入设备文件夹发生改变）被调用。
    void handleEvdevChanged(int fd);

    void work();

    // 判断指定路径的文件是否是字符设备。
    static bool isCharacterDevice(const std::string& filepath);

    intptr_t eventHandler_ = 0;

    // 用于互斥 `run()`，`stop()` 和 `setEventHandler()` 操作。
    mutable std::mutex operateMtx_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;

    std::list<WorkEvent> workEvents_;
    mutable std::mutex workEventsMtx_;

    // 布局：
    // 前两个预留元素分别是供程序传递工作事件的 workEventPollfd 和监听输入设备文件夹变化的 inotifyPollfd。
    // 之后均供输入设备的 pollfd 使用。
    std::vector<struct pollfd> watchedPollfds_{2, pollfd{-1, 0, 0}};
    // 存放已配置输入设备的名称，与 `watchedPollfds_` 第二个元素之后的 pollfd 形成对应关系。
    std::vector<std::string> evdevNames_;
};

} // namespace hidt

#endif // !HIDTOOL_HOOKER_PRIVATE_HPP
