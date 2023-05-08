/**
 * @author Šimon Benčík <xbenci01>
 * @author Roman Poliačik <xpolia05>
 * @file replay.cpp
 * @brief Implementation file for the replay.
 */

#include "replay.h"
#include <fstream>
#include <iostream>
#include <sstream>

Replay::Replay(std::string filename, bool start)
{
    parseFile(filename);
    if (!start)
    {
        currentStep = steps.size() - 1;
    }
    else
    {
        currentStep = 0;
    }

    maxStep = steps.size() - 1;
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
}

// Get next step
void Replay::nextMove()
{
    if (currentStep < maxStep)
    {
        currentStep++;
        parseStep(steps[currentStep]);
    }
}

// Get previous step
void Replay::prevMove()
{
    if (currentStep > 0)
    {
        --currentStep;
        parseStep(steps[currentStep]);
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
    // Free the previous grid
    for (auto &row : currentGrid)
    {
        for (auto &element : row)
        {
            delete element;
            element = nullptr;
        }
    }

    std::istringstream input(step);

    input >> cols >> rows;
    currentGrid.resize(rows, std::vector<MazeElement *>(cols));

    int numGhosts = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char symbol;
            input >> symbol;

            switch (symbol)
            {
            case 'T':
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
                        pixmap = QPixmap(":/images/data/blinky.png");
                        break;
                    case 1:
                        pixmap = QPixmap(":/images/data/pinky.png");
                        break;
                    case 2:
                        pixmap = QPixmap(":/images/data/inky.png");
                        break;
                    case 3:
                        pixmap = QPixmap(":/images/data/clyde.png");
                        break;
                    }
                    currentGrid[i][j] = new Ghost(pixmap, i, j);
                    numGhosts++;
                }
                break;
            case 'S':
                currentGrid[i][j] = new Pacman(i, j);
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
                break;
            }
        }
    }
}
