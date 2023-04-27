#pragma once
#include "MazeElement.h"

class Wall : public MazeElement{
public:
    char getSymbol() const override { return 'X'; }
};