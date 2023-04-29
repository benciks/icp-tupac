#ifndef ICP_PACMAN_MAZE_H
#define ICP_PACMAN_MAZE_H

#include <iostream>
#include <vector>

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
    char getSymbol() override;
    void move(Direction direction, const Maze& maze);

    int getRow() const { return row; }
    int getCol() const { return col; }
    void setPosition(int newRow, int newCol) { row = newRow; col = newCol; }

private:
    int row;
    int col;
};

class Ghost : public MazeElement
{
public:
    char getSymbol() override;
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

private:
    int rows;
    int cols;
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<MazeElement *>> grid;
};
#endif // ICP_PACMAN_MAZE_H
