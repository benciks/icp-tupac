#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "logic/game.h"
#include <QPainter>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContentsMargins(24, 24, 24, 24);
    setStyleSheet("background-color: #04080F;");

    Game *game = new Game();
    game->setGeometry(QRect(10, 10, 500, 500));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(game);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    game->paintMaze();
}

MainWindow::~MainWindow()
{
    delete ui;
}
