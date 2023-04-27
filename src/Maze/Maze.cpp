// Maze.cpp

#include "Maze.h"
#include <fstream>

Maze::Maze(std::string filename) {
    loadFromFile(filename);
}

void Maze::loadFromFile(const std::string& filename) {
    /*
        Příklad (bludiště je vždy ohraničeno zdí, neuvádí se v uložené mapě)
        10 10
        .........T
        XX...XX...
        X.........
        XXXXXXX...
        ....XG....
        XX..XGX...
        ....XXX...
        XX...XX.K.
        S.........
        XX...XX...
    */
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // TODO: redo?
    inputFile >> numRows >> numCols;

    grid.resize(numRows, std::vector<MazeElement*>(numCols));

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            char symbol;
            inputFile >> symbol;

            switch (symbol) {
                case 'T':
                    grid[i][j] = new Target();
                    break;
                case 'X':
                    grid[i][j] = new Wall();
                    break;
                case 'G':
                    grid[i][j] = new Ghost();
                    break;
                case 'K':
                    grid[i][j] = new Key();
                    break;
                case 'S':
                    grid[i][j] = new Start();
                    pacMan = new PacMan(j, i);
                    break;
                case '.':
                    grid[i][j] = new Empty();
                    break;
                default:
                    std::cerr << "Error: Invalid symbol: " << symbol << " in file " << filename << std::endl;
                    exit(1);
            }
        }
    }

    inputFile.close();
}

bool Maze::isValidMove(int x, int y) const {
    if (x < 0 || x >= numCols || y < 0 || y >= numRows) {
        return false;
    }
    return dynamic_cast<Wall*>(grid[y][x]) == nullptr;
}

MazeElement* Maze::getElementAt(int x, int y) const {
    if (x >= 0 && x < numCols && y >= 0 && y < numRows) {
        return grid[y][x];
    }
    return nullptr;
}

void Maze::setElementAt(int x, int y, MazeElement* element) {
    if (x >= 0 && x < numCols && y >= 0 && y < numRows) {
        grid[y][x] = element;
    }
}

PacMan* Maze::getPacMan() const {
    return pacMan;
}
