#ifndef ICP_PACMAN_MAZE_H
#define ICP_PACMAN_MAZE_H

#include <iostream>
#include <vector>
#include <QColor>
#include <QPixmap>

class Maze;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class MazeElement
{
public:
    virtual char getSymbol() = 0;
};

class Wall : public MazeElement
{
public:
    char getSymbol() override;
};

class Empty : public MazeElement
{
public:
    char getSymbol() override;
};

class Collectible : public MazeElement
{
public:
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/fruit.png"); }
};

class Pacman : public MazeElement
{
public:
    Pacman(int row, int col) : row(row), col(col), currentDirection(Direction::RIGHT), currentSprite(QPixmap(":/images/data/pacman_right.png")) {} // Initialize currentDirection
    char getSymbol() override;
    void move(Direction &currentDirection, const Maze &maze, bool keyCollected);

    // Getter functions
    int getRow() const { return row; }
    int getCol() const { return col; }
    QPixmap getPixmap() const { return currentSprite; }
    Direction getCurrentDirection() const { return currentDirection; }

    // Setter functions
    void setRow(int newRow)
    {
        row = newRow;
    }
    void setCol(int newCol)
    {
        col = newCol;
    }
    void setCurrentDirection(Direction newDirection) { currentDirection = newDirection; }
    void setPixmap(Direction newDirection);

private:
    int row;
    int col;
    Direction currentDirection; // Add currentDirection attribute
    QPixmap currentSprite;
};

class Ghost : public MazeElement
{
public:
    Ghost(const QPixmap &pixmap, int row, int col) : pixmap(pixmap), row(row), col(col) {}
    char getSymbol() override;
    const QPixmap &getPixmap() const { return pixmap; }
    void chase(Pacman &pacman, const Maze &maze);
    int getRow() const { return row; }
    int getCol() const { return col; }
    // Setter functions
    void setRow(int newRow)
    {
        row = newRow;
    }
    void setCol(int newCol)
    {
        col = newCol;
    }

    MazeElement *getCurrent() const { return current; }
    void setCurrent(MazeElement *newCurrent) { current = newCurrent; }

private:
    QPixmap pixmap;
    int row;
    int col;
    MazeElement *current;
};

class Key : public MazeElement
{
public:
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/key.png"); }
};

class Target : public MazeElement
{
public:
    char getSymbol() override;
    QPixmap getPixmap() const { return QPixmap(":/images/data/door.png"); }
};

class Maze
{
public:
    Maze(std::string filename);
    int getRows() const;
    int getCols() const;
    int getKeys() const;
    MazeElement *getElementAt(int row, int col) const;
    void setElementAt(int row, int col, MazeElement *element);
    bool isPositionValid(int row, int col, bool keyCollected) const;
    bool isGhostPositionValid(int row, int col) const;

private:
    int rows;
    int cols;
    int keys = 0;
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<MazeElement *>> grid;
};
#endif // ICP_PACMAN_MAZE_H
