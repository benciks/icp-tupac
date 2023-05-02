#include "game.h"
#include "maze.h"
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QMessageBox>

#include <QTimer>

Game::Game(QWidget *parent) : QWidget(parent)
{
#if __APPLE__
    maze = new Maze("../../../logic/maze.txt");
#else
    maze = new Maze("../src/logic/maze.txt");
#endif

    // setFixedSize(maze->getCols() * 20, maze->getRows() * 20); // Set the fixed size for the widget
    setFocusPolicy(Qt::StrongFocus); // Set focus policy
    setFocus();                      // Set focus on the widget

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Game::movePacman);
    moveTimer->start(200);
    keyCollected = !maze->hasKey();
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
        pacman->move(currentDirection, *maze, keyCollected);

        // Check if Pacman is on a key
        MazeElement *nextElement = maze->getElementAt(pacman->getRow(), pacman->getCol());
        if (nextElement->getSymbol() == 'K')
        {
            keyCollected = true;
        }

        // Update maze
        maze->setElementAt(pacmanRow, pacmanCol, new Empty());

        // check if Pac-Man is on target and key is collected
        if (maze->getElementAt(pacman->getRow(), pacman->getCol())->getSymbol() == 'T' && keyCollected)
        {
            qDebug() << "END OF GAME";
            moveTimer->stop();
            QMessageBox::information(this, "Game Over", "gadzo");
        }
        else
        {
            maze->setElementAt(pacman->getRow(), pacman->getCol(), pacman);
        }

        update();
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
                pacman->setPixmap(newDirection);
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
        painter.setBrush(QColor(80, 125, 188, 255));
        painter.drawRect(x, y, cellSize, cellSize);
        break;
    case 'G':
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Ghost *>(element)->getPixmap());
        break;
    case 'S':
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Pacman *>(element)->getPixmap());
        break;
    case 'T':
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Target *>(element)->getPixmap());
        break;
    case 'K':
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Key *>(element)->getPixmap());
        break;
    case '.':
    default:
        painter.setBrush(QColor(4, 8, 15, 255));
        painter.drawRect(x, y, cellSize, cellSize);
        break;
    }
}

void Game::paintMaze()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::transparent);
    // qDebug() << "paintMaze() called";

    // Set the background color
    painter.setBackground(QBrush(QColor(4, 8, 15, 255)));
    painter.eraseRect(rect());

    // Calculate the cell size based on the width and height of the widget
    int cellSize = qMin(width() / maze->getCols(), height() / maze->getRows());

    // Calculate the offset to center the maze
    int xOffset = (width() - cellSize * maze->getCols()) / 2;
    int yOffset = (height() - cellSize * maze->getRows()) / 2;

    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);

            int x = xOffset + j * cellSize;
            int y = yOffset + i * cellSize;

            paintElement(painter, element, x, y, cellSize);
        }
    }
}

void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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

    update(rect()); // Trigger a repaint after moving Pacman
}
