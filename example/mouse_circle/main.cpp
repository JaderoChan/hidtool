#include <cmath>
#include <cstdio>
#include <thread>

#include <hidtool/hidtool.hpp>

using namespace hidtool;

int main(int argc, char* argv[])
{
    constexpr double PI = 3.1415926;

    MouseSimulator& msSim = MouseSimulator::getInstance();
    msSim.initialize();

    AbsMoveRange range = MouseSimulator::getAbsMoveRange();
    int32_t halfWidth = (range.maxX - range.minX) / 2;
    int32_t halfHeight = (range.maxY - range.minY) / 2;

    double th = 0.0;
    while (th < 360.0)
    {
        int32_t x = halfWidth + halfWidth * cos(th * PI / 180.0);
        int32_t y = halfHeight + halfHeight * sin(th * PI / 180.0);
        msSim.moveTo(x, y);
        std::this_thread::sleep_for(std::chrono::microseconds(200));
        th += 0.1;
    }

    msSim.destroy();

    return 0;
}
