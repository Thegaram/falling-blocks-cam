#include <iostream>
#include <thread>

#include "./common/Logger.h"
#include "./input/input.h"
#include "./game/game.h"

int main(int argc, char *argv[])
{
    Logger& logger = Logger::getInstance();
    logger.info("Starting threads");

    std::thread input(inputLoop);
    std::thread game(gameLoop);

    game.join();
    input.join();

    logger.info("Threads completed");

    return 0;
}
