#include "pacman.h"
#include "ui_pacman.h"

PacMan::PacMan(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PacMan)
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked(bool)), QApplication::instance(), SLOT(quit()));
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(onStart()));
}

PacMan::~PacMan()
{
    delete ui;
}

void PacMan::onStart()
{
    // Change view, start the app here
    ui->startButton->hide();
    ui->exitButton->hide();
}
