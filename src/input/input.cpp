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
        if(state.isGameOver())
            break;

        bool isSmilingOld = fd.isSmiling;

        fd.read();
        fd.detect();

        state.setHeadPositionX(fd.headPosX);
        state.setHeadPositionY(fd.headPosY);

        if (!isSmilingOld && fd.isSmiling)
            state.pushCommand(Command::SHOOT);

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    fd.cleanup();
}