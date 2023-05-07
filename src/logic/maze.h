/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file maze.h
 * @brief Header file for the maze.
 */

#ifndef ICP_PACMAN_MAZE_H
#define ICP_PACMAN_MAZE_H

#include <iostream>
#include <vector>
#include <QColor>
#include <QPixmap>

class Maze;

/**
 * @enum Direction
 * @brief Enumeration representing possible directions for pacman, used to change pacman sprite.
 */
enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @class MazeElement
 * @brief Abstract base class for all elements in the maze.
 */
class MazeElement
{
public:
    /**
     * @brief Virtual destructor for the MazeElement class.
     */
    virtual ~MazeElement() {}

    /**
     * @brief Virtual function that returns the symbol representing the element.
     * @return The character symbol.
     */
    virtual char getSymbol() = 0;
};

/**
 * @class Wall
 * @brief Represents a wall in the maze.
 */
class Wall : public MazeElement
{
public:
    /**
     * @brief Destructor for the Wall class.
     */
    ~Wall() {}

    /**
     * @brief Returns the symbol representing a wall.
     * @return The character symbol.
     */
    char getSymbol() override;
};

/**
 * @class Empty
 * @brief Represents an empty space in the maze.
 */
class Empty : public MazeElement
{
public:
    /**
     * @brief Destructor for the Empty class.
     */
    ~Empty() {}

    /**
     * @brief Returns the symbol representing an empty space.
     * @return The character symbol.
     */
    char getSymbol() override;
};

/**
 * @class Collectible
 * @brief Represents a collectible item in the maze.
 */
class Collectible : public MazeElement
{
public:
    /**
     * @brief Destructor for the Collectible class.
     */
    ~Collectible() {}

    /**
     * @brief Returns the symbol representing a collectible.
     * @return The character symbol.
     */
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/fruit.png"); }
};

/**
 * @class Key
 * @brief Represents a key in the maze.
 */
class Key : public MazeElement
{
public:
    /**
     * @brief Destructor for the Key class.
     */
    ~Key() {}

    /**
     * @brief Returns the symbol representing a key in the maze.
     * @return The character symbol.
     */
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/key.png"); }
};

/**
 * @class Target
 * @brief Represents the target/exit in the maze.
 */
class Target : public MazeElement
{
public:
    /**
     * @brief Destructor for the Target class.
     */
    ~Target() {}
    /**
     * @brief Returns the symbol representing a target.
     * @return The character symbol.
     */
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/door.png"); }
};

/**
 * @class Pacman
 * @brief Represents the Pacman character in the maze.
 */
class Pacman : public MazeElement
{
public:
    /**
     * @brief Constructor for the Pacman class.
     * @param row Row position in maze.
     * @param col Column position in maze.
     * @param currentDirection Current direction of pacman.
     * @param currentSprite Current applied sprite to pacman.
     */
    Pacman(int row, int col) : row(row), col(col), currentDirection(Direction::RIGHT), currentSprite(QPixmap(":/images/data/pacman_right.png")) {} // Initialize currentDirection

    /**
     * @brief Destructor for the Pacman class.
     */
    ~Pacman() {}

    /**
     * @brief Returns the symbol representing the Pacman.
     * @return The character symbol.
     */
    char getSymbol() override;

    /**
     * @brief Moves the Pacman character.
     * @param currentDirection The current direction of Pacman.
     * @param maze The maze.
     * @param keyCollected A boolean indicating if a key was collected - used for target unlock.
     */
    void move(Direction &currentDirection, const Maze &maze, bool keyCollected);

    /**
     * @brief Gets the row position of Pacman.
     * @return The row position.
     */
    int getRow() const { return row; }

    /**
     * @brief Gets the column position of Pacman.
     * @return The column position.
     */
    int getCol() const { return col; }

    /**
     * @brief Gets the QPixmap of Pacman (sprite).
     * @return The QPixmap.
     */
    QPixmap getPixmap() const { return currentSprite; }

    /**
     * @brief Gets the current direction of Pacman.
     * @return The current direction.
     */
    Direction getCurrentDirection() const { return currentDirection; };

    /**
     * @brief Sets the row position of Pacman.
     * @param newRow The new row position.
     */
    void setRow(int newRow)
    {
        row = newRow;
    }

    /**
     * @brief Sets the column position of Pacman.
     * @param newCol The new column position.
     */
    void setCol(int newCol)
    {
        col = newCol;
    }

