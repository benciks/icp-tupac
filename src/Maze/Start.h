#pragma once
#include "MazeElement.h"

class Start : public MazeElement {
public:
    char getSymbol() const override { return 'S'; }
};