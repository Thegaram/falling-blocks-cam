#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "./GameObject.h"
#include "./Box.h"

#include <vector>

class Player final : public GameObject
{
public:
    Player(std::pair<double, double> pos): GameObject(pos) { }

    virtual bool updatePosition(long dt) override
    {
        return false;
        // TODO
    }

    void setPosition(double posX)
    {
        position.first = posX;
    }

    virtual void subdraw() const override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.2, 0.3);
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

    bool checkCollision(std::vector<Box> boxes) const
    {
        return false;
    }
};

#endif // PLAYER_H_INCLUDED