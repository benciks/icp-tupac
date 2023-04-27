#include "game.h"
#include <Maze/Maze.h>

#include <QPainter>

Game::Game()

void Game::startGame() {
    // Initialise maze - load from file 
    *Maze maze = new Maze('maze.txt');
    *QPainer painter = new QPainer();

    // Draw maze
    for (int i = 0; i < maze->getRows(); i++) {
        for (int j = 0; j < maze->getCols(); j++) {
            switch (maze->getMaze()[i][j]) {
                case instanceof(Wall):
                    painter->setBrush(Qt::black);
                    painter->drawRect();
                case instanceof(Ghost):
                    painter->setBrush(Qt::red);
                    painter->drawRect();
                case instanceof(PacMan):
                    pointer->setBrush(Qt::yellow);
                    painter->drawRect();
                case instanceof(Start):
                    painter->setBrush(Qt::green);
                    painter->drawRect();
                case instanceof(Target):
                    painter->setBrush(Qt::blue);
                    painter->drawRect();
                default:
                    painter->setBrush(Qt::white);
                    painter->drawRect();
            }
        }
    }
}
