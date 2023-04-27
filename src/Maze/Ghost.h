#pragma once
#include "MazeElement.h"

class Ghost : public MazeElement {
public:
    char getSymbol() const override { return 'G'; }
};