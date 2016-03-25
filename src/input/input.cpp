#include <chrono>
#include <thread>

#include "./input.h"
#include "../common/SharedState.h"
#include "../common/Logger.h"
#include "./FaceDetector.h"

using namespace std;

void inputLoop()
{
    SharedState& state = SharedState::getInstance();
    Logger& logger = Logger::getInstance();
    double posX = 0.5;

    FaceDetector fd;
    fd.load();

    while(true)
    {
        fd.read();
        fd.detect();

        state.setHeadPositionX(fd.headPosX);
        state.setHeadPositionY(fd.headPosY);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}