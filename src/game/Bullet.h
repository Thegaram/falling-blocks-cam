#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <iostream>

#include "./GameObject.h"
#include "Box.h"

class Bullet final : public GameObject
{
private:
    double velocity;

public:
    Bullet(std::pair<double, double> pos, double v): GameObject(pos), velocity(v) { }

    virtual bool updatePosition(double dt) override
    {
        position.second -= velocity * dt;

        return position.second > 101.f;
    }

    virtual void draw() const override
    {
        graphics->drawCube(glm::vec3(position.first, position.second, 0), 1.5, Graphics::FIRE);
    }

    int checkCollision(std::vector<Box> boxes) const
    {
        for (int ii = 0; ii < boxes.size(); ++ii)
        {
            auto boxPosition = boxes[ii].getPosition();

            if (std::fabs(position.first - boxPosition.first)   < 3.25f &&
                std::fabs(position.second - boxPosition.second) < 3.25f)
            {
                return ii;
            }
        }

        return -1;
    }
};

#endif // BULLET_H_INCLUDED