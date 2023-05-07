#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "logic/game.h"
#include "logic/replay_ui.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QString>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QListWidget>
#include <QDialog>
#include <QDialogButtonBox>

void MainWindow::updateScoreLabel(int newScore)
{
    score = newScore;
    scoreLabel->setText(QString("Score: %1").arg(newScore));
}

#include <QMessageBox>

void MainWindow::loadFile()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::currentPath(),
        tr("Text Files (*.txt);;All Files (*)"));

    if (file.isEmpty())
    {
        QMessageBox messageBox(QMessageBox::Warning, tr("Error"), tr("<font color='red'>No file selected. Please select a file.</font>"), QMessageBox::Ok, this);
        messageBox.setTextFormat(Qt::RichText);
        messageBox.exec();
        return;
    }

    fileName = file;
    startGame();
}

void MainWindow::loadReplay()
{
    QString replayFolder;
#ifdef __APPLE__
    replayFolder = QCoreApplication::applicationDirPath() + "/../../../examples/replays";
#else
    replayFolder = QCoreApplication::applicationDirPath() + "/examples/replays";
#endif

    QDir replayDir(replayFolder);
    QStringList fileList = replayDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    QDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("Select a Replay File"));
    // fileDialog.setStyleSheet("QDialog { background-color: orange; } QListWidget { background-color: orange; color: black; } QPushButton { background-color: white; color: black; } QDialogButtonBox { background-color: white; }");
    fileDialog.setStyleSheet("QDialog { background-color: black; } QListWidget { background-color: black; color: white; } QPushButton { background-color: #4A298C; color: black; } QDialogButtonBox { background-color: black; }");
    QVBoxLayout *fileDialogLayout = new QVBoxLayout(&fileDialog);

    QLabel *label = new QLabel("Choose a file from the list below or browse:", &fileDialog);
    label->setStyleSheet("QLabel { color: white; }");
    fileDialogLayout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&fileDialog);
    listWidget->addItems(fileList);
    fileDialogLayout->addWidget(listWidget);

    QPushButton *browseButton = new QPushButton("Browse", &fileDialog);
    fileDialogLayout->addWidget(browseButton);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &fileDialog);
    fileDialogLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &fileDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &fileDialog, &QDialog::reject);
    connect(browseButton, &QPushButton::clicked, [&]()
            {
        QString file = QFileDialog::getOpenFileName(
            &fileDialog,
            tr("Open File"),
            QDir::currentPath(),
            tr("Text Files (*.txt);;All Files (*)"));

        if (!file.isEmpty())
        {
            listWidget->clear();
            listWidget->addItem(file);
            listWidget->setCurrentRow(0);
        } });

    if (fileDialog.exec() == QDialog::Accepted)
    {
        QString fileName = listWidget->currentItem()->text();
        QString filePath = replayDir.exists(fileName) ? replayDir.filePath(fileName) : fileName;
        if (filePath.isEmpty())
        {
            QMessageBox messageBox(QMessageBox::Warning, tr("Error"), tr("<font color='red'>No file selected. Please select a file.</font>"), QMessageBox::Ok, this);
            messageBox.setTextFormat(Qt::RichText);
            messageBox.exec();
            return;
        }

        replayFile = filePath;
    }
    else
    {
        return;
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QLabel *logoLabel = new QLabel();
    QPixmap logoPixmap(":images/data/logo.png");
    logoLabel->setPixmap(logoPixmap.scaled(logoPixmap.width() / 4, logoPixmap.height() / 4, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    QPushButton *startButton = new QPushButton("From start", this);
    startButton->setStyleSheet("background-color: #FFCC00; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont startFont = startButton->font();
    startFont.setWeight(QFont::Medium);
    startButton->setFont(startFont);
    layout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, [this]()
            {
    bool start = true;
    replayGame(start); });
    QPushButton *endButton = new QPushButton("From end", this);
    endButton->setStyleSheet("background-color: #4A298C; color: white; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont quitFont = endButton->font();
    quitFont.setWeight(QFont::Medium);
    endButton->setFont(quitFont);
    layout->addWidget(endButton);
    connect(endButton, &QPushButton::clicked, this, [this]()
            {
    bool start = false;
    replayGame(start); });

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::replayGame(bool start)
{
    ReplayUI *replayUI = new ReplayUI(this, replayFile, start);
    replayUI->setGeometry(QRect(10, 10, 500, 500));

    std::cout << "ReplayUI created" << std::endl;

    // Add a QLabel for displaying the legend
    QLabel *legendLabel = new QLabel(this);
    legendLabel->setText("Switch replay modes: Spacebar\n<In sequential>\nLeft arrow key: Go backwards\nRight arrow key: Go forwards\nEsc: quit replay mode");
    legendLabel->setStyleSheet("color: #FFFFFF;");
    legendLabel->setFont(QFont("Arial", 12));
    legendLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(replayUI, 30);
    layout->addWidget(legendLabel); // Add the legend label to the layout
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(replayUI, &ReplayUI::replayFinished, this, &MainWindow::loadMenu);
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

    QPushButton *quitButton = new QPushButton("Go to main menu", this);
    quitButton->setStyleSheet("background-color: #4A298C; color: white; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont quitFont = quitButton->font();
    quitFont.setWeight(QFont::Medium);
    quitButton->setFont(quitFont);
    layout->addWidget(quitButton);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::loadMenu);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::loadMenu()
{
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

    QPushButton *replayButton = new QPushButton("Replay", this);
    replayButton->setStyleSheet("background-color: #FF8300; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
    QFont replayFont = replayButton->font();
    replayFont.setWeight(QFont::Medium);
    replayButton->setFont(replayFont);
    layout->addWidget(replayButton);
    connect(replayButton, &QPushButton::clicked, this, &MainWindow::loadReplay);

    QPushButton *loadFileButton = new QPushButton("Load map from file", this);
    loadFileButton->setStyleSheet("background-color: #FF5733; color: black; font-size: 18px; padding: 6px 12px; border-radius: 16px; min-width: 200px;");
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContentsMargins(24, 24, 24, 24);
    setStyleSheet("background-color: #04080F;");

    loadMenu();
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