    /**
     * @brief Sets the current direction of Pacman.
     * @param newDirection The new direction.
     */
    void setCurrentDirection(Direction newDirection) { currentDirection = newDirection; }

    /**
     * @brief Sets the QPixmap of Pacman (sprite) according to the new direction.
     * @param newDirection The new direction.
     */
    void setPixmap(Direction newDirection);

private:
    int row;
    int col;
    Direction currentDirection;
    QPixmap currentSprite;
};

/**
 * @class Ghost
 * @brief Represents a ghost in the maze.
 */
class Ghost : public MazeElement
{
public:
    /**
     * @brief Constructor for the Ghost class.
     * @param pixmap The sprite representing the ghost.
     * @param row Row of the ghost's position.
     * @param col Column of the ghost's position.
     */
    Ghost(const QPixmap &pixmap, int row, int col) : pixmap(pixmap), row(row), col(col) {}
    ~Ghost() {}
    char getSymbol() override;

    /**
     * @brief Get the ghost's sprite.
     * @return The QPixmap representing the ghost.
     */
    const QPixmap &getPixmap() const { return pixmap; }

    /**
     * @brief Makes the ghost chase Pac-Man.
     * @param pacman The Pac-Man object to chase.
     * @param maze The maze object where the game is being played.
     */
    void chase(Pacman &pacman, const Maze &maze);

    /**
     * @brief Get the ghost's row position.
     * @return The row position of the ghost.
     */
    int getRow() const { return row; }

    /**
     * @brief Get the ghost's column position.
     * @return The column position of the ghost.
     */
    int getCol() const { return col; }

    /**
     * @brief Set the ghost's row position.
     * @param newRow The new row position.
     */
    void setRow(int newRow) { row = newRow; }

    /**
     * @brief Set the ghost's column position.
     * @param newCol The new column position.
     */
    void setCol(int newCol) { col = newCol; }

    /**
     * @brief Get the current MazeElement the ghost is on.
     * @return The current MazeElement.
     */
    MazeElement *getCurrent() const { return current; }

    /**
     * @brief Set the current MazeElement the ghost is on.
     * @param newCurrent The new MazeElement.
     */
    void setCurrent(MazeElement *newCurrent) { current = newCurrent; }

private:
    QPixmap pixmap;
    int row;
    int col;
    MazeElement *current = nullptr;
};

/**
 * @class Maze
 * @brief Represents a maze for a Pacman game.
 */
class Maze
{
public:
    /**
     * @brief Constructor that initializes the maze from a file.
     * @param filename The name of the file containing the maze.
     */
    Maze(std::string filename);

    /**
     * @brief Destructor that cleans up the allocated memory.
     */
    ~Maze();

    /**
     * @brief Returns the number of rows in the maze.
     * @return The number of rows.
     */
    int getRows() const;

    /**
     * @brief Returns the number of columns in the maze.
     * @return The number of cols.
     */
    int getCols() const;

    /**
     * @brief Returns the number of keys in the maze.
     * @return The number of keys.
     */
    int getKeys() const;

    /**
     * @brief Returns the MazeElement at the (row, col) position in maze.
     * @param row The row index.
     * @param col The column index.
     * @return A pointer to the MazeElement.
     */
    MazeElement *getElementAt(int row, int col) const;

    /**
     * @brief Sets the MazeElement at the (row, col) position.
     * @param row The row index.
     * @param col The column index.
     * @param element A pointer to the MazeElement.
     */
    void setElementAt(int row, int col, MazeElement *element);

    /**
     * @brief Checks if the (row, col) position is valid for Pacman.
     * @param row The row index.
     * @param col The column index.
     * @param keyCollected Bool that checks if key was collected.
     * @return True if the position is valid, false otherwise.
     */
    bool isPositionValid(int row, int col, bool keyCollected) const;

    /**
     * @brief Checks if the specified position is valid for a Ghost.
     * @param row The row index.
     * @param col The column index.
     * @return True if the position is valid, false otherwise.
     */
    bool isGhostPositionValid(int row, int col) const;

private:
    int rows;
    int cols;
    int keys = 0;
    int ghosts = 0;

    /**
     * @brief Loads the maze from a file.
     * @param filename The name of the file containing the maze.
     */
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<MazeElement *>> grid;
};

#endif // ICP_PACMAN_MAZE_H