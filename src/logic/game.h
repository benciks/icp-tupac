#ifndef ICP_PACMAN_GAME_H
#define ICP_PACMAN_GAME_H

#include <QWidget>
#include "maze.h"

#include <QTimer>

class Game : public QWidget
{
public:
    Game(QWidget *parent = nullptr);
    void paintMaze();
    void rotatePacman(Direction direction);

private:
    Maze *maze;
    QTimer *moveTimer;
    bool exitOpened = false;
    int keysCollected = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void movePacman();
};

#endif // ICP_PACMAN_GAME_H