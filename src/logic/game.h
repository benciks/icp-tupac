#ifndef ICP_PACMAN_GAME_H
#define ICP_PACMAN_GAME_H

#include <QWidget>
#include "maze.h"
#include <QObject>
#include <QTimer>

class Game : public QWidget
{
    Q_OBJECT
public:
    Game(QWidget *parent = nullptr);
    void paintMaze();
    void rotatePacman(Direction direction);
    int getGhostCount() { return ghosts; };

private:
    Maze *maze;
    QTimer *moveTimer;
    QTimer *ghostTimer;
    bool exitOpened = false;
    int keysCollected = 0;
    int score = 0;
    int ghosts = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void movePacman();
    void moveGhosts();

signals:
    void scoreChanged(int newScore);
};

#endif // ICP_PACMAN_GAME_H