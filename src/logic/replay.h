#ifndef ICP_PACMAN_REPLAY_H
#define ICP_PACMAN_REPLAY_H
#include <iostream>
#include "maze.h"
#include <vector>

class Replay
{
public:
    Replay(std::string filename, bool start = true);
    ~Replay(); 
    void nextMove();
    void prevMove();
    std::vector<std::vector<MazeElement *>> getGrid() { return currentGrid; }
    int getRows() { return rows; }
    int getCols() { return cols; }

private:
    int rows;
    int cols;
    int maxStep;
    bool start;
    int currentStep = 0;
    std::vector<std::string> steps;
    void parseFile(const std::string &filename);
    void parseStep(const std::string &step);
    std::vector<std::vector<MazeElement *>> currentGrid;
    std::vector<std::vector<std::vector<MazeElement *>>> prevGrids;
};
#endif // ICP_PACMAN_REPLAY_H