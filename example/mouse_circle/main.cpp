#include <cmath>
#include <cstdio>
#include <chrono>
#include <thread>

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

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    msSim.destroy();

    return 0;
}
