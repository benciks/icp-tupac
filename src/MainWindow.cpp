#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "logic/game.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QString>

void MainWindow::updateScoreLabel(int newScore)
{
    scoreLabel->setText(QString("Score: %1").arg(newScore));
}

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
    layout->addWidget(scoreLabel, 1);
    layout->addWidget(game, 30);
    layout->setContentsMargins(0, 0, 0, 0);

    scoreLabel->setText("Score: 0");
    scoreLabel->setStyleSheet("color: #FFFFFF;");
    scoreLabel->setFont(QFont("Arial", 16));
    scoreLabel->setMargin(0);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(game, &Game::scoreChanged, this, &MainWindow::updateScoreLabel); // Connect the signal to the slot

    game->paintMaze();
}

MainWindow::~MainWindow()
{
    delete ui;
}
