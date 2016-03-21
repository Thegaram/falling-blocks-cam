#ifndef GAMEOBJECT_H_DEFINED
#define GAMEOBJECT_H_DEFINED

#include <utility>

class GameObject
{
protected:
    std::pair<double, double> position;

public:
    GameObject(std::pair<double, double> pos): position(pos) { }

    virtual bool updatePosition(long dt) = 0;

    void draw() const {
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);

        glTranslatef(position.first, position.second, 0);

        subdraw();

        glPopMatrix();
    }

    virtual void subdraw() const = 0;

    virtual ~GameObject() {}
};

#endif // GAMEOBJECT_H_DEFINED