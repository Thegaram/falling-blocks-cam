#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <vector>
#include <set>
#include <utility>
#include <random>

#include "./Box.h"
#include "./Bullet.h"
#include "./Player.h"
#include "../common/SharedState.h"
#include "../common/Logger.h"

#include <GL/glut.h>
#include <OpenGL/gl3.h>

static std::random_device rndDevice;

class Controller
{
private:
    SharedState& sharedState;
    Logger& logger;

    std::vector<Box> boxes;
    std::vector<Bullet> bullets;

    int numBoxes;

    Player player;
    double playerPosition = 0.5;

    bool gameOver = false;

    double randomBoxX() const
    {
        double rnd = -1 + (double) (rndDevice() % 100) / 100 * 2;
        return rnd;
    }

    double randomBoxVelocity() const
    {
        double rnd = 0.0001 + (double) (rndDevice() % 100) / 100 * 0.0001;
        return rnd;
    }

    Box createBox() const
    {
        return Box{{randomBoxX(), 1}, randomBoxVelocity()};
    }

public:
    Controller(int numBoxes = 10):
        player({0, -1}),
        sharedState(SharedState::getInstance()),
        logger(Logger::getInstance()),
        numBoxes(numBoxes)
    {
        // TODO: init boxes
        for (int ii = 0; ii < numBoxes; ++ii)
            boxes.push_back(createBox());

        // TODO: init player
    }

    bool update(long dt)
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
        double newPlayerPosition = -1 + sharedState.getHeadPositionX() * 2;
        playerPosition = (5 * playerPosition + newPlayerPosition) / 6;
        player.setPosition(playerPosition);

        return player.checkCollision(boxes);
    }

    void draw()
    {
        // TODO
        for (auto& box : boxes)
            box.draw();

        for (auto& bullet : bullets)
            bullet.draw();

        player.draw();

        if (gameOver)
            drawGameOver();
    }

    void renderString(GLdouble x, GLdouble y, const std::string& str) const
    {
        glColor3d(1.0, 0.0, 0.0);
        glRasterPos2d(x, y);
        for (char ch : str)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch);
    }

    void drawGameOver() const
    {
        renderString(0, 0, "GAME OVER");
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
                    -0.0005
                });

                break;

            case Command::NOOP: break;
        }
    }

    void mainLoop(long dt)
    {
        handleNextCommand();

        if (!gameOver) {
            bool collision = update(dt);

            if (collision)
                gameOver = true;
        }

        draw();
    }
};

#endif // CONTROLLER_H_INCLUDED