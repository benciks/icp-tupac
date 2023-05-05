#ifndef ICP_PACMAN_PATH_H
#define ICP_PACMAN_PATH_H

#include "maze.h"
#include <vector>
#include <queue>
#include <iostream>

class Node
{
public:
    int row;
    int col;
    int gCost;
    int fCost;
    Node *parent;

    Node(int r, int c, int g, int f, Node *p)
        : row(r), col(c), gCost(g), fCost(f), parent(p) {}

    // Define a comparison operator for sorting nodes in the priority queue
    bool operator>(const Node &other) const
    {
        return fCost > other.fCost;
    }
};

std::vector<std::pair<int, int>> AStar(int startRow, int startCol, int endRow, int endCol, const Maze &maze);
#endif
