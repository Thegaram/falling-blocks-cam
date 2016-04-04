#ifndef GAMEOBJECT_H_DEFINED
#define GAMEOBJECT_H_DEFINED

#include <utility>
#include "Graphics.h"

class GameObject
{
protected:
    std::pair<double, double> position;
    Graphics* graphics;


public:
    GameObject(std::pair<double, double> pos): position(pos), graphics(&Graphics::getInstance()) { }

    virtual bool updatePosition(double dt) = 0;

    virtual void draw() const = 0;

    std::pair<double, double> getPosition() const
    {
        return position;
    }

    virtual ~GameObject() {}
};

#endif // GAMEOBJECT_H_DEFINED