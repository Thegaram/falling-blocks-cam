
#include <iostream>
#include <exception>

#include "Graphics.h"
#include "../common/Logger.h"
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

    try
    {
        graphicsLoop();
    }
    catch (const std::runtime_error& error)
    {
        logger.error("Runtime error:", error.what());
    }
}
