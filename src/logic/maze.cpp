#include "maze.h"
#include <fstream>
#include <iostream>

Maze::Maze(std::string filename)
{
    loadFromFile("../src/logic/" + filename);
}

void Maze::loadFromFile(const std::string &filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        std::cerr << "File " << filename << " not found!" << std::endl;
        return;
    }

    input >> rows >> cols;
    grid.resize(rows, std::vector<MazeElement *>(cols));

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
                grid[i][j] = new Target();
                break;
            case 'K':
                grid[i][j] = new Key();
                break;
            case 'G':
                if (numGhosts < 4)
                {
                    QColor ghostColor;
                    switch (numGhosts)
                    {
                    case 0:
                        ghostColor = Qt::red;
                        break;
                    case 1:
                        ghostColor = Qt::magenta;
                        break;
                    case 2:
                        ghostColor = Qt::cyan;
                        break;
                    case 3:
                        ghostColor = Qt::yellow;
                        break;
                    }
                    grid[i][j] = new Ghost(ghostColor);
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
                grid[i][j] = new Pacman(i, j); // Set the initial position
                break;
            case 'X':
                grid[i][j] = new Wall();
                break;
            case '.':
                grid[i][j] = new Empty();
                break;
            default:
                std::cerr << "Unknown symbol " << symbol << std::endl;
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

int Maze::getRows() const
{
    return rows;
}

int Maze::getCols() const
{
    return cols;
}

MazeElement *Maze::getElementAt(int row, int col) const
{
    return grid[row][col];
}

void Maze::setElementAt(int row, int col, MazeElement *element)
{
    grid[row][col] = element;
}

bool Maze::isPositionValid(int row, int col) const {
    // Check if the position is within the maze boundaries
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return false;
    }

    // Check if the position is not occupied by a wall
    return !(dynamic_cast<Wall *>(grid[row][col]));
}

char Wall::getSymbol()
{
    return 'X';
}

char Empty::getSymbol()
{
    return '.';
}

char Pacman::getSymbol()
{
    return 'S';
}

void Pacman::move(Direction &currentDirection, const Maze& maze)
{
    int newRow = row;
    int newCol = col;

    switch (currentDirection)
    {
    case Direction::UP:
        newRow--;
        break;
    case Direction::DOWN:
        newRow++;
        break;
    case Direction::LEFT:
        newCol--;
        break;
    case Direction::RIGHT:
        newCol++;
        break;
    }

    if (maze.isPositionValid(newRow, newCol))
    {
        row = newRow;
        col = newCol;
    }
    else
    {
        // If the current direction is blocked, try to continue moving in the previous direction
        switch (this->currentDirection)
        {
        case Direction::UP:
            newRow = row - 1;
            newCol = col;
            break;
        case Direction::DOWN:
            newRow = row + 1;
            newCol = col;
            break;
        case Direction::LEFT:
            newRow = row;
            newCol = col - 1;
            break;
        case Direction::RIGHT:
            newRow = row;
            newCol = col + 1;
            break;
        }

        // If the previous direction is not blocked, keep moving in that direction
        if (maze.isPositionValid(newRow, newCol))
        {
            row = newRow;
            col = newCol;
        }
    }
}

char Ghost::getSymbol()
{
    return 'G';
}

char Key::getSymbol()
{
    return 'K';
}

char Target::getSymbol()
{
    return 'T';
}
