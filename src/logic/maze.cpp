#include "maze.h"
#include <fstream>
#include <iostream>

Maze::Maze(std::string filename)
{
    loadFromFile(filename);
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

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char symbol;
            input >> symbol;

            switch (symbol)
            {
            case 'T':
                grid[i][j] = new Target();
                break;
            case 'K':
                grid[i][j] = new Key();
                break;
            case 'G':
                grid[i][j] = new Ghost();
                break;
            case 'S':
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
}

int Maze::getRows() const
{
    return rows;
}

int Maze::getCols() const
{
    return cols;
}

MazeElement *Maze::getElementAt(int row, int col)
{
    return grid[row][col];
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
