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
                    numGhosts++;
                }
                else
                {
                    std::cerr << "Too many ghosts!" << std::endl;
                    exit(1);
                }
                grid[i][j] = new Ghost();
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
                grid[i][j] = new Pacman();
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

void Pacman::move(Direction direction, const Maze& maze)
{
    int newRow = row;
    int newCol = col;

    switch (direction)
    {
    case Direction::UP:
        newRow -= 1;
        break;
    case Direction::DOWN:
        newRow += 1;
        break;
    case Direction::LEFT:
        newCol -= 1;
        break;
    case Direction::RIGHT:
        newCol += 1;
        break;
    }

    // Check if the new position is inside the maze and not a wall
    if (newRow >= 0 && newRow < maze.getRows() && newCol >= 0 && newCol < maze.getCols() && maze.getElementAt(newRow, newCol)->getSymbol() != 'X')
    {
        row = newRow;
        col = newCol;
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
