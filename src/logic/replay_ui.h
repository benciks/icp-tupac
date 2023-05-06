#ifndef ICP_PACMAN_REPLAYUI_H
#define ICP_PACMAN_REPLAYUI_H

#include <QWidget>
#include "replay.h"
#include <QObject>
#include <QTimer>

class ReplayUI : public QWidget
{
    Q_OBJECT
public:
    ReplayUI(QWidget *parent = nullptr, QString fileName = "");
    void paintMaze(QPainter &painter);

private:
    Replay *replay;
    QTimer *replayTimer;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintMaze(QPainter &painter, std::vector<std::vector<MazeElement *>> grid, int cellSize);
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);

private slots:
    void replayStep();
};

#endif // ICP_PACMAN_REPLAYUI_H