#include "pacman.h"
#include "ui_pacman.h"

PacMan::PacMan(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PacMan)
{
    ui->setupUi(this);
}

PacMan::~PacMan()
{
    delete ui;
}

