#include <chrono>
#include <thread>

#include "./input.h"
#include "../common/SharedState.h"
#include "./FaceDetector.h"

using namespace std;

void inputLoop()
{
    SharedState& state = SharedState::getInstance();
    double posX = 0.5;

    FaceDetector fd;
    fd.load();

    while(true)
    {
        fd.read();
        posX = fd.detect();

        state.setHeadPositionX(posX);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}