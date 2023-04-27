#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>

class PacMan : public QObject
{
    Q_OBJECT

public:
    PacMan(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void gameOver();

public slots:
    void startGame()
    {
        // Start game loop
    }
};

#endif // PACMAN_H
