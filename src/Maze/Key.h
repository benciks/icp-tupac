#pragma once
#include "MazeElement.h"

class Key: public MazeElement {
public:
    char getSymbol() const override { return 'K'; }
};