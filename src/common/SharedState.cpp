#include "./SharedState.h"

SharedState& SharedState::getInstance()
{
    static SharedState instance;
    return instance;
}

void SharedState::setHeadPositionX(double x)
{
    headPositionX = x;
}

double SharedState::getHeadPositionX() const
{
    return headPositionX;
}