#include "game.h"
#include "maze.h"
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

Game::Game(QWidget *parent) : QWidget(parent)
{
    maze = new Maze("maze.txt");
}

void Game::movePacman(Direction direction)
{
    Pacman *pacman = nullptr;
    int pacmanRow = -1;
    int pacmanCol = -1;

    // Find Pacman
    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);
            if (element->getSymbol() == 'S')
            {
                pacman = dynamic_cast<Pacman *>(element);
                pacmanRow = i;
                pacmanCol = j;
                break;
            }
        }

        if (pacman != nullptr)
            break;
    }

    // Move Pacman
    if (pacman != nullptr)
    {
        pacman->move(direction, *maze);

        // Update maze
        maze->setElementAt(pacmanRow, pacmanCol, new Empty());
        maze->setElementAt(pacman->getRow(), pacman->getCol(), pacman);
    }
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

void Game::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        movePacman(Direction::UP);
        break;
    case Qt::Key_Down:
        movePacman(Direction::DOWN);
        break;
    case Qt::Key_Left:
        movePacman(Direction::LEFT);
        break;
    case Qt::Key_Right:
        movePacman(Direction::RIGHT);
        break;
    default:
        QWidget::keyPressEvent(event);
    }

     update(rect()); // Trigger a repaint after moving Pacman
}
