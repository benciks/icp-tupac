#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "logic/game.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContentsMargins(24, 24, 24, 24);
    setStyleSheet("background-color: #04080F;");

    Game *game = new Game(this);
    game->setGeometry(QRect(10, 10, 500, 500));

    scoreLabel = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(scoreLabel);
    layout->addWidget(game);
    layout->setContentsMargins(0, 0, 0, 0);

    scoreLabel->setText("Score: 0");
    scoreLabel->setStyleSheet("color: #FFFFFF;");
    scoreLabel->setFont(QFont("Arial", 16));

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    game->paintMaze();
}

MainWindow::~MainWindow()
{
    delete ui;
}
