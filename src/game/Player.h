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
        for (const Box& box : boxes)
        {
            auto boxPosition = box.getPosition();
            double diffX = position.first - boxPosition.first;
            double diffY = position.second - boxPosition.second;

            if (std::abs(position.first - boxPosition.first)   < 0.1 &&
                std::abs(position.second - boxPosition.second) < 0.1)
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