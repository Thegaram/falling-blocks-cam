#include <iostream>
#include <chrono>
#include <thread>

#include "./game.h"
#include "../common/SharedState.h"
#include "../common/Logger.h"

constexpr int WINDOW_WIDTH = 480;
constexpr int WINDOW_HEIGHT = 600;

using namespace std;

void gameLoop()
{
    SharedState& state = SharedState::getInstance();
    Logger& logger = Logger::getInstance();

    while(true)
    {
        double posX = state.getHeadPositionX();
        logger.debug(posX);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}