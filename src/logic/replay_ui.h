/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file replay_ui.h
 * @brief Header file for the replay user interface.
 */

#ifndef ICP_PACMAN_REPLAYUI_H
#define ICP_PACMAN_REPLAYUI_H

#include <QWidget>
#include "replay.h"
#include <QObject>
#include <QTimer>

/**
 * @class ReplayUI
 * @brief Provides a user interface for replaying a saved game.
 */
class ReplayUI : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for the ReplayUI class.
     * @param parent The parent widget.
     * @param fileName The name of the file containing the saved game.
     * @param start A boolean indicating whether to start at the beginning or end of the saved game.
     */
    ReplayUI(QWidget *parent = nullptr, QString fileName = "", bool start = true);

    /**
     * @brief Destructor for the ReplayUI class.
     */
    ~ReplayUI();

    /**
     * @brief Paints the maze onto the QWidget.
     * @param painter The QPainter to use for painting the maze.
     */
    void paintMaze(QPainter &painter);

    /**
     * @brief Switches the replay mode between sequential and automatic.
     */
    void switchMode();

private:
    Replay *replay;
    QTimer *replayTimer;
    bool start;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    /**
     * @brief Advances the replay by one step.
     */
    void replayStep();

signals:
    /**
     * @brief Emitted when the replay has finished.
     */
    void replayFinished();

private:
    bool sequentialMode;
};

#endif // ICP_PACMAN_REPLAYUI_H