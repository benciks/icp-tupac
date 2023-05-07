/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file replay.h
 * @brief Header file for the replay logic.
 */

#ifndef ICP_PACMAN_REPLAY_H
#define ICP_PACMAN_REPLAY_H
#include <iostream>
#include "maze.h"
#include <vector>
#include <deque>

/**
 * @class Replay
 * @brief Provides functionality to replay a saved game.
 */
class Replay
{
public:
    /**
     * @brief Constructor for the Replay class.
     * @param filename The name of the file containing the saved game.
     * @param start A boolean indicating whether to start at the beginning or end of the saved game.
     */
    Replay(std::string filename, bool start = true);

    /**
     * @brief Destructor for the Replay class.
     */
    ~Replay();

    /**
     * @brief Moves to the next step in the saved game.
     */
    void nextMove();

    /**
     * @brief Moves to the previous step in the saved game.
     */
    void prevMove();

    /**
     * @brief Returns the current grid of MazeElements.
     * @return The grid.
     */
    std::vector<std::vector<MazeElement *>> getGrid() const
    {
        return currentGrid;
    }

    /**
     * @brief Gets the number of rows in the grid.
     * @return The number of rows.
     */
    int getRows()
    {
        return rows;
    };

    /**
     * @brief Gets the number of columns in the grid.
     * @return The number of columns.
     */
    int getCols()
    {
        return cols;
    };

private:
    int rows;
    int cols;
    bool start;
    size_t currentStep;
    size_t maxStep;
    std::vector<std::string> steps;
    void parseFile(const std::string &filename);
    void parseStep(const std::string &step);
    std::vector<std::vector<MazeElement *>> currentGrid;
};
#endif // ICP_PACMAN_REPLAY_H