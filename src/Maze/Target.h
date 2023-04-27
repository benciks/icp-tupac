#pragma once
#include "MazeElement.h"

class Target : public MazeElement {
public:
    char getSymbol() const override { return 'T'; }
};