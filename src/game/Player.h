#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "./GameObject.h"
#include "./Box.h"

#include <vector>
#include <cmath>
#include <iostream>

class Player final : public GameObject
{
public:
    Player(std::pair<double, double> pos): GameObject(pos) { }

    virtual bool updatePosition(double dt) override
    {
        return false;
        // TODO
    }

    void setPosition(double posX)
    {
        position.first = posX;
    }

    virtual void draw() const override
    {
        graphics->drawCube(glm::vec3(position.first, position.second, 0), 5);
    }

    bool checkCollision(std::vector<Box> boxes) const
    {
        for (const Box& box : boxes)
        {
            auto boxPosition = box.getPosition();
            double diffX = position.first - boxPosition.first;
            double diffY = position.second - boxPosition.second;

            // std::cout << diffX << ",\t" << diffY << std::endl;

            if (std::fabs(position.first - boxPosition.first)   < 5.f &&
                std::fabs(position.second - boxPosition.second) < 5.f)
            {
                // std::cout << '(' << position.first    << ',' << position.second    << ") ~ "
                //           << '(' << boxPosition.first << ',' << boxPosition.second << ')' << std::endl;

                return true;
            }
        }

        return false;
    }
};

#endif // PLAYER_H_INCLUDED