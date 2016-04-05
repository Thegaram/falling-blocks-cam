#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <vector>
#include <set>
#include <utility>
#include <random>

#include "./Box.h"
#include "./Bullet.h"
#include "./Player.h"
#include "./Graphics.h"
#include "../common/SharedState.h"
#include "../common/Logger.h"

static std::random_device rndDevice;

class Controller
{
private:
    SharedState& sharedState;
    Logger& logger;
    Graphics& graphics;

    std::vector<Box> boxes;
    std::vector<Bullet> bullets;

    int numBoxes;

    Player player;
    double playerPosition = 10.0;

    bool gameOver = false;

    double currentTime;

    double randomBoxX() const
    {
        double rnd;
        bool found = false;

        while (!found)
        {
            rnd = (double) (rndDevice() % 100) / 100 * 100;
            found = true;

            for (const auto& box : boxes)
            {
                auto pos = box.getPosition();
                if (fabs(rnd - pos.first) < 5.f && fabs(102.5 - pos.second) < 5.f)
                {
                    found = false;
                    break;
                }
            }
        }

        return rnd;
    }

    double randomBoxVelocity() const
    {
        double rnd = 5 + (double) (rndDevice() % 100) / 100 * 5;
        return rnd;
    }

    Box createBox() const
    {
        return Box{{randomBoxX(), 102.5}, randomBoxVelocity()};
    }

    Controller(int numBoxes = 10):
        player({50, 2.5}),
        sharedState(SharedState::getInstance()),
        logger(Logger::getInstance()),
        graphics(Graphics::getInstance()),
        numBoxes(numBoxes)
    {
        // TODO: init boxes
        for (int ii = 0; ii < numBoxes; ++ii)
            boxes.push_back(createBox());

        currentTime = 0;

        // TODO: init player
    }

public:
    static Controller& getInstance()
    {
        static Controller instance;
        return instance;
    }

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    bool update(double dt)
    {
        // update bullet position
        std::set<int> bulletsToDelete;
        for (int ii = 0; ii < bullets.size(); ++ii)
        {
            bool destroyed = bullets[ii].updatePosition(dt);
            if (destroyed)
                bulletsToDelete.insert(ii);
        }

        // update box positions
        std::set<int> boxesToDelete;
        for (int ii = 0; ii < boxes.size(); ++ii)
        {
            bool destroyed = boxes[ii].updatePosition(dt);
            if (destroyed)
                boxesToDelete.insert(ii);
        }

        int numBoxesDestroyed = boxesToDelete.size();

        // check bullet hits
        for (int ii = 0; ii < bullets.size(); ++ii)
        {
            int boxId = bullets[ii].checkCollision(boxes);
            if (boxId > -1)
            {
                boxesToDelete.insert(boxId);
                bulletsToDelete.insert(ii);
                ++numBoxesDestroyed;
                // TODO: same id twice?
            }
        }

        // remove destroyed boxes
        for (int id : boxesToDelete)
        {
            boxes[id] = boxes.back();
            boxes.pop_back();
        }

        // remove destroyed bullets
        for (int id : bulletsToDelete)
        {
            bullets[id] = bullets.back();
            bullets.pop_back();
        }

        for (int ii = 0; ii < numBoxesDestroyed; ++ii)
            boxes.push_back(createBox());

        // update player position
        double newPlayerPosition = sharedState.getHeadPositionX() * 100;
        float alpha = 0.9;
        playerPosition = alpha * playerPosition + (1 - alpha) * newPlayerPosition;
        player.setPosition(playerPosition);

        return player.checkCollision(boxes);
    }

    void draw()
    {
        static float eyePosY = 0.f;
        static const float alpha = 0.95;

        float newEyePosY = -5 + 10 * sharedState.getHeadPositionY();
        eyePosY = alpha * eyePosY + (1 - alpha) * newEyePosY;
        graphics.setEyePosY(eyePosY);

        if (eyePosY < 0.f)
            graphics.drawCube(glm::vec3(50, -55, 0), 110, Graphics::GROUND);

        // TODO
        for (auto& box : boxes)
            box.draw();

        for (auto& bullet : bullets)
            bullet.draw();

        player.draw();

        if (gameOver)
        {
            sharedState.setGameOver();
            drawGameOver();
        }
    }

    void drawGameOver() const
    {
        graphics.drawSuzanne(glm::vec3(50, 50, 0), 50);
    }

    void handleNextCommand()
    {
        Command nextCommand = sharedState.getCommand();

        switch(nextCommand)
        {
            case Command::SHOOT:
                logger.debug("SHOOT!");

                bullets.push_back(Bullet{
                    player.getPosition(),
                    -25
                });

                break;

            case Command::NOOP: break;
        }
    }

    void step()
    {
        double newTime = glfwGetTime();
        double dt = newTime - currentTime;
        currentTime = newTime;

        handleNextCommand();

        if (!gameOver)
        {
            bool collision = update(dt);

            if (collision)
                gameOver = true;
        }

        draw();
    }
};

#endif // CONTROLLER_H_INCLUDED