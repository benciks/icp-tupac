#ifndef ICP_PACMAN_GAME_H
#define ICP_PACMAN_GAME_H

#include <QWidget>
#include "maze.h"

class Game : public QWidget
{
public:
    Game(QWidget *parent = nullptr);
    void paintMaze();
    void movePacman(Direction direction); 

private:
    Maze *maze;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // ICP_PACMAN_GAME_H