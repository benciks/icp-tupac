#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <Maze/Maze.h>

class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject* parent = nullptr) : QObject(parent) {}

    void startGame() const;

signals:
    void gameOver();

};
#endif // GAME_H
