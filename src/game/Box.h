#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include "./GameObject.h"
#include <OpenGL/gl3.h>

#include <iostream>

class Box final : public GameObject
{
private:
    double velocity;

public:
    Box(std::pair<double, double> pos, double v): GameObject(pos), velocity(v) { }

    virtual bool updatePosition(long dt) override
    {
        position.second -= velocity * dt;

        return position.second < -1.1;
    }

    virtual void subdraw() const override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
            glColor3f(0.1, 0.2, 0.3);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.1f, 0.0f, 0.0f);
            glVertex3f(0.1f, 0.1f, 0.0f);
            glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.1f, 0.0f, 0.0f);
            glVertex3f(0.1f, 0.1f, 0.0f);
            glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();
    }
};

#endif // BOX_H_INCLUDED