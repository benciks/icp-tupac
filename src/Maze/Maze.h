// Maze.h
#pragma once

#include <vector>
#include <iostream>

#include "MazeElement.h"
#include "PacMan.h"

class Maze {
public:
    Maze(std::string filename);

    void loadFromFile(const std::string& filename);
    bool isValidMove(int x, int y) const;
    MazeElement* getElementAt(int x, int y) const;
    void setElementAt(int x, int y, MazeElement* element);
    PacMan* getPacMan() const;
    int getRows() const;
    int getCols() const;

private:
    int numRows;
    int numCols;
    std::vector<std::vector<MazeElement*>> grid;
    PacMan* pacMan;
};
