#include <iostream>
#include <queue>
#include "path.h"

enum Direction AStar(int startRow, int startCol, int endRow, int endCol, const Maze &maze)
{
    // Create a priority queue for the open list
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;

    // Create a map for the closed list
    std::map<std::pair<int, int>, bool> closedList;

    // Create a map for the parent node of each node in the path
    std::map<std::pair<int, int>, std::pair<int, int>> parentMap;

    // Create the starting node
    Node startNode(startRow, startCol, 0, 0, nullptr);

    // Add the starting node to the open list
    openList.push(startNode);

    // Loop until the open list is empty or the target node is found
    while (!openList.empty())
    {
        // Get the node with the lowest f cost from the open list
        Node currentNode = openList.top();

        std::cout << "Current node: " << currentNode.row << ", " << currentNode.col << std::endl;

        // Remove the current node from the open list
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

            // If the path is not empty, return the next direction
            if (!path.empty())
            {
                int nextRow = path[0].first;
                int nextCol = path[0].second;

                if (nextRow == startRow - 1 && nextCol == startCol)
                {
                    return Direction::UP;
                }
                else if (nextRow == startRow + 1 && nextCol == startCol)
                {
                    return Direction::DOWN;
                }
                else if (nextRow == startRow && nextCol == startCol - 1)
                {
                    return Direction::LEFT;
                }
                else if (nextRow == startRow && nextCol == startCol + 1)
                {
                    return Direction::RIGHT;
                }
            }

            // If the path is empty, return direction::NONE
            return Direction::NONE;
        }

        // Generate the neighbors of the current node
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
            // If the neighbor is not in the closed list, calculate its f cost and add it to the open list
            if (!closedList[neighbor])
            {
                // Calculate the g cost of the neighbor
                int gCost = currentNode.gCost + 1;

                // Calculate the h cost of the neighbor using the Manhattan distance heuristic
                int hCost = abs(neighbor.first - endRow) + abs(neighbor.second - endCol);

                // Calculate
                int fCost = gCost + hCost;

                // Create the neighbor node
                Node neighborNode(neighbor.first, neighbor.second, gCost, fCost, &currentNode);

                // Add the neighbor node to the open list
                openList.push(neighborNode);

                // Add the neighbor node to the parent map
                parentMap[neighbor] = std::make_pair(currentNode.row, currentNode.col);
            }
        }
    }

    // If no path is found, return an empty vector
    return Direction::NONE;
}