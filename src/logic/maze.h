#ifndef ICP_PACMAN_MAZE_H
#define ICP_PACMAN_MAZE_H

#include <iostream>
#include <vector>

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
    MazeElement *getElementAt(int row, int col);

private:
    int rows;
    int cols;
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<MazeElement *>> grid;
};
#endif // ICP_PACMAN_MAZE_H
