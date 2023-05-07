/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file game.cpp
 * @brief Game implementation file.
 */

#include "game.h"
#include "maze.h"

#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <filesystem>
#include <fstream>

Game::Game(QWidget *parent, QString fileName) : QWidget(parent)
{
    if (fileName == "")
    {
#ifdef __APPLE__
        QString defaultMazePath = QCoreApplication::applicationDirPath() + "/../../../examples/maps/original.txt";
#else
        QString defaultMazePath = QCoreApplication::applicationDirPath() + "/examples/maps/original.txt";
#endif
        QFileInfo mazeFileInfo(defaultMazePath);

        if (mazeFileInfo.exists() && mazeFileInfo.isFile())
        {
            maze = new Maze(mazeFileInfo.absoluteFilePath().toStdString());
        }
        else
        {
            QMessageBox::critical(this, "Error", "<font color='red'>Couldn't locate default maze file\n" + mazeFileInfo.absoluteFilePath() + "</font>");
            exit(1);
        }
    }
    else
    {
        maze = new Maze(fileName.toStdString());
    }
    // setFixedSize(maze->getCols() * 20, maze->getRows() * 20); // Set the fixed size for the widget
    setFocusPolicy(Qt::StrongFocus); // Set focus policy
    setFocus();                      // Set focus on the widget

    ghostTimer = new QTimer(this);
    connect(ghostTimer, &QTimer::timeout, this, &Game::moveGhosts);
    ghostTimer->start(400);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Game::movePacman);
    moveTimer->start(150);

    // Generate a random file name for the save file in replays folder
    QString replayPath;
#ifdef __APPLE__
    replayPath = QCoreApplication::applicationDirPath() + "/../../../examples/replays/";
#else
    replayPath = QCoreApplication::applicationDirPath() + "/examples/replays/";
#endif

    QDir().mkdir(replayPath);
    replayFileName = replayPath + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";

    exitOpened = maze->getKeys() == 0;
}

Game::~Game()
{
    delete maze;
    delete moveTimer;
    delete ghostTimer;
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

        if (nextElement->getSymbol() == 'G')
        {
            endGame();
        }

        // Update maze
        maze->setElementAt(pacmanRow, pacmanCol, new Empty());

        // check if Pac-Man is on target and key is collected
        if (maze->getElementAt(pacman->getRow(), pacman->getCol())->getSymbol() == 'T' && exitOpened)
        {
            // Victory boie
            endGame(true);
        }
        else
        {
            maze->setElementAt(pacman->getRow(), pacman->getCol(), pacman);
        }

        update();
        saveGame();
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
    // Create ghost array
    std::vector<Ghost *> ghosts;

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

    for (size_t i = 0; i < ghosts.size(); i++)
    {
        MazeElement *previousElement = ghosts[i]->getCurrent();
        int ghostRow = ghosts[i]->getRow();
        int ghostCol = ghosts[i]->getCol();

        // Replace current
        ghosts[i]->chase(*pacman, *maze);
        int newGhostRow = ghosts[i]->getRow();
        int newGhostCol = ghosts[i]->getCol();

        if (maze->isGhostPositionValid(newGhostRow, newGhostCol))
        {
            MazeElement *current = maze->getElementAt(newGhostRow, newGhostCol);
            ghosts[i]->setCurrent(current);

            // Set previous element back to the maze
            if (previousElement != nullptr)
            {
                maze->setElementAt(ghostRow, ghostCol, previousElement);
            }
            else
            {
                maze->setElementAt(ghostRow, ghostCol, new Empty());
            }

            // Check if ghost is going to reach pacman
            if (current->getSymbol() == 'S')
            {
                endGame();
            }

            maze->setElementAt(newGhostRow, newGhostCol, ghosts[i]);
        }
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

void Game::paintMaze(QPainter &painter)
{
    // QPainter painter(this);
    // if (!painter.isActive())
    //  {
    //      painter.begin(this);
    //  }
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::transparent);

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
    QPainter painter(this);
    paintMaze(painter);
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
}

void Game::endGame(bool victory)
{
    moveTimer->stop();
    ghostTimer->stop();

    emit gameOver(victory);
}

void Game::saveGame()
{
    // Open the file
    std::ofstream file(replayFileName.toStdString(), std::ios::app);
    file << maze->getCols() << " " << maze->getRows() << std::endl;
    // Iterate over the maze and save the elements
    for (int i = 0; i < maze->getRows(); i++)
    {
        for (int j = 0; j < maze->getCols(); j++)
        {
            MazeElement *element = maze->getElementAt(i, j);
            if (element->getSymbol() == ' ')
            {
                file << 'E';
            }
            else
            {
                file << element->getSymbol();
            }
        }
        file << std::endl;
    }

    file << "---" << std::endl;
    file.flush();
    file.close();
}