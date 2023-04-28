#ifndef ICP_PACMAN_GAME_H
#define ICP_PACMAN_GAME_H

#include <QWidget>
#include "maze.h"

class Game : public QWidget
{
public:
    Game(QWidget *parent = nullptr);
    void paintMaze();

private:
    Maze *maze;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // ICP_PACMAN_GAME_H