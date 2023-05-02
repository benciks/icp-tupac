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
    RIGHT
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

class Pacman : public MazeElement
{
public:
    Pacman(int row, int col) : row(row), col(col), currentDirection(Direction::RIGHT) {} // Initialize currentDirection
    char getSymbol() override;
    void move(Direction &currentDirection, const Maze &maze);

    // Getter functions
    int getRow() const { return row; }
    int getCol() const { return col; }
    QPixmap getPixmap() const { return QPixmap(":/images/data/pacman.png"); }
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

private:
    int row;
    int col;
    Direction currentDirection; // Add currentDirection attribute
};

class Ghost : public MazeElement
{
public:
    Ghost(const QPixmap &pixmap) : pixmap(pixmap) {}
    char getSymbol() override;
    const QPixmap &getPixmap() const { return pixmap; }

private:
    QPixmap pixmap;
};

class Key : public MazeElement
{
public:
    char getSymbol() override;
};

class Target : public MazeElement
{
public:
    char getSymbol() override;
};

class Maze
{
public:
    Maze(std::string filename);
    int getRows() const;
    int getCols() const;
    MazeElement *getElementAt(int row, int col) const;
    void setElementAt(int row, int col, MazeElement *element);
    bool isPositionValid(int row, int col) const;

private:
    int rows;
    int cols;
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<MazeElement *>> grid;
};
#endif // ICP_PACMAN_MAZE_H
