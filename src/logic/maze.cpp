#include "maze.h"
#include <fstream>
#include <iostream>
#include "path.h"

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
    std::cout << "Rows: " << rows << ", cols: " << cols << std::endl;
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
                keys++;
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
                    grid[i][j] = new Ghost(pixmap, i, j);
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
                grid[i][j] = new Collectible();
                break;
            case 'E':
                grid[i][j] = new Empty();
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

int Maze::getRows() const
{
    return rows;
}

int Maze::getCols() const
{
    return cols;
}

int Maze::getKeys() const
{
    return keys;
}

MazeElement *Maze::getElementAt(int row, int col) const
{
    return grid[row][col];
}

void Maze::setElementAt(int row, int col, MazeElement *element)
{
    grid[row][col] = element;
}

bool Maze::isPositionValid(int row, int col, bool keyCollected) const
{
    // Check if the position is within the maze boundaries
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        return false;
    }

    // Check if the position is not occupied by a target when the key is not collected
    if (!keyCollected && dynamic_cast<Target *>(grid[row][col]))
    {
        return false;
    }
    // Check if the position is not occupied by a wall
    return !(dynamic_cast<Wall *>(grid[row][col]));
}

bool Maze::isGhostPositionValid(int row, int col) const
{
    // Check if the position is within the maze boundaries
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        return false;
    }

    // Check if the position is not occupied by a ghost
    if (dynamic_cast<Ghost *>(grid[row][col]))
    {
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
    return ' ';
}

char Collectible::getSymbol()
{
    return '.';
}

char Pacman::getSymbol()
{
    return 'S';
}

void Pacman::move(Direction &currentDirection, const Maze &maze, bool keyCollected)
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

    if (maze.isPositionValid(newRow, newCol, keyCollected))
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
        if (maze.isPositionValid(newRow, newCol, keyCollected))
        {
            row = newRow;
            col = newCol;
        }
    }
}

void Pacman::setPixmap(Direction newDirection)
{
    switch (newDirection)
    {
    case Direction::UP:
        currentSprite = QPixmap(":/images/data/pacman_up.png");
        break;
    case Direction::DOWN:
        currentSprite = QPixmap(":/images/data/pacman_down.png");
        break;
    case Direction::LEFT:
        currentSprite = QPixmap(":/images/data/pacman_left.png");
        break;
    case Direction::RIGHT:
        currentSprite = QPixmap(":/images/data/pacman_right.png");
        break;

    default:
        currentSprite = QPixmap(":/images/data/pacman_right.png");
        break;
    }
}

char Ghost::getSymbol()
{
    return 'G';
}

void Ghost::chase(Pacman &pacman, const Maze &maze)
{
    // Get the current ghost position
    int ghostRow = row;
    int ghostCol = col;

    // Get the current pacman position
    int pacmanRow = pacman.getRow();
    int pacmanCol = pacman.getCol();

    // // Find the shortest path from the ghost to the pacman using A*
    std::vector<std::pair<int, int>> path = AStar(ghostRow, ghostCol, pacmanRow, pacmanCol, maze);
    if (path.size() > 1)
    {
        // Get the next position in the path
        std::pair<int, int> nextPosition = path[0];
        int nextRow = nextPosition.first;
        int nextCol = nextPosition.second;

        // Move the ghost in the next direction
        if (maze.isGhostPositionValid(nextRow, nextCol))
        {
            row = nextRow;
            col = nextCol;
        }
    }
}

char Key::getSymbol()
{
    return 'K';
}

char Target::getSymbol()
{
    return 'T';
}
