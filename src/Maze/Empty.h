#pragma once
#include "MazeElement.h"

#include <QObject>

class Empty: public MazeElement {
public:
    Empty(QObject* parent = nullptr) : MazeElement(parent) {}

    char getSymbol() const override { return '.'; }
};