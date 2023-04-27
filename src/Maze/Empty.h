#pragma once
#include "MazeElement.h"

#include <QObject>

class Empty : public MazeElement
{
public:
    char getSymbol() const override { return '.'; }
};