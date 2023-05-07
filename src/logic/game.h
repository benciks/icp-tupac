/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file game.h
 * @brief Header file for the main game logic.
 */

#ifndef ICP_PACMAN_GAME_H
#define ICP_PACMAN_GAME_H

#include <QWidget>
#include "maze.h"
#include <QObject>
#include <QTimer>

/**
 * @class Game
 * @brief The Game class is the main class for the Pac-Man game.
 *
 * It creates a QWidget to display the game and initializes the maze and Pac-Man.
 * It also handles the game loop, including moving Pac-Man and ghosts, and updating the display.
 */
class Game : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a new Game object.
     * @param parent The parent widget.
     * @param fileName The name of the file to load the maze from. If empty, loads the default maze.
     */
    Game(QWidget *parent = nullptr, QString fileName = "");
    /**
     * @brief Destructs the Game object.
     */
    ~Game();
    /**
     * @brief Paints the maze.
     * @param painter The QPainter object to use for painting.
     */
    void paintMaze(QPainter &painter);
    /**
     * @brief Sets the pacman direction.
     * @param direction The direction to set.
     */
    void rotatePacman(Direction direction);
    /**
     * @brief Returns ghost count.
     */
    int getGhostCount() { return ghosts; };

private:
    Maze *maze;              /**< The maze object. */
    QTimer *moveTimer;       /**< The timer for moving Pac-Man. */
    QTimer *ghostTimer;      /**< The timer for moving the ghosts. */
    QPainter *painter;       /**< The QPainter object for painting the maze. */
    QString replayFileName;  /**< The name of the replay file. */
    bool exitOpened = false; /**< Whether the exit has been opened. */
    int keysCollected = 0;   /**< The number of keys collected. */
    int score = 0;           /**< The current score. */
    int steps = 0;           /**< The number of steps taken. */
    int ghosts = 0;          /**< The number of ghosts. */

protected:
    /**
     * @brief Paint event handler.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;
    /**
     * @brief Paints the element.
     * @param painter The QPainter object to use for painting.
     * @param element The element to paint.
     * @param x The x coordinate of the element.
     * @param y The y coordinate of the element.
     * @param cellSize The size of the cell.
     */
    void paintElement(QPainter &painter, MazeElement *element, int x, int y, int cellSize);
    /**
     * @brief Key press event handler.
     * @param event The key press event.
     */
    void keyPressEvent(QKeyEvent *event) override;
    /**
     * @brief Saves the game to replay file.
     */
    void saveGame();
    /**
     * @brief Ends the current game.
     */
    void endGame(bool victory = false);

private slots:
    /**
     * @brief Handler function for pacman move timer
     */
    void movePacman();
    /**
     * @brief Handler function for ghost move timer
     */
    void moveGhosts();

signals:
    /**
     * @brief Signal for score change.
     * @param newScore The new score.
     */
    void scoreChanged(int newScore);

    /**
     * @brief Signal for score change.
     * @param newKeyCount The new score.
     */
    void keysChanged(int newKeyCount);

    /**
     * @brief Signal for game over.
     * @param victory Whether the player won.
     */
    void gameOver(bool victory);
};

#endif // ICP_PACMAN_GAME_H