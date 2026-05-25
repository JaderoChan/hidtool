#include <cstdio>
#include <condition_variable>
#include <future>
#include <mutex>
#include <string>
#include <thread>

#include <hidtool/hidtool.hpp>

using namespace hidt;

std::atomic<AbsolutePos> currentPoint;
bool pointSelected;
std::promise<void> pointSelectedPromise;
std::atomic<AbsolutePos> selectedPoint;

bool shouldClose = false;
std::mutex shouldCloseMtx;
std::condition_variable shouldCloseCv;

static bool keyboardEventHandler(const KeyboardEvent& event, void* userData)
{
    if (event.type == KeyboardEvent::ET_PRESS)
    {
        auto key = keyboardKeyFromNativeKey(event.nativeKey);
        switch (key)
        {
            case KBDKEY_ESC:
            {
                if (!pointSelected)
                {
                    pointSelectedPromise.set_value();
                    pointSelected = true;
                }

                std::lock_guard<std::mutex> locker(shouldCloseMtx);
                shouldClose = true;
                shouldCloseCv.notify_one();

                break;
            }
            case KBDKEY_F5:
            {
                auto pos = currentPoint.load();
                selectedPoint.store(pos);

                if (!pointSelected)
                {
                    pointSelectedPromise.set_value();
                    pointSelected = true;
                }

                printf("Selected point: [%d, %d]\n", pos.x, pos.y);
                fflush(stdout);

                break;
            }
            case KBDKEY_F6:
            {
                if (!pointSelected)
                    break;

                auto pos = selectedPoint.load();
                std::thread th = std::thread([=]()
                {
                    auto& msSim = MouseSimulator::getInstance();
                    msSim.doubleClickButton(pos, MSBTN_LEFT);
                });

                th.detach();
            }
            default:
                break;
        }
    }

    return true;
}

static bool mouseEventHandler(const MouseEvent& event, void* userData)
{
    if (event.type == MouseEvent::ET_ABS_MOVE)
        currentPoint.store(event.absPos);
    return true;
}

int main(int argc, char* argv[])
{
    KeyboardHooker& kbdHooker = KeyboardHooker::getInstance();
    if (!kbdHooker.setEventHandler(keyboardEventHandler))
    {
        printf("Failed to set the keyboard event handler.\n");
        return -1;
    }
    if (!kbdHooker.run())
    {
        printf("Failed to run the keyboard hooker.\n");
        return -1;
    }

    MouseHooker& msHooker = MouseHooker::getInstance();
    if (!msHooker.setEventHandler(&mouseEventHandler))
    {
        printf("Failed to set the mouse event handler.\n");
        return -1;
    }
    if (!msHooker.run())
    {
        printf("Failed to run the mouse hooker.\n");
        return -1;
    }

    MouseSimulator& msSim = MouseSimulator::getInstance();
    if (!msSim.initialize())
    {
        printf("Failed to initialize mouse simulator.\n");
        return -1;
    }

    printf("1. Move your mouse to point you want to clicked.\n");
    printf("2. Press 'F5' to set point.\n");
    printf("3. Press 'F6' to double click point you selected.\n\n");
    printf("(Press 'ESC' to exit program).\n" );

    printf("Wait set the point...\n");
    fflush(stdout);

    {
        auto fut = pointSelectedPromise.get_future();
        fut.wait();
    }

    {
        std::unique_lock<std::mutex> locker(shouldCloseMtx);
        shouldCloseCv.wait(locker, [&]() { return shouldClose; });
    }

    msSim.destroy();
    kbdHooker.stop();

    return 0;
}
