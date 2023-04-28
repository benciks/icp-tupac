#include "game.h"
#include "maze.h"
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QDebug>

Game::Game(QWidget *parent) : QWidget(parent)
{
    maze = new Maze("maze.txt");
}

void Game::paintMaze()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    qDebug() << "paintMaze() called";

    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            switch (maze->getElementAt(i, j)->getSymbol())
            {
            case 'W':
                painter.setBrush(Qt::black);
                painter.drawRect(i + 10, j + 10, 10, 10);
            case 'G':
                painter.setBrush(Qt::red);
                painter.drawRect(i + 10, j + 10, 10, 10);
            case 'P':
                painter.setBrush(Qt::yellow);
                painter.drawRect(i + 10, j + 10, 10, 10);
            case 'S':
                painter.setBrush(Qt::green);
                painter.drawRect(i + 10, j + 10, 10, 10);
            case 'T':
                painter.setBrush(Qt::blue);
                painter.drawRect(i + 10, j + 10, 10, 10);
            default:
                painter.setBrush(Qt::white);
                painter.drawRect(i + 10, j + 10, 10, 10);
            }
        }
    }
}

void Game::paintEvent(QPaintEvent *event)
{
    paintMaze();
}