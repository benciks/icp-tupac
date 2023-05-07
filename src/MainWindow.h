#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QLabel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startGame();
    void loadFile();
    void loadReplay();
    void loadMenu();
    void replayGame(bool start);

private:
    Ui::MainWindow *ui;
    QLabel *scoreLabel;
    int score = 0;
    QString fileName;
    QString replayFile;

private slots:
    void updateScoreLabel(int newScore);
    void gameOver(bool victory);
};
#endif // MAINWINDOW_H
