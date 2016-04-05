
#include <iostream>
#include <exception>

#include "Graphics.h"
#include "../common/Logger.h"
#include "../common/SharedState.h"
#include "Controller.h"

void graphicsLoop()
{
    Graphics& graphics = Graphics::getInstance();
    Controller& controller = Controller::getInstance();

    while (true)
    {
        graphics.onLoopStart();

        controller.step();

        graphics.onLoopEnd();
    }
}

void gameLoop()
{
    Logger& logger = Logger::getInstance();
    SharedState& sharedState = SharedState::getInstance();

    try
    {
        graphicsLoop();
    }
    catch (const std::runtime_error& error)
    {
        logger.error("Runtime error:", error.what());
    }

    sharedState.setGameOver();
}
