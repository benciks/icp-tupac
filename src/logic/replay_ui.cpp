#include "replay_ui.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

ReplayUI::ReplayUI(QWidget *parent, QString fileName, bool start) : QWidget(parent)
{
    //setFocusPolicy(Qt::StrongFocus); // Set focus policy
    setFocusPolicy(Qt::ClickFocus);
    setFocus();                      // Set focus on the widget

    replay = new Replay(fileName.toStdString(), start);
    replayTimer = new QTimer(this);
    connect(replayTimer, SIGNAL(timeout()), this, SLOT(replayStep()));
    replayTimer->start(150);
}

void ReplayUI::switchMode()
{
    sequentialMode = !sequentialMode;
    if (sequentialMode)
    {
        replayTimer->stop();
    }
    else
    {
        replayTimer->start(150);
    }
    setFocus(); // Add this line to set focus on the widget
    qDebug() << "Switching mode"; 
}

void ReplayUI::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed:" << event->key();
    if (event->key() == Qt::Key_Space)
    {
        switchMode();
        return;
    }

    if (sequentialMode)
    {
        if (event->key() == Qt::Key_Right)
        {
            replay->nextMove();
            update();
        }
        else if (event->key() == Qt::Key_Left)
        {
            replay->prevMove();
            update();
        }
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void ReplayUI::replayStep()
{
    if (start)
    {
        replay->nextMove();
        update();
    }
    else
    {
        replay->prevMove();
        update();
    }
}

void ReplayUI::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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
    }
}

void ReplayUI::paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize)
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