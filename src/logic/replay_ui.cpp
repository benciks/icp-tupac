#include "replay_ui.h"
#include <QPainter>

ReplayUI::ReplayUI(QWidget *parent, QString fileName) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // Set focus policy
    setFocus();                      // Set focus on the widget

    replay = new Replay(fileName.toStdString());
    replayTimer = new QTimer(this);
    connect(replayTimer, SIGNAL(timeout()), this, SLOT(replayStep()));
    replayTimer->start(1000);
}

void ReplayUI::replayStep()
{
    replay->nextMove();
    update();
}

void ReplayUI::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintMaze(painter);
}

void ReplayUI::paintMaze(QPainter &painter)
{
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::transparent);

    // Set the background color
    painter.setBackground(QBrush(QColor(4, 8, 15, 255)));
    painter.eraseRect(rect());

    // Calculate the cell size based on the width and height of the widget
    int cellSize = qMin(width() / replay->getCols(), height() / replay->getRows());

    // Calculate the offset to center the maze
    int xOffset = (width() - cellSize * replay->getCols()) / 2;
    int yOffset = (height() - cellSize * replay->getRows()) / 2;

    for (int i = 0; i < replay->getRows(); i++)
    {
        for (int j = 0; j < replay->getCols(); j++)
        {
            int x = xOffset + j * cellSize;
            int y = yOffset + i * cellSize;
            paintElement(painter, replay->getGrid()[i][j], x, y, cellSize);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ReplayUI::paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize)
{
    char symbol = element->getSymbol();
    switch (symbol)
    {
    case 'X':
        std::cout << "X";
        painter.setBrush(QColor(33, 33, 255, 255));
        painter.drawRect(x, y, cellSize, cellSize);
        break;
    case 'G':
        std::cout << "G";
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Ghost *>(element)->getPixmap());
        break;
    case 'S':
        std::cout << "S";
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Pacman *>(element)->getPixmap());
        break;
    case 'T':
        std::cout << "T";
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Target *>(element)->getPixmap());
        break;
    case 'K':
        std::cout << "K";
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Key *>(element)->getPixmap());
        break;
    case ' ':
        std::cout << " ";
        painter.setBrush(QColor(4, 8, 15, 255));
        painter.drawRect(x, y, cellSize, cellSize);
        break;
    case '.':
    default:
        std::cout << ".";
        painter.drawPixmap(x, y, cellSize, cellSize, static_cast<Collectible *>(element)->getPixmap());
        break;
    }
}