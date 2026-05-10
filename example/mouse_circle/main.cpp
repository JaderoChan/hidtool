#include <cmath>
#include <cstdio>

#ifndef _MSC_VER
    #include <chrono>
    #include <thread>
#endif

#include <hidtool/hidtool.hpp>

using namespace hidt;

int main(int argc, char* argv[])
{
    constexpr double PI = 3.1415926;

    MouseSimulator& msSim = MouseSimulator::getInstance();
    if (!msSim.initialize())
    {
        printf("Failed to initialize mouse simulator.\n");
        return -1;
    }

    auto absMoveRange = MouseSimulator::getAbsoluteMoveRange();
    printf("Absolute move range: (X[%d, %d], Y[%d, %d]).\n",
        absMoveRange.minX, absMoveRange.maxX, absMoveRange.minY, absMoveRange.maxY);

    int32_t halfWidth = (absMoveRange.maxX - absMoveRange.minX) / 2;
    int32_t halfHeight = (absMoveRange.maxY - absMoveRange.minY) / 2;

    double th = 0.0;
    while (th < 360.0)
    {
        int32_t x = halfWidth + halfWidth * cos(th * PI / 180.0);
        int32_t y = halfHeight + halfHeight * sin(th * PI / 180.0);
        msSim.moveTo({x, y});
        th += 0.05;

        // Disable sleep when using the MSVC compiler,
        // as MSVC's std::this_thread::sleep_for() has implementation flaws at the microsecond level.
        // It actually calls Windows's Sleep() function, whose minimum precision is approximately 15ms.
        // This will cause the program to make the mouse movement extremely slow after running.
    #ifndef _MSC_VER
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    #endif
    }

    msSim.destroy();

    return 0;
}
