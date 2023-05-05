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

    ghostTimer = new QTimer(this);
    connect(ghostTimer, &QTimer::timeout, this, &Game::moveGhosts);
    ghostTimer->start(600);

    exitOpened = maze->getKeys() == 0;
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
        pacman->move(currentDirection, *maze, exitOpened);

        // Check if Pacman is on a collectible
        MazeElement *nextElement = maze->getElementAt(pacman->getRow(), pacman->getCol());
        if (nextElement->getSymbol() == '.')
        {
            score += 10;              // Update the score accordingly
            emit scoreChanged(score); // Emit the signal
            maze->setElementAt(pacman->getRow(), pacman->getCol(), new Empty());
        }

        // Check if Pacman is on a key
        if (nextElement->getSymbol() == 'K')
        {
            keysCollected++;
            if (keysCollected == maze->getKeys())
            {
                exitOpened = true;
                qDebug() << "EXIT OPENED";
            }
        }

        // Update maze
        maze->setElementAt(pacmanRow, pacmanCol, new Empty());

        // check if Pac-Man is on target and key is collected
        if (maze->getElementAt(pacman->getRow(), pacman->getCol())->getSymbol() == 'T' && exitOpened)
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

void Game::moveGhosts()
{
    Pacman *pacman = nullptr;

    // Find Pacman
    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);
            if (element->getSymbol() == 'S')
            {
                pacman = dynamic_cast<Pacman *>(element);
                break;
            }
        }

        if (pacman != nullptr)
            break;
    }

    // Find ghosts and add them to an array
    std::vector<Ghost *> ghosts;
    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);
            if (element->getSymbol() == 'G')
            {
                Ghost *ghost = dynamic_cast<Ghost *>(element);
                ghosts.push_back(ghost);
            }
        }
    }

    // Set the position of each ghost and update the maze
    for (int i = 0; i < ghosts.size(); i++)
    {
        maze->setElementAt(ghosts[i]->getRow(), ghosts[i]->getCol(), new Empty());
        ghosts[i]->chase(*pacman, *maze);
        maze->setElementAt(ghosts[i]->getRow(), ghosts[i]->getCol(), ghosts[i]);
    }

    update();
}

void Game::paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize)
{
    char symbol = element->getSymbol();
    switch (symbol)
    {
    case 'X':
        painter.setBrush(QColor(33, 33, 255, 255));
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
    case ' ':
        painter.setBrush(QColor(4, 8, 15, 255));
        painter.drawRect(x, y, cellSize, cellSize);
        break;
    case '.':
    default:
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Collectible *>(element)->getPixmap());
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
