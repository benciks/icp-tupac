#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "logic/game.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QString>
#include <QPushButton>
#include <QFileDialog>

void MainWindow::updateScoreLabel(int newScore)
{
    score = newScore;
    scoreLabel->setText(QString("Score: %1").arg(newScore));
}

void MainWindow::loadFile()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::homePath(),
        tr("Text Files (*.txt);;All Files (*)"));

    fileName = file;
}

void MainWindow::gameOver(bool victory)
{
    // Add logo to the top
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QLabel *logoLabel = new QLabel();
    QPixmap logoPixmap(":images/data/logo.png"); // Replace with the path to your logo file
    logoLabel->setPixmap(logoPixmap.scaled(logoPixmap.width() / 4, logoPixmap.height() / 4, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    // Create a label for the score
    QLabel *endLabel;
    if (victory)
    {
        endLabel = new QLabel("You Win!", this);
    }
    else
    {
        endLabel = new QLabel("Game Over!", this);
    }
    endLabel->setAlignment(Qt::AlignCenter);
    endLabel->setStyleSheet("color: white; font-size: 32px;");
    layout->addWidget(endLabel);

    // Create a label for the score
    QLabel *scoreLabel = new QLabel("Final Score: " + QString::number(score), this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("color: white; font-size: 18px;");
    layout->addWidget(scoreLabel);

    QPushButton *startButton = new QPushButton("Restart", this);
    startButton->setStyleSheet("background-color: #FFCC00; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont startFont = startButton->font();
    startFont.setWeight(QFont::Medium);
    startButton->setFont(startFont);
    layout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    QPushButton *quitButton = new QPushButton("Quit", this);
    quitButton->setStyleSheet("background-color: #4A298C; color: white; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont quitFont = quitButton->font();
    quitFont.setWeight(QFont::Medium);
    quitButton->setFont(quitFont);
    layout->addWidget(quitButton);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContentsMargins(24, 24, 24, 24);
    setStyleSheet("background-color: #04080F;");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    // Add logo to the top
    QLabel *logoLabel = new QLabel();
    QPixmap logoPixmap(":images/data/logo.png"); // Replace with the path to your logo file
    logoLabel->setPixmap(logoPixmap.scaled(logoPixmap.width() / 4, logoPixmap.height() / 4, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    QPushButton *startButton = new QPushButton("Start", this);
    startButton->setStyleSheet("background-color: #FFCC00; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont startFont = startButton->font();
    startFont.setWeight(QFont::Medium);
    startButton->setFont(startFont);
    layout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    QPushButton *loadFileButton = new QPushButton("Load map from file", this);
    loadFileButton->setStyleSheet("background-color: #FF8300; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont loadFont = loadFileButton->font();
    loadFont.setWeight(QFont::Medium);
    loadFileButton->setFont(loadFont);
    layout->addWidget(loadFileButton);
    connect(loadFileButton, &QPushButton::clicked, this, &MainWindow::loadFile);

    QPushButton *quitButton = new QPushButton("Quit", this);
    quitButton->setStyleSheet("background-color: #4A298C; color: white; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont quitFont = quitButton->font();
    quitFont.setWeight(QFont::Medium);
    quitButton->setFont(quitFont);
    layout->addWidget(quitButton);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::startGame()
{
    std::cout << "Starting game" << std::endl;

    Game *game = new Game(this, fileName);
    game->setGeometry(QRect(10, 10, 500, 500));

    scoreLabel = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(scoreLabel, 0, Qt::AlignCenter); // Center the score label
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
    connect(game, &Game::gameOver, this, &MainWindow::gameOver);
}

MainWindow::~MainWindow()
{
    delete ui;
}
