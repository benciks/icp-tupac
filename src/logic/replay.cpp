#include "replay.h"
#include <fstream>
#include <iostream>
#include <sstream>

Replay::Replay(std::string filename, bool start)
    : start(start) // Initialize the start member variable
{
    parseFile(filename);
    if (!start)
    {
        currentStep = steps.size() - 1;
        maxStep = 0;
    }
    else
    {
        currentStep = 0;
        maxStep = steps.size() - 1;
    }

    parseStep(steps[currentStep]);
}

Replay::~Replay()
{
    for (auto &row : currentGrid)
    {
        for (auto &element : row)
        {
            delete element;
            element = nullptr;
        }
    }

    for (auto &grid : prevGrids)
    {
        for (auto &row : grid)
        {
            for (auto &element : row)
            {
                delete element;
                element = nullptr;
            }
        }
    }
}

// Get next step
void Replay::nextMove()
{
    if (this->start) // Normal mode
    {
        if (currentStep < maxStep)
        {
            currentStep++;
            parseStep(steps[currentStep]);
        }
    }
    else // Reverse mode
    {
        if (currentStep > 0)
        {
            --currentStep; // Decrement the step in reverse mode
            currentGrid = prevGrids[currentStep];
            prevGrids.pop_back(); 
        }
    }
}

// Get previous step
void Replay::prevMove()
{
    if (this->start) // Normal mode
    {
        if (currentStep > 0)
        {
            currentStep--;
            currentGrid = prevGrids[currentStep];
            prevGrids.pop_back();   
        }
    }
    else // Reverse mode
    {
        if (currentStep < maxStep)
        {
            currentStep++; // Increment the step in reverse mode
            parseStep(steps[currentStep]);
        }
    }
}



// Parse into vector of strings
void Replay::parseFile(const std::string &filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        std::cerr << "File " << filename << " not found!" << std::endl;
        return;
    }

    std::string line;
    std::string block = "";

    while (std::getline(input, line))
    {
        if (line == "---")
        {
            steps.push_back(block);
            block = "";
        }
        else
        {
            block += line + "\n";
        }
    }
}

// Parse into grid
void Replay::parseStep(const std::string &step)
{

    if (currentStep != 0) 
    {
        prevGrids.push_back(currentGrid);
    }

    std::istringstream input(step);

    input >> cols >> rows;
    std::cout << "Rows: " << rows << ", cols: " << cols << std::endl;
    currentGrid.resize(rows, std::vector<MazeElement *>(cols));

    int numGhosts = 0;
    bool hasStart = false;
    bool hasTarget = false;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char symbol;
            input >> symbol;

            switch (symbol)
            {
            case 'T':
                if (!hasTarget)
                {
                    hasTarget = true;
                }
                else
                {
                    std::cerr << "Multiple target positions found!" << std::endl;
                    exit(1);
                }
                currentGrid[i][j] = new Target();
                break;
            case 'K':
                currentGrid[i][j] = new Key();
                break;
            case 'G':
                if (numGhosts < 4)
                {
                    QPixmap pixmap;
                    switch (numGhosts)
                    {
                    case 0:
                        pixmap = QPixmap(":/images/data/blinky_right.png");
                        break;
                    case 1:
                        pixmap = QPixmap(":/images/data/pinky_right.png");
                        break;
                    case 2:
                        pixmap = QPixmap(":/images/data/inky_right.png");
                        break;
                    case 3:
                        pixmap = QPixmap(":/images/data/clyde_right.png");
                        break;
                    }
                    currentGrid[i][j] = new Ghost(pixmap, i, j);
                    numGhosts++;
                }
                else
                {
                    std::cerr << "Too many ghosts!" << std::endl;
                    exit(1);
                }
                break;
            case 'S':
                if (!hasStart)
                {
                    hasStart = true;
                }
                else
                {
                    std::cerr << "Multiple starting positions found!" << std::endl;
                    exit(1);
                }
                currentGrid[i][j] = new Pacman(i, j); // Set the initial position
                break;
            case 'X':
                currentGrid[i][j] = new Wall();
                break;
            case '.':
                currentGrid[i][j] = new Collectible();
                break;
            case 'E':
                currentGrid[i][j] = new Empty();
                break;
            default:
                std::cerr << "Unknown maze symbol " << symbol << std::endl;
                exit(1);
            }
        }
    }

    if (!hasStart)
    {
        std::cerr << "No starting position found!" << std::endl;
        exit(1);
    }

    if (!hasTarget)
    {
        std::cerr << "No target position found!" << std::endl;
        exit(1);
    }

    if (numGhosts == 0)
    {
        std::cerr << "No ghosts found!" << std::endl;
        exit(1);
    }
}
