#include <chrono>
#include <thread>

#include "./input.h"
#include "../common/SharedState.h"

using namespace std;

void inputLoop()
{
    SharedState& state = SharedState::getInstance();
    double posX = 0;

    while(true)
    {
        state.setHeadPositionX(posX);
        posX += 0.01;
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}