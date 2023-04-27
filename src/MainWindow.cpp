#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked(bool)), QApplication::instance(), SLOT(quit()));
    connect(ui->startButton, SIGNAL(clicked(bool)), game, SLOT(startGame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStart()
{
    // Change view, start the app here
    ui->startButton->hide();
    ui->exitButton->hide();
}
