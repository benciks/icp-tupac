/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file path.cpp
 * @brief Implementation file for the pathfinding algorithm.
 */

#include <iostream>
#include <queue>
#include "path.h"

std::vector<std::pair<int, int>> AStar(int startRow, int startCol, int endRow, int endCol, const Maze &maze)
{
    // Create a priority queue for the open list
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::map<std::pair<int, int>, bool> closedList;
    std::map<std::pair<int, int>, std::pair<int, int>> parentMap;

    // Create the starting node
    Node startNode(startRow, startCol, 0, 0, nullptr);
    openList.push(startNode);

    while (!openList.empty())
    {
        // Get the node with the lowest f cost
        Node currentNode = openList.top();
        openList.pop();

        // Mark the current node as visited in the closed list
        closedList[std::make_pair(currentNode.row, currentNode.col)] = true;

        // If the target node is found, construct the path and return it
        if (currentNode.row == endRow && currentNode.col == endCol)
        {
            std::vector<std::pair<int, int>> path;

            // Follow the parent nodes to construct the path
            while (parentMap.find(std::make_pair(currentNode.row, currentNode.col)) != parentMap.end())
            {
                path.push_back(std::make_pair(currentNode.row, currentNode.col));
                currentNode = Node(parentMap[std::make_pair(currentNode.row, currentNode.col)].first, parentMap[std::make_pair(currentNode.row, currentNode.col)].second, 0, 0, nullptr);
            }

            // Reverse the path and return it
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Generate the neighbors
        std::vector<std::pair<int, int>> neighbors;
        if (maze.isGhostPositionValid(currentNode.row - 1, currentNode.col))
        {
            neighbors.push_back(std::make_pair(currentNode.row - 1, currentNode.col));
        }
        if (maze.isGhostPositionValid(currentNode.row + 1, currentNode.col))
        {
            neighbors.push_back(std::make_pair(currentNode.row + 1, currentNode.col));
        }
        if (maze.isGhostPositionValid(currentNode.row, currentNode.col - 1))
        {
            neighbors.push_back(std::make_pair(currentNode.row, currentNode.col - 1));
        }
        if (maze.isGhostPositionValid(currentNode.row, currentNode.col + 1))
        {
            neighbors.push_back(std::make_pair(currentNode.row, currentNode.col + 1));
        }

        // Loop through the neighbors of the current node
        for (std::pair<int, int> neighbor : neighbors)
        {
            if (!closedList[neighbor])
            {
                int gCost = currentNode.gCost + 1;
                int hCost = abs(neighbor.first - endRow) + abs(neighbor.second - endCol);
                int fCost = gCost + hCost;

                // Create the neighbor node
                Node neighborNode(neighbor.first, neighbor.second, gCost, fCost, &currentNode);
                openList.push(neighborNode);

                // Add the neighbor node to the parent map
                parentMap[neighbor] = std::make_pair(currentNode.row, currentNode.col);
            }
        }
    }

    // If no path is found, return an empty vector
    return std::vector<std::pair<int, int>>();
}