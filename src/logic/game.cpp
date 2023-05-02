#include "game.h"
#include "maze.h"
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QFocusEvent>

#include <QTimer>

Game::Game(QWidget *parent) : QWidget(parent)
{
    maze = new Maze("maze.txt");
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    moveTimer = new QTimer(this); // Initialize moveTimer
    connect(moveTimer, &QTimer::timeout, this, &Game::movePacman); // Connect moveTimer timeout signal to movePacman slot
    moveTimer->start(200); // Start moveTimer with a 200 ms interval
}

void Game::movePacman()
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
        Direction currentDirection = pacman->getCurrentDirection();
        pacman->move(currentDirection, *maze);

        // Update maze
        maze->setElementAt(pacmanRow, pacmanCol, new Empty());
        maze->setElementAt(pacman->getRow(), pacman->getCol(), pacman);

        update(); // Add this line to update the display after moving Pacman
    }
}

void Game::rotatePacman(Direction newDirection)
{
    // Find Pacman
    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);
            if (element->getSymbol() == 'S')
            {
                Pacman *pacman = dynamic_cast<Pacman *>(element);
                pacman->setCurrentDirection(newDirection);
                break;
            }
        }
    }
}

void Game::paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize)
{
    char symbol = element->getSymbol();
    switch (symbol)
    {
    case 'X':
        painter.setBrush(Qt::black);
        break;
    case 'G':
        painter.setBrush(static_cast<Ghost *>(element)->getColor());
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
            MazeElement *element = maze->getElementAt(i, j);
            paintElement(painter, element, j, i, cellSize);
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
        rotatePacman(Direction::UP);
        break;
    case Qt::Key_Down:
        rotatePacman(Direction::DOWN);
        break;
    case Qt::Key_Left:
        rotatePacman(Direction::LEFT);
        break;
    case Qt::Key_Right:
        rotatePacman(Direction::RIGHT);
        break;
    default:
        QWidget::keyPressEvent(event);
    }

    // Don't update here, update will be called in the game loop
}