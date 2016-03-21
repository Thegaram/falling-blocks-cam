#include <chrono>
#include <thread>

#include "./input.h"
#include "../common/SharedState.h"

using namespace std;

void inputLoop()
{
    SharedState& state = SharedState::getInstance();
    double posX = 0.5;

    while(true)
    {
        state.setHeadPositionX(posX);
        posX += 0.001;
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}