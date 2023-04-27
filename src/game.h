#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <Maze/Maze.h>

class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void gameOver() {};

public slots:
    void startGame() {};
};

#endif // GAME_H