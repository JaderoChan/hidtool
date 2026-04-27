#include <cstdio>
#include <condition_variable>
#include <mutex>

#include <hidtool/hidtool.hpp>

using namespace hidtool;

bool shouldClose = false;
std::mutex shouldCloseMtx;
std::condition_variable shouldCloseCv;

static const char* getMouseButtonString(MouseButton button)
{
    switch (button)
    {
        case MouseButton::MSBTN_LEFT:
            return "Left";
        case MouseButton::MSBTN_RIGHT:
            return "Right";
        case MouseButton::MSBTN_MIDDLE:
            return "Middle";
        case MouseButton::MSBTN_BACK:
            return "Back";
        case MouseButton::MSBTN_FORWARD:
            return "Forward";
        default:
            return "Unknown button";
    }
}

static bool keyboardEventHandler(const KeyboardEvent& event)
{
    auto key = keyboardKeyFromNativeKey(event.nativeKey);
    switch (event.eventType)
    {
        case KeyboardEvent::ET_PRESS:
            printf("Key Pressed: %d.\n", key);
            if (key == KBDKEY_ESC)
            {
                std::lock_guard<std::mutex> locker(shouldCloseMtx);
                shouldClose = true;
                shouldCloseCv.notify_one();
            }
            break;
        case KeyboardEvent::ET_RELEASE:
            printf("Key Released: %d.\n", key);
            break;
        default:
            break;
    }

    return true;
}
static bool mouseEventHandler(const MouseEvent& event)
{
    switch (event.eventType)
    {
        case MouseEvent::ET_ABS_MOVE:
            printf("Mouse absolute move: [%d, %d].\n", event.absPos.x, event.absPos.y);
            break;
        case MouseEvent::ET_REL_MOVE:
            printf("Mouse relative move: [%d, %d].\n", event.relPos.dx, event.relPos.dy);
            break;
        case MouseEvent::ET_WHEEL:
            printf("Mouse wheel: %d.\n", event.wheelDelta);
            break;
        case MouseEvent::ET_PRESS:
            printf("Mouse button pressed: %s.\n", getMouseButtonString(event.button));
            break;
        case MouseEvent::ET_RELEASE:
            printf("Mouse button released: %s.\n", getMouseButtonString(event.button));
            break;
        default:
            break;
    }

    return true;
}

int main(int argc, char* argv[])
{
    KeyboardHooker& kbdHooker = KeyboardHooker::getInstance();
    if (!kbdHooker.setEventHandler(&keyboardEventHandler))
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

    printf("Successfully run the keyboard hooker and mouse hooker.\n");
    printf("Please press 'ESC' to exit program.\n");

    {
        std::unique_lock<std::mutex> locker(shouldCloseMtx);
        shouldCloseCv.wait(locker, [&]() { return shouldClose; });
    }

    kbdHooker.stop();
    msHooker.stop();

    return 0;
}
