#include "PacMan.h"

PacMan::PacMan(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

int PacMan::getX() const
{
    return this->posX;
}

int PacMan::getY() const
{
    return this->posY;
}

void PacMan::setPos(int x, int y)
{
    this->posX = x;
    this->posY = y;
}