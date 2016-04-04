#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include <iostream>

#include "./GameObject.h"

class Box final : public GameObject
{
private:
    double velocity;

public:
    Box(std::pair<double, double> pos, double v): GameObject(pos), velocity(v) { }

    virtual bool updatePosition(double dt) override
    {
        position.second -= velocity * dt;

        return position.second < -2.5;
    }

    virtual void draw() const override
    {
        graphics->drawCube(glm::vec3(position.first, position.second, 0), 5);
    }
};

#endif // BOX_H_INCLUDED