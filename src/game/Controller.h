#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <vector>
#include <utility>

#include <random>
#include <iostream>

#include "./Box.h"
#include "./Player.h"
#include "../common/SharedState.h"


static std::random_device rndDevice;

class Controller
{
private:
    SharedState& sharedState;
    std::vector<Box> boxes;

    int numBoxes;

    Player player;
    double playerPosition = 0.5;

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
        numBoxes(numBoxes)
    {
        // TODO: init boxes
        for (int ii = 0; ii < numBoxes; ++ii)
            boxes.push_back(createBox());

        // TODO: init player
    }

    bool update(long dt)
    {
        // update box positions
        std::vector<int> toDelete;
        for (int ii = 0; ii < boxes.size(); ++ii)
        {
            bool destroyed = boxes[ii].updatePosition(dt);
            if (destroyed)
                toDelete.push_back(ii);
        }

        int numDestroyed = toDelete.size();

        // remove destroyed boxes
        for (int id : toDelete)
        {
            boxes[id] = boxes.back();
            boxes.pop_back();
        }

        for (int ii = 0; ii < numDestroyed; ++ii)
            boxes.push_back(createBox());

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

        player.draw();
    }

    void mainLoop()
    {
        long dt; // TODO
        bool gameOver = update(dt);

        if (gameOver)
        {
            // TODO
            return;
        }

        draw();
    }
};

#endif // CONTROLLER_H_INCLUDED