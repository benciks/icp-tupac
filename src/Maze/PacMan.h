#pragma once

class PacMan {
public:
    PacMan(int x, int y);

    int getX() const;
    int getY() const;
    void setPos(int x, int y);
    

private:
    int posX;
    int posY;
};
