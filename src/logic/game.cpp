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

void Game::paintElement(QPainter &painter, char symbol, int x, int y, int cellSize)
{
    switch (symbol)
    {
    case 'X':
        painter.setBrush(Qt::black);
        break;
    case 'G':
        painter.setBrush(Qt::red);
        break;
    case 'S':
        painter.setBrush(Qt::green);
        break;
    case 'T':
        painter.setBrush(Qt::blue);
        break;
    case '.':
    default:
        painter.setBrush(Qt::white);
        break;
    }
    painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
}

void Game::paintMaze()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    qDebug() << "paintMaze() called";

    int cellSize = 20;

    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            char symbol = maze->getElementAt(i, j)->getSymbol();
            paintElement(painter, symbol, j, i, cellSize);
        }
    }
}

void Game::paintEvent(QPaintEvent *event)
{
    paintMaze();
}
