/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file path.h
 * @brief Header file for the pathfinding algorithm.
 */

#ifndef ICP_PACMAN_PATH_H
#define ICP_PACMAN_PATH_H

#include "maze.h"
#include <vector>
#include <queue>
#include <iostream>

/**
 * @class Node
 * @brief Represents a node in the A* algorithm.
 */
class Node
{
public:
    int row;      /**< Row of the node */
    int col;      /**< Column of the node */
    int gCost;    /**< Cost of the path from the start node to this node */
    int fCost;    /**< Cost of the path from the start node to this node + estimated cost to the end node */
    Node *parent; /**< Pointer to the parent node */

    /**
     * @brief Constructor that initializes the node.
     * @param r Row of the node.
     * @param c Column of the node.
     * @param g Cost of the path from the start node to this node.
     * @param f Cost of the path from the start node to this node + estimated cost to the end node.
     * @param p Pointer to the parent node.
     */
    Node(int r, int c, int g, int f, Node *p)
        : row(r), col(c), gCost(g), fCost(f), parent(p) {}

    // Define a comparison operator for sorting nodes in the priority queue
    /**
     * @brief Comparison operator for sorting nodes in the priority queue.
     * @param other The other node to compare to.
     */
    bool operator>(const Node &other) const
    {
        return fCost > other.fCost;
    }
};

/**
 * @brief Returns the path from the start node to the end node.
 * @param startRow Row of the start node.
 * @param startCol Column of the start node.
 * @param endRow Row of the end node.
 * @param endCol Column of the end node.
 * @param maze The maze.
 * @return A vector of pairs of integers representing the path.
 */
std::vector<std::pair<int, int>> AStar(int startRow, int startCol, int endRow, int endCol, const Maze &maze);
#endif
