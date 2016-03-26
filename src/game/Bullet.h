#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "./GameObject.h"
#include <OpenGL/gl3.h>

#include <iostream>

class Bullet final : public GameObject
{
private:
    double velocity;

public:
    Bullet(std::pair<double, double> pos, double v): GameObject(pos), velocity(v) { }

    virtual bool updatePosition(long dt) override
    {
        position.second -= velocity * dt;

        return position.second > 1.1;
    }

    virtual void subdraw() const override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
            glColor3f(0.1, 0.2, 0.3);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.05f, 0.0f);
            glVertex3f(0.0f, 0.05f, 0.0f);

            glVertex3f(0.0f, 0.0f, 0.05f);
            glVertex3f(0.05f, 0.0f, 0.05f);
            glVertex3f(0.05f, 0.05f, 0.05f);
            glVertex3f(0.0f, 0.05f, 0.05f);

            glColor3f(1.0, 0.2, 0.3);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.0f, 0.05f);
            glVertex3f(0.0f, 0.0f, 0.05f);

            glVertex3f(0.0f, 0.05f, 0.0f);
            glVertex3f(0.05f, 0.05f, 0.0f);
            glVertex3f(0.05f, 0.05f, 0.05f);
            glVertex3f(0.0f, 0.05f, 0.05f);

            glColor3f(0.1, 1.0, 0.3);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.05f, 0.0f);
            glVertex3f(0.0f, 0.05f, 0.05f);
            glVertex3f(0.0f, 0.0f, 0.05f);

            glVertex3f(0.05f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.05f, 0.0f);
            glVertex3f(0.05f, 0.05f, 0.05f);
            glVertex3f(0.05f, 0.0f, 0.05f);
        glEnd();
    }

    int checkCollision(std::vector<Box> boxes) const
    {
        for (int ii = 0; ii < boxes.size(); ++ii)
        {
            auto boxPosition = boxes[ii].getPosition();

            if (std::abs(position.first - boxPosition.first)   < 0.05 &&
                std::abs(position.second - boxPosition.second) < 0.05)
            {
                return ii;
            }
        }

        return -1;
    }
};

#endif // BULLET_H_INCLUDED